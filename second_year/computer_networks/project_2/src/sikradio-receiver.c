#include "common.h" //W nim pozostałe biblioteki
#include <termios.h>

#define MAX_STATIONS 10000
#define MAX_ADDR_LEN 19

char *discover_addr = DEFAULT_DISCOVER_ADDR;
uint16_t ctrl_port = DEFAULT_CTRL_PORT;
uint16_t ui_port = DEFAULT_UI_PORT;
uint64_t psize = 0; // Poznajemy w pierwszej paczce
uint64_t bsize = DEFAULT_BSIZE;
uint64_t rtime = DEFAULT_RTIME;
char *out_buf = NULL;
char *msg_buf = NULL;
char *name = NULL;
uint16_t wanted_port = -1; // Port, na którym chcemy odbierać multicast
char *wanted_addr = NULL;  // Adres, na którym chcemy odbierać multicast

// Dane współdzielone
pthread_mutex_t mutex_stations = PTHREAD_MUTEX_INITIALIZER;
uint64_t *iteration = NULL; // Wsatwiamy tam numer pakietu obebrany
uint64_t pack_space = 0;    // Ile odebranych pakietów mieści się w out_buf
uint64_t session_id = 0;    // Id obecnej sesji
_Atomic uint64_t max_n = 0; // Największy odebrany pakiet
_Atomic uint64_t first_byte = 0;
int ctrl_socket = -1;
struct sockaddr_in broadcast_addr;

typedef struct {
    char mcast_addr[MAX_ADDR_LEN];
    int data_port;
    char station_name[MAX_STATION_NAME_LENGTH];
    struct timeval timestamp;
} StationInfo;

StationInfo stationList[MAX_STATIONS];
int numStations = 0;
int currentStationIndex = -1;

inline static void addStationToList(char *mcastAddr, int dataPort,
                                    char *stationName) {
    // Sprawdzanie, czy stacja już istnieje na liście
    for (int i = 0; i < numStations; i++) {
        if (strcmp(stationList[i].station_name, stationName) == 0) {
            gettimeofday(&stationList[i].timestamp, NULL);
            return;
        }
    }
    if (numStations >= MAX_STATIONS) {
        fprintf(stderr, "Przekroczono maksymalną liczbę stacji.\n");
        return;
    }
    StationInfo station;
    memcpy(station.mcast_addr, mcastAddr, MAX_ADDR_LEN);
    station.data_port = dataPort;
    memcpy(station.station_name, stationName, MAX_STATION_NAME_LENGTH);
    gettimeofday(&station.timestamp, NULL);
    pthread_mutex_lock(&mutex_stations);
    stationList[numStations] = station;
    (numStations)++;
    // Sortowanie stacji alfabetycznie
    int i, j;
    StationInfo key;
    for (i = 1; i < numStations; i++) {
        key = stationList[i];
        j = i - 1;
        while (j >= 0 &&
               strcmp(stationList[j].station_name, key.station_name) > 0) {
            stationList[j + 1] = stationList[j];
            j--;
        }
        stationList[j + 1] = key;
    }
    pthread_mutex_unlock(&mutex_stations);
}

inline static void removeExpiredStations() {
    struct timeval currentTime;
    gettimeofday(&currentTime, NULL);
    int i = 0;
    while (i < numStations) {
        double elapsedTime =
            difftime(currentTime.tv_sec, stationList[i].timestamp.tv_sec);
        if (elapsedTime >= 20.0) {
            // Usunięcie stacji, których czas od ostatniej aktualizacji wynosi
            // 20 sekund lub więcej
            pthread_mutex_lock(&mutex_stations);
            for (int j = i; j < numStations - 1; j++) {
                stationList[j] = stationList[j + 1];
            }
            (numStations)--;
            pthread_mutex_unlock(&mutex_stations);
        } else {
            i++;
        }
    }
}

inline static void printStationList() {
    removeExpiredStations(stationList, &numStations);
    // printf("Dostępne stacje:\n"); //Jeżeli chcemy wypisać na ekran
    // for (int i = 0; i < numStations; i++) {
    //     printf("Stacja %d:\n", i + 1);
    //     printf("  Adres multicast: %s\n", stationList[i].mcast_addr);
    //     printf("  Port danych: %d\n", stationList[i].data_port);
    //     printf("  Nazwa stacji: %s\n", stationList[i].station_name);
    //     struct timeval currentTime;
    //     gettimeofday(&currentTime, NULL);
    //     double elapsedTime =
    //         difftime(currentTime.tv_sec, stationList[i].timestamp.tv_sec);
    //     printf("  Czas od ostatniej aktualizacji: %.2f sekund\n",
    //     elapsedTime);
    // }
}

