#include "common.h" //W nim pozostałe biblioteki

#define MAX_PACKETS_TO_RETRANSMIT 100000

char *mcast_addr = NULL;
uint16_t data_port = DEFAULT_DATA_PORT;
uint16_t ctrl_port = DEFAULT_CTRL_PORT;
uint32_t psize = DEFAULT_PSIZE;
uint64_t fsize = DEFAULT_FSIZE;
uint64_t rtime = DEFAULT_RTIME;
char *name = DEFAULT_NAME;

// Współdzielone przez wątki
pthread_mutex_t mutex_fifo = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_packet_buf = PTHREAD_MUTEX_INITIALIZER;
char ctrl_buf[MAX_UDP_MESSAGE_SIZE + 1];
char *fifo_buf = NULL;
int data_socket = -1;
struct sockaddr_in data_addr;
uint64_t session_id = 0;
uint64_t sequence_num = 0;
uint64_t packet_buf[MAX_PACKETS_TO_RETRANSMIT];
int packet_buf_len = 0;

int compare(const void *a, const void *b) {
    uint64_t numA = *((uint64_t *)a);
    uint64_t numB = *((uint64_t *)b);
    if (numA < numB)
        return -1;
    else if (numA > numB)
        return 1;
    else
        return 0;
}

void *control_thread_function(void *arg) {
    int ctrl_socket = create_socket();
    set_port_reuse(ctrl_socket);
    bind_socket(ctrl_socket, ctrl_port);
    struct sockaddr_in sender_addr;
    socklen_t sender_addr_len = sizeof(sender_addr);
    char temp[20];
    sprintf(temp, "%d", data_port);
    int digits = strlen(temp);
    char *reply_buffer = allocate_buffer(14 + strlen(mcast_addr) + 1 + digits +
                                         1 + strlen(name) + 1);
    sprintf(reply_buffer, "BOREWICZ_HERE %s %d %s\n", mcast_addr, data_port,
            name);

    int msg_len = -1;
    while (1) {
        msg_len = recvfrom(ctrl_socket, ctrl_buf, MAX_UDP_MESSAGE_SIZE, 0,
                           (struct sockaddr *)&sender_addr, &sender_addr_len);
        if (msg_len < 0) {
            fprintf(stderr, "Błąd podczas odbierania wiadomości\n");
            continue;
        } else {
            char *command = strtok(ctrl_buf, " ");
            if (strcmp(command, "ZERO_SEVEN_COME_IN\n") == 0) {
                send_message(ctrl_socket, &sender_addr, reply_buffer,
                             strlen(reply_buffer));
            } else if (fsize > 0 && strcmp(command, "LOUDER_PLEASE") == 0) {
                char *packet_numbers = strtok(NULL, "\n");
                if (packet_numbers != NULL) {
                    int isValid = 1; // czy podane numery pakietów są poprawne
                    uint64_t length = strlen(packet_numbers);
                    if (packet_numbers[0] == ',' ||
                        packet_numbers[length - 1] == ',')
                        isValid = 0;
                    for (uint64_t i = 0; i < length; i++) {
                        if (packet_numbers[i] != ',' &&
                            !isdigit(packet_numbers[i])) {
                            isValid = 0;
                            break;
                        }
                        if (i > 0 && packet_numbers[i] == ',' &&
                            packet_numbers[i - 1] == ',') {
                            isValid = 0;
                            break;
                        }
                    }
                    if (isValid) {
                        char *token = strtok(packet_numbers, ",");
                        while (token != NULL) {
                            unsigned long long packetNumber =
                                strtoull(token, NULL, 10);
                            pthread_mutex_lock(&mutex_packet_buf);
                            packet_buf[packet_buf_len++] = packetNumber;
                            pthread_mutex_unlock(&mutex_packet_buf);
                            token = strtok(NULL, ",");
                        }
                    }
                }
            }
        }
    }
    CHECK_ERRNO(close(ctrl_socket));
}

void *retransmition_thread_function(void *arg) {
    struct timeval startTime, currentTime;
    gettimeofday(&startTime, NULL);
    uint64_t packet_buf_cpy[MAX_PACKETS_TO_RETRANSMIT];
    int packet_buf_cpy_len = 0;
    char *retransmition_buf = allocate_buffer(psize + TWO_UINT64_T);
    memcpy(retransmition_buf, &session_id, 8);
    while (1) {
        gettimeofday(&currentTime, NULL);
        uint64_t elapsedTime = (currentTime.tv_sec - startTime.tv_sec) * 1000 +
                               (currentTime.tv_usec - startTime.tv_usec) / 1000;
        if (fsize > 0 && elapsedTime >= rtime) {
            startTime.tv_usec += rtime * 1000;
            startTime.tv_sec += startTime.tv_usec / 1000000;
            startTime.tv_usec %= 1000000;
            pthread_mutex_lock(&mutex_packet_buf);
            if (packet_buf_len == 0) {
                pthread_mutex_unlock(&mutex_packet_buf);
                continue;
            }
            packet_buf_cpy_len = packet_buf_len;
            memcpy(packet_buf_cpy, packet_buf,
                   packet_buf_len * sizeof(uint64_t));
            packet_buf_len = 0;
            pthread_mutex_unlock(&mutex_packet_buf);
            qsort(packet_buf_cpy, packet_buf_cpy_len, sizeof(uint64_t),
                  compare);
            for (int i = 0; i < packet_buf_len; i++) {
                if (i > 0 && packet_buf[i] == packet_buf[i - 1]) {
                    continue;
                }
                uint64_t wanted_sequence = packet_buf_cpy[i] / psize;
                pthread_mutex_lock(&mutex_fifo);
                if (wanted_sequence + fsize / psize > sequence_num &&
                    wanted_sequence < sequence_num) {
                    uint64_t start_position =
                        fsize + packet_buf_cpy[i] - sequence_num * psize;
                    uint64_t first_byte = htonll(packet_buf_cpy[i]);
                    memcpy(retransmition_buf + 8, &first_byte, 8);
                    memcpy(retransmition_buf + TWO_UINT64_T,
                           fifo_buf + start_position, psize);
                    pthread_mutex_unlock(&mutex_fifo);
                    send_multicast(data_socket, retransmition_buf,
                                   psize + TWO_UINT64_T);
                } else {
                    pthread_mutex_unlock(&mutex_fifo);
                }
            }
        }
    }
}

