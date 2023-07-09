#include <pthread.h>

#include "common.h" //W nim juz includujemy err.h

char *src_addr = NULL;
uint16_t data_port = DEFAULT_PORT;
uint64_t psize = 0; // Poznajemy w pierwszej paczce
uint64_t bsize = DEFAULT_BSIZE;
char *out_buf = NULL;
char *msg_buf = NULL;
uint64_t max_msg_len = DEFAULT_BSIZE + TWO_UINT64_T;

// Dla 2 wątków współdzielone,
uint64_t *iteration = NULL; // Wsatwiamy tam numer pakietu obebrany
uint64_t pack_space = 0;    // Ile odebranych pakietów mieści się w out_buf
uint64_t session_id = 0;    // Id obecnej sesji
_Atomic uint64_t max_n = 0; // Największy odebrany pakiet

inline static int get_message(int socket_fd, struct sockaddr_in *sender_addr,
                              socklen_t *address_length) {
    int msg_len = -1;
    while (1) {
        msg_len = recvfrom(socket_fd, msg_buf, max_msg_len, 0,
                           (struct sockaddr *)sender_addr, address_length);
        if (msg_len < 0) {
            fprintf(stderr, "Błąd podczas odbierania wiadomości\n");
            continue;
        }
        if ((*sender_addr).sin_addr.s_addr != inet_addr(src_addr)) {
            fprintf(stderr, "Odebrano wiadomość od niechcianego nadawcy\n");
            continue;
        }
        break; // Poprawnie odebrana paczka
    }
    return msg_len;
}

inline static void decode(uint64_t *current_session_id, uint64_t *current_byte,
                          char **audio_data) {
    memcpy(current_session_id, msg_buf, 8);
    *current_session_id = ntohll(*current_session_id);
    memcpy(current_byte, msg_buf + 8, 8);
    *current_byte = ntohll(*current_byte);
    *audio_data = msg_buf + TWO_UINT64_T;
}

void *thread_out(void *arg) {
    uint64_t thread_session = *((uint64_t *)arg);
    uint64_t i = 0;
    uint64_t index = 0;
    char quiet = 0;

    while (thread_session == session_id) {
        if (i > max_n) {
            fflush(stdout); // Czekamy na odbiornik
            continue;
        }
        index = i % pack_space;
        if (i > iteration[index]) {
            for (uint64_t j = 0; j < psize; j++) {
                fputc(quiet, stdout); // Cisza, paczka nie dotarła
            }
        } else { // i <= iteration[index]
            fwrite(out_buf + index * psize, sizeof(char), psize, stdout);
            i = iteration[index]; // Różnica to wielokrotność pack_size
        }
        fflush(stdout);
        i++;
    }

    pthread_detach(pthread_self());
    return NULL;
}

int main(int argc, char *argv[]) {
    int opt;
    while ((opt = getopt(argc, argv, "a:P:b:")) != -1) {
        switch (opt) {
        case 'a':
            src_addr = optarg;
            break;
        case 'P':
            data_port = read_port(optarg);
            break;
        case 'b':
            bsize = read_number(optarg);
            break;
        default:
            fprintf(stderr,
                    "Użycie: %s -a DEST_ADDR [-P DATA_PORT] [-b BSIZE] \n",
                    argv[0]);
            exit(EXIT_FAILURE);
        }
    }
    if (src_addr == NULL) {
        fprintf(stderr, "Nie podano adresu docelowego\n");
        exit(EXIT_FAILURE);
    }

    max_msg_len = bsize + TWO_UINT64_T;
    out_buf = allocate_buffer(bsize);       // Bufor na dane do odtworzenia
    msg_buf = allocate_buffer(max_msg_len); // Bufor na wiadomość

    // Inicjalizacja połączenia
    int socket_fd = create_socket();
    bind_socket(socket_fd, data_port);
    struct sockaddr_in sender_addr;
    socklen_t sender_addr_len = sizeof(sender_addr);

    // Zmienne do obsługi sesji
    uint64_t current_session_id = 0;
    uint64_t first_byte = 0, current_byte = 0;
    char *audio_data = NULL; // W celu przejrzystości kodu

    uint64_t n = 0;
    uint64_t byte_to_play = 0;
    uint64_t index = 0;
    uint64_t msg_len;
    bool started_out = false;

    while (1) {
        msg_len = get_message(socket_fd, &sender_addr, &sender_addr_len);
        decode(&current_session_id, &current_byte, &audio_data);

        if (current_session_id > session_id) { // nowa sesja
            session_id = current_session_id;
            max_n = 0;
            started_out = false;
            first_byte = current_byte;
            psize = msg_len - TWO_UINT64_T;
            memset(out_buf, 0, bsize);
            byte_to_play = first_byte + ((bsize * 3) / 4) - psize + 1;
            pack_space = bsize / psize;
            iteration = calloc(pack_space, sizeof(uint64_t));
        }

        if (current_session_id < session_id) {
            continue; // Pomijamy przestarzałe pakiety
        }

        n = (current_byte - first_byte) / psize;
        if (n + pack_space <= max_n) {
            continue; // Pomijamy pakiety tak stare, że się nie mieszczą.
        }

        // Dodajemy pakiet
        index = n % pack_space;
        memcpy(out_buf + index * psize, audio_data, psize);
        iteration[index] = n;
        if (n > max_n) {
            max_n = n;
        }
        // Wypisujemy braki pakietów
        uint64_t i = 0;
        if (max_n >= pack_space) {
            i = max_n - pack_space + 1; // Cykliczny bufor
        }
        while (i < n) {
            index = i % pack_space; // Już nie potrzebujemy tej zmiennej
            if (iteration[index] != i) {
                fprintf(stderr, "MISSING: BEFORE %lu EXPECTED %lu\n", n, i);
            }
            i++;
        }

        if (!started_out && current_byte >= byte_to_play) {
            started_out = true;
            pthread_t thread;
            while (pthread_create(&thread, NULL, thread_out, &session_id) !=
                   0) {
                fprintf(stderr, "Błąd tworzenia wątku pomocniczego.\n");
                sleep(1);
            }
        }
    }

    CHECK_ERRNO(close(socket_fd));
    return 0;
}