inline static int getStationIndex() {
    if (wanted_addr != NULL && wanted_port != -1) {
        for (int i = 0; i < numStations; i++) {
            if (strcmp(stationList[i].mcast_addr, wanted_addr) == 0 &&
                stationList[i].data_port == wanted_port) {
                return i;
            }
        }
        return -1;
    }
    // mamy juz mutexa
    if (name == NULL && numStations > 0) {
        return 0;
    }
    for (int i = 0; i < numStations; i++) {
        if (strcmp(stationList[i].station_name, name) == 0) {
            return i;
        }
    }
    return -1;
}

inline static void decode(uint64_t *current_session_id, uint64_t *current_byte,
                          char **audio_data) {
    memcpy(current_session_id, msg_buf, 8);
    *current_session_id = ntohll(*current_session_id);
    memcpy(current_byte, msg_buf + 8, 8);
    *current_byte = ntohll(*current_byte);
    *audio_data = msg_buf + TWO_UINT64_T;
}

// Funkcja do wysyłania aktualnej listy stacji do wszystkich podłączonych
// klientów
void sendStationListToClients(int clientSockets[], int numClients) {
    char message[MAX_UDP_MESSAGE_SIZE];
    memset(message, 0, sizeof(message));
    // Tworzenie wiadomości z listą stacji
    sprintf(message, "---------------------------------------------------------"
                     "---------------\n");
    sprintf(message + strlen(message), " SIK Radio\n");
    sprintf(message + strlen(message), "---------------------------------------"
                                       "---------------------------------\n");
    for (int i = 0; i < numStations; i++) {
        if (i == currentStationIndex) {
            sprintf(message + strlen(message), " > %s\n",
                    stationList[i].station_name);
        } else {
            sprintf(message + strlen(message), "   %s\n",
                    stationList[i].station_name);
        }
    }
    sprintf(message + strlen(message), "---------------------------------------"
                                       "---------------------------------\n");
    // Wysyłanie wiadomości do wszystkich klientów
    for (int i = 0; i < numClients; i++) {
        write(clientSockets[i], message, strlen(message));
    }
}

// Funkcja do odczytywania pojedynczego znaku bez buforowania
char getch() {
    struct termios old, new;
    char ch;
    tcgetattr(0, &old);
    new = old;
    new.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(0, TCSANOW, &new);
    ch = getchar();
    tcsetattr(0, TCSANOW, &old);
    return ch;
}

// Wątek obsługujący połączenia klientów
void *thread_tcp(void *arg) {
    int serverSocket, clientSocket;
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    // Tworzenie gniazda serwera
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        fprintf(stderr, "Błąd przy tworzeniu gniazda TCP\n");
        exit(EXIT_FAILURE);
    }
    bind_socket(serverSocket, ui_port);
    // Nasłuchiwanie na połączenia przychodzące
    if (listen(serverSocket, 10) == -1) {
        fprintf(stderr, "Błąd przy nasłuchiwaniu na połączenia\n");
        exit(EXIT_FAILURE);
    }
    // Tablica klientów
    int clientSockets[MAX_STATIONS];
    int numClients = 0;
    // Czas ostatniej aktualizacji listy stacji
    struct timeval lastUpdateTime;
    gettimeofday(&lastUpdateTime, NULL);

    while (1) {
        // Akceptowanie połączenia klienta
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr,
                              &clientAddrLen);
        if (clientSocket == -1) {
            fprintf(stderr, "Błąd przy akceptowaniu połączenia\n");
            usleep(1000);
            continue;
        }
        // Dodawanie klienta do tablicy klientów
        clientSockets[numClients++] = clientSocket;
        sendStationListToClients(&clientSocket, 1);
        // Odbieranie danych od klienta
        while (1) {
            char key = getch();
            // Poruszanie się po liście stacji strzałkami góra/dół
            if (key == '\033') {
                getch(); // skip [
                switch (getch()) {
                case 'A': // strzałka w górę
                    pthread_mutex_lock(&mutex_stations);
                    if (currentStationIndex > 0) {
                        currentStationIndex--;
                        wanted_addr =
                            stationList[currentStationIndex].mcast_addr;
                        wanted_port =
                            stationList[currentStationIndex].data_port;
                        sendStationListToClients(clientSockets, numClients);
                    }
                    pthread_mutex_unlock(&mutex_stations);
                    break;
                case 'B': // strzałka w dół
                    pthread_mutex_lock(&mutex_stations);
                    if (currentStationIndex < numStations - 1) {
                        currentStationIndex++;
                        wanted_addr =
                            stationList[currentStationIndex].mcast_addr;
                        wanted_port =
                            stationList[currentStationIndex].data_port;
                        sendStationListToClients(clientSockets, numClients);
                    }
                    pthread_mutex_unlock(&mutex_stations);
                    break;
                }
            }
            // Sprawdzenie, czy należy zaktualizować listę stacji
            struct timeval currentTime;
            gettimeofday(&currentTime, NULL);
            long elapsedTime =
                (currentTime.tv_sec - lastUpdateTime.tv_sec) * 1000 +
                (currentTime.tv_usec - lastUpdateTime.tv_usec) / 1000;
            if (elapsedTime >= rtime) {
                lastUpdateTime = currentTime;
                sendStationListToClients(clientSockets, numClients);
            }
        }
    }
    return NULL;
}

