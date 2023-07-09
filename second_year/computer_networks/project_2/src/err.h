#ifndef MIMUW_SIK_ERR_H
#define MIMUW_SIK_ERR_H

#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Evaluate `x`: if false, print an error message and exit with an error.
#define ENSURE(x)                                                              \
    do {                                                                       \
        bool result = (x);                                                     \
        if (!result) {                                                         \
            fprintf(stderr, "Error: %s was false in %s at %s:%d\n", #x,        \
                    __func__, __FILE__, __LINE__);                             \
            exit(EXIT_FAILURE);                                                \
        }                                                                      \
    } while (0)

// Check if errno is non-zero, and if so, print an error message and exit with
// an error.
#define PRINT_ERRNO()                                                          \
    do {                                                                       \
        if (errno != 0) {                                                      \
            fprintf(stderr, "Error: errno %d in %s at %s:%d\n%s\n", errno,     \
                    __func__, __FILE__, __LINE__, strerror(errno));            \
            exit(EXIT_FAILURE);                                                \
        }                                                                      \
    } while (0)

// Set `errno` to 0 and evaluate `x`. If `errno` changed, describe it and exit.
#define CHECK_ERRNO(x)                                                         \
    do {                                                                       \
        errno = 0;                                                             \
        (void)(x);                                                             \
        PRINT_ERRNO();                                                         \
    } while (0)

// Note: the while loop above wraps the statements so that the macro can be used
// with a semicolon for example: if (a) CHECK(x); else CHECK(y);

// Print an error message and exit with an error.
inline static void fatal(const char *fmt, ...) {
    va_list fmt_args;

    fprintf(stderr, "Error: ");
    va_start(fmt_args, fmt);
    vfprintf(stderr, fmt, fmt_args);
    va_end(fmt_args);
    fprintf(stderr, "\n");
    exit(EXIT_FAILURE);
}

#endif // MIMUW_SIK_ERR_H
