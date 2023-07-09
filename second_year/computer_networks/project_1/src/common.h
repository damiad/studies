#ifndef MIMUW_SIK_UDP_SOCKETS_COMMON_H
#define MIMUW_SIK_UDP_SOCKETS_COMMON_H

#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "err.h"

#define NO_FLAGS 0
#define DEFAULT_PSIZE 512
#define DEFAULT_BSIZE 65536 // Zalecam zmiane na 512kB, czyli 524288
#define DEFAULT_PORT 20043
#define TWO_UINT64_T 16

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

inline static uint16_t read_port(char *string) {
    errno = 0;
    unsigned long port = strtoul(string, NULL, 10);
    PRINT_ERRNO();
    if (port > UINT16_MAX) {
        fatal("%ul is not a valid port number", port);
    }
    return (uint16_t)port;
}

inline static uint64_t read_number(char *string) {
    errno = 0;
    unsigned long long number = strtoull(string, NULL, 10);
    PRINT_ERRNO();

    if (number > UINT64_MAX) {
        fatal("%llu is out of range for uint64_t", number);
    }

    return (uint64_t)number;
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
    char *buffer = calloc(size, sizeof(char));
    if (buffer == NULL) {
        fprintf(stderr, "Błąd podczas alokacji bufora\n");
        exit(EXIT_FAILURE);
    }
    return buffer;
}

#endif // MIMUW_SIK_UDP_SOCKETS_COMMON_H