void *thread_out(void *arg) {
    uint64_t thread_session = *((uint64_t *)arg);
    uint64_t i = 0;
    uint64_t index = 0;
    char quiet = 0;
    while (thread_session == session_id) {
        if (i > max_n) {
            usleep(3);
            continue;
        }
        index = i % pack_space;
        if (i > iteration[index]) {
            //! Aby lepiej grało i odtwarzało cokolwiek w przypadku gubienia
            //! 1/50 paczek, nie czyścimy całkowicie bufora (zgodnie z treścią
            //! to zaznaczam)
            for (uint64_t j = 0; j < psize; j++) {
                fputc(quiet, stdout);
            }
        } else { // i <= iteration[index]
            fwrite(out_buf + index * psize, sizeof(char), psize, stdout);
            i = iteration[index]; // Różnica to wielokrotność pack_size
        }
        i++;
    }
    pthread_detach(pthread_self());
    return NULL;
}

void *control_thread_function(void *arg) {
    char *response_buf = allocate_buffer(MAX_UDP_MESSAGE_SIZE);
    struct sockaddr_in response_addr;

    char *lockup_buf = "ZERO_SEVEN_COME_IN\n";
    uint64_t lockup_size = strlen(lockup_buf);
    while (1) {
        send_message(ctrl_socket, &broadcast_addr, lockup_buf, lockup_size);
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(ctrl_socket, &readfds);

        struct timeval timeout;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        int maxfd = ctrl_socket + 1;
        int activity;
        while (1) {
            printStationList(stationList, numStations);
            // Czekanie na aktywność na gnieździe
            activity = select(maxfd, &readfds, NULL, NULL, &timeout);
            if (activity < 0) {
                continue;
            } else if (activity == 0) {
                break;
            }
            // Odczytanie odpowiedzi
            if (FD_ISSET(ctrl_socket, &readfds)) {
                socklen_t addrLen = sizeof(response_addr);
                int bytesRead =
                    recvfrom(ctrl_socket, response_buf, MAX_UDP_MESSAGE_SIZE, 0,
                             (struct sockaddr *)&response_addr, &addrLen);
                if (bytesRead < 0) {
                    fprintf(stderr, "Błąd podczas odbierania danych\n");
                    continue;
                }
                if (strncmp(response_buf, "BOREWICZ_HERE ", 14) == 0) {
                    char mcastAddr[MAX_ADDR_LEN];
                    int dataPort;
                    char stationName[MAX_STATION_NAME_LENGTH];
                    // Pomiń "BOREWICZ_HERE " (13 znaków + spacja)
                    char *ptr = response_buf + 14;
                    if (sscanf(ptr, "%s", mcastAddr) != 1) {
                        struct in_addr addr;
                        if (inet_pton(AF_INET, mcastAddr, &(addr.s_addr)) !=
                            1) {
                            continue;
                        }
                    }
                    ptr += strlen(mcastAddr) + 1;
                    if (sscanf(ptr, "%d", &dataPort) != 1 || dataPort < 0 ||
                        dataPort > UINT16_MAX) {
                        continue;
                    }
                    ptr = strchr(ptr, ' ') + 1;
                    char *end = strchr(ptr, '\n');
                    if (end == NULL) {
                        continue;
                    }
                    memcpy(stationName, ptr, end - ptr);
                    addStationToList(mcastAddr, dataPort, stationName);
                }
            }
        }
    }
    CHECK_ERRNO(close(ctrl_socket));
    return NULL;
}