int main(int argc, char *argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, "a:P:C:p:f:R:n:")) != -1) {
        switch (opt) {
        case 'a':
            check_addr(optarg);
            mcast_addr = optarg;
            break;
        case 'P':
            data_port = read_port(optarg);
            break;
        case 'C':
            ctrl_port = read_port(optarg);
            break;
        case 'p':
            psize = read_psize(optarg);
            break;
        case 'f':
            fsize = read_fsize(optarg);
            break;
        case 'R':
            rtime = read_number(optarg);
            break;
        case 'n':
            check_name(optarg);
            name = optarg;
            break;
        default:
            fprintf(stderr,
                    "Użycie: %s -a MCAST_ADDR [-P DATA_PORT] [-C CTRL_PORT] "
                    "[-p PSIZE] [-f FSIZE] [-R RTIME] [-n NAME]\n",
                    argv[0]);
            exit(EXIT_FAILURE);
        }
    }
    if (mcast_addr == NULL) {
        fprintf(stderr, "Nie podano adresu rozgłaszania\n");
        exit(EXIT_FAILURE);
    }

    // Utwórz gniazdo UDP dla nadawania danych
    data_socket = create_socket();
    int optval = 1; // Ustal opcje gniazda dla rozgłaszania
    set_port_reuse(data_socket);
    CHECK_ERRNO(setsockopt(data_socket, SOL_SOCKET, SO_BROADCAST,
                           (void *)&optval, sizeof optval));
    optval = 4; /* ustawienie TTL dla datagramów rozsyłanych do grupy */
    CHECK_ERRNO(setsockopt(data_socket, IPPROTO_IP, IP_MULTICAST_TTL,
                           (void *)&optval, sizeof optval));
    // Utwórz strukturę adresu rozgłaszania
    data_addr = get_receiver_addr(mcast_addr, data_port);

    // Utwórz bufor dla danych odczytanych ze standardowego wejścia
    char *message_buf = allocate_buffer(psize + TWO_UINT64_T);
    char *audio_data = message_buf + TWO_UINT64_T;
    session_id = htonll(time(NULL));
    memcpy(message_buf, &session_id, 8);

    fsize = fsize - (fsize % psize);
    if (fsize > 0)
        fifo_buf = allocate_buffer(fsize);

    uint64_t bytes_read = 0;
    uint64_t total_bytes_read = 0;

    connect_socket(data_socket, &data_addr);

    pthread_t control_thread, retransmition_thread;
    int control_thread_result =
        pthread_create(&control_thread, NULL, control_thread_function, NULL);
    int retransmition_thread_result = pthread_create(
        &retransmition_thread, NULL, retransmition_thread_function, NULL);
    if (control_thread_result != 0 || retransmition_thread_result != 0) {
        fprintf(stderr, "Błąd podczas tworzenia wątków pomocniczych\n");
        exit(EXIT_FAILURE);
    }

    // Czytanie danych z wejścia i wysyłanie ich w pakietach
    while ((bytes_read = read(STDIN_FILENO, audio_data + total_bytes_read,
                              psize - total_bytes_read)) > 0) {
        total_bytes_read += bytes_read;
        // Jeśli odczytano cały pakiet, to go wysyłamy
        if (total_bytes_read == psize) {
            uint64_t first_byte = htonll(sequence_num * psize);
            memcpy(message_buf + 8, &first_byte, 8);
            send_multicast(data_socket, message_buf, psize + TWO_UINT64_T);
            if (fsize > 0) {
                pthread_mutex_lock(&mutex_fifo);
                if (fsize > psize) { // dodaje nowy pakiet na koniec
                    memcpy(fifo_buf, fifo_buf + psize, fsize - psize);
                    memcpy(fifo_buf + fsize - psize, audio_data, psize);
                }
                sequence_num++;
                pthread_mutex_unlock(&mutex_fifo);
            }
            total_bytes_read = 0;
        }
    }

    pthread_join(control_thread, NULL);
    pthread_join(retransmition_thread, NULL);
    free(message_buf);
    free(fifo_buf);
    CHECK_ERRNO(close(data_socket));
    return EXIT_SUCCESS;
}