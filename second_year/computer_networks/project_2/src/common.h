#ifndef MIMUW_SIK_UDP_SOCKETS_COMMON_H
#define MIMUW_SIK_UDP_SOCKETS_COMMON_H

#include <arpa/inet.h>
#include <ctype.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "err.h"

#define NO_FLAGS 0
#define DEFAULT_PSIZE 512
#define DEFAULT_BSIZE 65536 // Zalecam zmiane na 512kB, czyli 524288
#define DEFAULT_DATA_PORT 29954
#define DEFAULT_DISCOVER_ADDR "255.255.255.255"
#define DEFAULT_CTRL_PORT 39954
#define DEFAULT_UI_PORT 19954
#define DEFAULT_FSIZE 131072
#define DEFAULT_RTIME 250
#define DEFAULT_NAME "Nienazwany Nadajnik"
#define TWO_UINT64_T 16
#define MAX_UDP_MESSAGE_SIZE 65507 // 65535 - 20 - 8
#define MAX_STATION_NAME_LENGTH 64

inline static uint64_t htonll(uint64_t x) {
    if (htonl(1) == 1) { // Big-endian
        return x;
    } else { // Little-endian
        return (((uint64_t)htonl(x & 0xFFFFFFFF)) << 32) | htonl(x >> 32);
    }
}

inline static uint64_t ntohll(uint64_t x) {
    if (htonl(1) == 1) { // Big-endian
        return x;
    } else { // Little-endian
        return (((uint64_t)ntohl(x & 0xFFFFFFFF)) << 32) | ntohl(x >> 32);
    }
}

inline static void check_addr(const char *address) {
    struct in_addr addr; // todo pula adresów.. 224.0.0.1 – 239.255.255.25
    if (inet_pton(AF_INET, address, &(addr.s_addr)) != 1) {
        fprintf(stderr, "Invalid address%s\n", address);
        exit(EXIT_FAILURE);
    }
}

inline static uint16_t read_port(char *string) {
    errno = 0;
    char *endptr;
    unsigned long port = strtoul(string, &endptr, 10);
    // Sprawdza, czy cały argument został poprawnie przekonwertowany
    if (*endptr != '\0' || errno != 0) {
        fprintf(stderr, "Invalid port: %s\n", string);
        exit(EXIT_FAILURE);
    }
    // Sprawdza, czy port mieści się w zakresie UINT16_MAX
    if (port > UINT16_MAX) {
        fprintf(stderr, "Port number out of range: %lu\n", port);
        exit(EXIT_FAILURE);
    }
    return (uint16_t)port;
}

inline static uint32_t read_psize(char *string) {
    errno = 0;
    char *endptr;
    unsigned long psize = strtoul(string, &endptr, 10);
    // Sprawdza, czy cały argument został poprawnie przekonwertowany
    if (*endptr != '\0' || errno != 0) {
        fprintf(stderr, "Invalid PSIZE: %s\n", string);
        exit(EXIT_FAILURE);
    }
    // Sprawdza, czy psize ma odpowiedni zakres
    if (psize == 0 || psize > 65507 - TWO_UINT64_T) {
        fprintf(stderr, "Invalid PSIZE: %lu\n", psize);
        exit(EXIT_FAILURE);
    }
    return (uint32_t)psize;
}

inline static uint64_t read_fsize(char *string) {
    errno = 0;
    char *endptr;
    unsigned long long number = strtoull(string, &endptr, 10);
    // Sprawdza, czy cały argument został poprawnie przekonwertowany
    if (*endptr != '\0' || errno != 0) {
        fprintf(stderr, "Invalid number: %s\n", string);
        exit(EXIT_FAILURE);
    }
    return (uint64_t)number;
}

inline static uint64_t read_number(char *string) {
    errno = 0;
    char *endptr;
    unsigned long long number = strtoull(string, &endptr, 10);
    // Sprawdza, czy cały argument został poprawnie przekonwertowany
    if (*endptr != '\0' || errno != 0 || number == 0) {
        fprintf(stderr, "Invalid number: %s\n", string);
        exit(EXIT_FAILURE);
    }
    return (uint64_t)number;
}

inline static void check_name(const char *name) {
    if (name[0] == '\0') {
        fprintf(stderr, "Name cannot be empty\n");
        exit(EXIT_FAILURE);
    }
    if (strlen(name) > MAX_STATION_NAME_LENGTH) {
        fprintf(stderr, "Too long name: %s\n", name);
        exit(EXIT_FAILURE);
    }

    // Sprawdzenie spacji na brzegach
    if (isspace(name[0]) || isspace(name[strlen(name) - 1])) {
        fprintf(stderr, "Invalid spaces name sides: %s\n", name);
        exit(EXIT_FAILURE);
    }
    // Sprawdzenie zakresu znaków
    for (int i = 0; name[i] != '\0'; i++) {
        if (!isascii(name[i]) || name[i] < 32) {
            fprintf(stderr, "Invalid characters in name: %s\n", name);
            exit(EXIT_FAILURE);
        }
    }
}

inline static int create_socket() {
    int socket_fd = socket(AF_INET, SOCK_DGRAM, 0); // creating IPv4 UDP
    ENSURE(socket_fd >= 0);
    return socket_fd;
}

inline static struct sockaddr_in get_receiver_addr(const char *addr,
                                                   uint16_t port) {
    struct sockaddr_in receiver_addr;
    receiver_addr.sin_family = AF_INET;
    receiver_addr.sin_port = htons(port);
    ENSURE(inet_aton(addr, &receiver_addr.sin_addr) != 0);
    return receiver_addr;
}

inline static void bind_socket(int socket_fd, uint16_t port) {
    struct sockaddr_in receiver_addr;
    receiver_addr.sin_family = AF_INET;
    receiver_addr.sin_port = htons(port);
    receiver_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(socket_fd, (struct sockaddr *)&receiver_addr,
             sizeof(receiver_addr)) < 0) {
        fprintf(stderr, "Nie można powiązać gniazda z adresem\n");
        exit(EXIT_FAILURE);
    }
}

inline static char *allocate_buffer(uint64_t size) {
    char *buffer = (char *)calloc(size, sizeof(char));
    if (buffer == NULL) {
        fprintf(stderr, "Błąd podczas alokacji bufora\n");
        exit(EXIT_FAILURE);
    }
    return buffer;
}

inline static void send_message(int socket_fd,
                                const struct sockaddr_in *client_address,
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

inline static void send_multicast(int socket_fd, const void *message,
                                size_t length) {
    int flags = 0;
    while(send(socket_fd, message, length, flags) == -1) {
        fprintf(stderr, "Błąd podczas wysyłania datagramu UDP: %s\n",
                strerror(errno));
        usleep(1);
    }
}

inline static void set_port_reuse(int socket_fd) {
    int option_value = 1;
    CHECK_ERRNO(setsockopt(socket_fd, SOL_SOCKET, SO_REUSEPORT, &option_value,
                           sizeof(option_value)));
}

inline static void connect_socket(int socket_fd,
                                  const struct sockaddr_in *address) {
    CHECK_ERRNO(
        connect(socket_fd, (struct sockaddr *)address, sizeof(*address)));
}
#endif // MIMUW_SIK_UDP_SOCKETS_COMMON_H