void *retransmition_thread_function(void *arg) {
    char *ret_buf = allocate_buffer(MAX_UDP_MESSAGE_SIZE);
    strcpy(ret_buf, "LOUDER_PLEASE ");
    while (1) {
        uint64_t i = 0;
        uint64_t index = 0;
        if (max_n >= pack_space) {
            i = max_n - pack_space + 1; // Cykliczny bufor
        }
        while (i < max_n) {
            index = i % pack_space;
            if (iteration[index] != i) {
                char temp[20];
                sprintf(temp, "%lu,", i * psize + first_byte);
                strcat(ret_buf, temp);
            }
            i++;
        }
        if (strlen(ret_buf) > 14) {
            ret_buf[strlen(ret_buf) - 1] = '\n';
            send_message(ctrl_socket, &broadcast_addr, ret_buf,
                         strlen(ret_buf));
            memset(ret_buf, 0, MAX_UDP_MESSAGE_SIZE); // Wyczyszczenie bufora
            strcpy(ret_buf, "LOUDER_PLEASE "); // Ustawienie początkowego napisu
        }
        usleep(rtime * 1000);
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, "d:C:U:b:R:n:")) != -1) {
        switch (opt) {
        case 'd':
            check_addr(optarg);
            discover_addr = optarg;
            break;
        case 'C':
            ctrl_port = read_port(optarg);
            break;
        case 'U':
            ui_port = read_port(optarg);
            break;
        case 'b':
            bsize = read_number(optarg);
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
                    "Użycie: %s [-d DISCOVER_ADDR] [-C CTRL_PORT] [-U UI_PORT] "
                    "[-b BSIZE] [-R RTIME]\n",
                    argv[0]);
            exit(EXIT_FAILURE);
        }
    }

    ctrl_socket = create_socket();
    int optval = 1;
    CHECK_ERRNO(setsockopt(ctrl_socket, SOL_SOCKET, SO_BROADCAST,
                           (void *)&optval, sizeof optval));
    // Inicjalizacja struktury adresu broadcastu
    broadcast_addr = get_receiver_addr(discover_addr, ctrl_port);

    out_buf = allocate_buffer(bsize); // Bufor na dane do odtwarzenia
    msg_buf = allocate_buffer(MAX_UDP_MESSAGE_SIZE); // Bufor na odbiór
    int mcast_sock;
    mcast_sock = create_socket();
    set_port_reuse(mcast_sock);

    // Zmienne do obsługi sesji
    uint64_t current_session_id = 0;
    uint64_t current_byte = 0;
    char *audio_data = NULL; // W celu przejrzystości kodu

    uint64_t n = 0;
    uint64_t byte_to_play = 0;
    uint64_t index = 0;
    uint64_t msg_len;

    pthread_t control_thread, retransmition_thread;
    int control_thread_result =
        pthread_create(&control_thread, NULL, control_thread_function, NULL);
    int retransmition_thread_result = pthread_create(
        &retransmition_thread, NULL, retransmition_thread_function, NULL);
    if (control_thread_result != 0 || retransmition_thread_result != 0) {
        fprintf(stderr, "Błąd podczas tworzenia wątków pomocniczych\n");
        exit(EXIT_FAILURE);
    }

    pthread_t tcp_thread;
    int tcp_thread_result = pthread_create(&tcp_thread, NULL, thread_tcp, NULL);
    if (tcp_thread_result != 0) {
        fprintf(stderr, "Błąd podczas tworzenia wątku TCP\n");
        exit(EXIT_FAILURE);
    }

    bool started_out = false;
    while (1) {
        // znajdź stacje
        pthread_mutex_lock(&mutex_stations);
        int stationIndex = getStationIndex();
        currentStationIndex = stationIndex;
        if (stationIndex == -1) {
            pthread_mutex_unlock(&mutex_stations);
            sleep(1);
            continue;
        }
        StationInfo station = stationList[stationIndex];
        wanted_addr = station.mcast_addr;
        wanted_port = station.data_port;
        pthread_mutex_unlock(&mutex_stations);

        CHECK_ERRNO(close(mcast_sock));
        mcast_sock = create_socket();
        set_port_reuse(mcast_sock);

        /* podłączenie do grupy rozsyłania (ang. multicast) */
        struct ip_mreq ip_mreq;
        ip_mreq.imr_interface.s_addr = htonl(INADDR_ANY);
        if (inet_aton(station.mcast_addr, &ip_mreq.imr_multiaddr) == 0) {
            continue;
        }
        CHECK_ERRNO(setsockopt(mcast_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP,
                               (void *)&ip_mreq, sizeof ip_mreq));

        bind_socket(mcast_sock, station.data_port);
        // Ustawienie limitu czasowego na 20 sekund
        struct timeval timeout;
        timeout.tv_sec = 20;
        timeout.tv_usec = 0;
        bool received = false;
        while (strcmp(station.mcast_addr, wanted_addr) == 0 &&
               station.data_port == wanted_port) {
            fd_set read_fds;
            FD_ZERO(&read_fds);
            FD_SET(mcast_sock, &read_fds);
            int num_fds = mcast_sock + 1;
            int activity = select(num_fds, &read_fds, NULL, NULL, &timeout);
            if (activity < 01) {
                continue;
            } else if (activity == 0) {
                if (received) {
                    received = false;
                    timeout.tv_sec = 20;
                    timeout.tv_usec = 0;
                    continue;
                }
                pthread_mutex_lock(&mutex_stations);
                wanted_addr = NULL;
                wanted_port = -1;
                pthread_mutex_unlock(&mutex_stations);
                break; // Upłynęło 20 sekund bez otrzymania wiadomości
            } else {
                if (FD_ISSET(mcast_sock, &read_fds)) {
                    received = true;
                    msg_len =
                        recv(mcast_sock, msg_buf, MAX_UDP_MESSAGE_SIZE, 0);
                    if (msg_len < 0) {
                        fprintf(stderr, "Błąd podczas odbierania danych\n");
                        continue;
                    }
                    decode(&current_session_id, &current_byte, &audio_data);

                    if (current_session_id > session_id) { // nowa sesja
                        session_id = current_session_id;
                        max_n = 0;
                        started_out = false;
                        first_byte = current_byte;
                        psize = msg_len - TWO_UINT64_T;
                        memset(out_buf, 0, bsize);
                        byte_to_play =
                            first_byte + ((bsize * 3) / 4) - psize + 1;
                        pack_space = bsize / psize;
                        iteration = calloc(pack_space, sizeof(uint64_t));
                    }

                    if (current_session_id < session_id) {
                        continue; // Pomijamy przestarzałe pakiety
                    }

                    n = (current_byte - first_byte) / psize;
                    if (n + pack_space <= max_n) {
                        continue; // Pomijamy pakiety tak stare, że się nie
                                  // mieszczą.
                    }

                    // Dodajemy pakiet
                    index = n % pack_space;
                    memcpy(out_buf + index * psize, audio_data, psize);
                    iteration[index] = n;
                    if (n > max_n) {
                        max_n = n;
                    }

                    if (!started_out && (current_byte >= byte_to_play)) {
                        started_out = true;
                        pthread_t thread;
                        while (pthread_create(&thread, NULL, thread_out,
                                              &session_id) != 0) {
                            fprintf(stderr,
                                    "Błąd tworzenia wątku pomocniczego.\n");
                            usleep(1000);
                        }
                    }
                }
            }
        }
        /* odłączenie od grupy rozsyłania */
        CHECK_ERRNO(setsockopt(mcast_sock, IPPROTO_IP, IP_DROP_MEMBERSHIP,
                               (void *)&ip_mreq, sizeof(ip_mreq)));
    }

    CHECK_ERRNO(close(mcast_sock));
    pthread_join(control_thread, NULL);
    pthread_join(retransmition_thread, NULL);
    pthread_join(tcp_thread, NULL);
    return 0;
}