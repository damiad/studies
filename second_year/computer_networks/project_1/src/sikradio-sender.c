#include <time.h>

#include "common.h" //W nim juz includujemy err.h

void send_message(int socket_fd, const struct sockaddr_in *client_address,
                  const char *message, size_t length) {
    socklen_t address_length = (socklen_t)sizeof(*client_address);
    int flags = 0;
    // Wysyła do skutku
    while (sendto(socket_fd, message, length, flags,
                  (struct sockaddr *)client_address, address_length) == -1) {
        fprintf(stderr, "Błąd podczas wysyłania datagramu UDP: %s\n",
                strerror(errno));
        usleep(1);
    }
}

int main(int argc, char *argv[]) {
    char *dest_addr = NULL;
    uint16_t data_port = DEFAULT_PORT;
    uint64_t psize = DEFAULT_PSIZE;
    char *name = "Nienazwany Nadajnik";
    int opt;
    while ((opt = getopt(argc, argv, "a:P:p:n:")) != -1) {
        switch (opt) {
        case 'a':
            dest_addr = optarg;
            break;
        case 'P':
            data_port = read_port(optarg);
            break;
        case 'p':
            psize = read_number(optarg);
            break;
        case 'n':
            name = optarg;
            break;
        default:
            fprintf(stderr,
                    "Użycie: %s -a DEST_ADDR [-P DATA_PORT] [-p PSIZE] [-n "
                    "NAZWA]\n",
                    argv[0]);
            exit(EXIT_FAILURE);
        }
    }
    if (dest_addr == NULL) {
        fprintf(stderr, "Nie podano adresu docelowego\n");
        exit(EXIT_FAILURE);
    }
    if (name == NULL) // Unikniecię warninga
        ;

    int socket_fd = create_socket();
    struct sockaddr_in receiver_addr = get_receiver_addr(dest_addr, data_port);

    char *buf = allocate_buffer(psize + TWO_UINT64_T);
    char *audio_data = buf + TWO_UINT64_T;

    const uint64_t session_id = htonll(time(NULL));

    uint64_t sequence_num = 0;

    uint64_t bytes_read = 0;
    uint64_t total_bytes_read = 0;

    // Czytanie danych z wejścia i wysyłanie ich w pakietach
    while ((bytes_read = read(STDIN_FILENO, audio_data + total_bytes_read,
                              psize - total_bytes_read)) > 0) {
        total_bytes_read += bytes_read;

        // Jeśli odczytano cały pakiet, to go wysyłamy
        if (total_bytes_read == psize) {

            uint64_t first_byte = htonll(sequence_num * psize);
            memcpy(buf, &session_id, 8);
            memcpy(buf + 8, &first_byte, 8);
            send_message(socket_fd, &receiver_addr, buf, psize + TWO_UINT64_T);

            // Zbieramy kolejny pakiet
            sequence_num++;
            total_bytes_read = 0;
        }
    }

    CHECK_ERRNO(close(socket_fd));
    return EXIT_SUCCESS;
}