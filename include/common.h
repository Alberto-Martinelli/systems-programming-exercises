#ifndef SYSTEMS_EXERCISES_COMMON_H
#define SYSTEMS_EXERCISES_COMMON_H

#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Reject signs, whitespace, trailing characters, and out-of-range values. */
static inline int parse_uint(const char *text, unsigned long low,
                             unsigned long high, unsigned long *value) {
    if (text == NULL || *text == '\0') return -1;
    for (const char *p = text; *p != '\0'; ++p) {
        if (*p < '0' || *p > '9') return -1;
    }
    errno = 0;
    char *end = NULL;
    unsigned long parsed = strtoul(text, &end, 10);
    if (errno != 0 || *end != '\0' || parsed < low || parsed > high) return -1;
    *value = parsed;
    return 0;
}

static inline int write_all(int fd, const void *buffer, size_t length) {
    const unsigned char *cursor = buffer;
    while (length != 0) {
        ssize_t written = write(fd, cursor, length);
        if (written < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        if (written == 0) {
            errno = EIO;
            return -1;
        }
        cursor += (size_t)written;
        length -= (size_t)written;
    }
    return 0;
}

#endif
