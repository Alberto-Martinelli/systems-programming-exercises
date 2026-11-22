#ifndef SYSTEMS_EXERCISES_RECORDS_H
#define SYSTEMS_EXERCISES_RECORDS_H

#include "common.h"
#include <ctype.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/stat.h>

/* Disk layout: u32 id, u32 registration, char[31] surname,
 * char[31] name, u32 mark. Integers are little endian: 74 bytes total.
 * This format deliberately contains no pointers or native struct padding. */
enum { RECORD_NAME_BYTES = 31, RECORD_BYTES = 74 };
typedef struct {
    uint32_t id;
    uint32_t registration;
    char surname[RECORD_NAME_BYTES];
    char name[RECORD_NAME_BYTES];
    uint32_t mark;
} record_t;

static inline uint32_t decode_u32(const unsigned char *p) {
    return (uint32_t)p[0] | ((uint32_t)p[1] << 8) |
           ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

static inline void encode_u32(unsigned char *p, uint32_t value) {
    for (unsigned int i = 0; i < 4; ++i) p[i] = (unsigned char)(value >> (8 * i));
}

static inline int valid_name(const char *name) {
    size_t length = strlen(name);
    if (length == 0 || length >= RECORD_NAME_BYTES) return 0;
    for (size_t i = 0; i < length; ++i) {
        if (isspace((unsigned char)name[i])) return 0;
    }
    return 1;
}

static inline int valid_record(const record_t *record) {
    return record->id >= 1 && record->id <= 1000000 &&
           record->registration >= 100000 && record->registration <= 999999 &&
           record->mark <= 30 && valid_name(record->surname) &&
           valid_name(record->name);
}

static inline int parse_record(const char *line, record_t *record) {
    char copy[256];
    size_t length = strlen(line);
    if (length >= sizeof copy) return -1;
    memcpy(copy, line, length + 1);
    char *fields[5];
    char *state = NULL;
    char *token = strtok_r(copy, " \t\r\n", &state);
    for (int i = 0; i < 5; ++i) {
        if (token == NULL) return -1;
        fields[i] = token;
        token = strtok_r(NULL, " \t\r\n", &state);
    }
    if (token != NULL || strlen(fields[2]) >= RECORD_NAME_BYTES ||
        strlen(fields[3]) >= RECORD_NAME_BYTES) return -1;
    unsigned long id, registration, mark;
    if (parse_uint(fields[0], 1, 1000000, &id) < 0 ||
        parse_uint(fields[1], 100000, 999999, &registration) < 0 ||
        parse_uint(fields[4], 0, 30, &mark) < 0) return -1;
    record->id = (uint32_t)id;
    record->registration = (uint32_t)registration;
    record->mark = (uint32_t)mark;
    strcpy(record->surname, fields[2]);
    strcpy(record->name, fields[3]);
    return valid_record(record) ? 0 : -1;
}

/* Return 1 for a record, 0 for clean EOF, -1 for invalid/truncated data or I/O. */
static inline int read_record(int fd, record_t *record) {
    unsigned char bytes[RECORD_BYTES];
    size_t offset = 0;
    while (offset < sizeof bytes) {
        ssize_t count = read(fd, bytes + offset, sizeof bytes - offset);
        if (count < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        if (count == 0) {
            if (offset == 0) return 0;
            errno = EINVAL;
            return -1;
        }
        offset += (size_t)count;
    }
    if (memchr(bytes + 8, 0, RECORD_NAME_BYTES) == NULL ||
        memchr(bytes + 39, 0, RECORD_NAME_BYTES) == NULL) {
        errno = EINVAL;
        return -1;
    }
    record->id = decode_u32(bytes);
    record->registration = decode_u32(bytes + 4);
    memcpy(record->surname, bytes + 8, RECORD_NAME_BYTES);
    memcpy(record->name, bytes + 39, RECORD_NAME_BYTES);
    record->mark = decode_u32(bytes + 70);
    if (!valid_record(record)) {
        errno = EINVAL;
        return -1;
    }
    return 1;
}

static inline int write_record(int fd, const record_t *record) {
    if (!valid_record(record)) {
        errno = EINVAL;
        return -1;
    }
    unsigned char bytes[RECORD_BYTES] = {0};
    encode_u32(bytes, record->id);
    encode_u32(bytes + 4, record->registration);
    memcpy(bytes + 8, record->surname, strlen(record->surname));
    memcpy(bytes + 39, record->name, strlen(record->name));
    encode_u32(bytes + 70, record->mark);
    return write_all(fd, bytes, sizeof bytes);
}

static inline int print_record(FILE *stream, const record_t *record) {
    return fprintf(stream, "%u %u %s %s %u\n", (unsigned int)record->id,
                   (unsigned int)record->registration, record->surname,
                   record->name, (unsigned int)record->mark) < 0 ? -1 : 0;
}

/* Check device/inode before truncating, including hard links to the input. */
static inline int open_distinct_output(int input_fd, const char *path) {
    int output_fd = open(path, O_WRONLY | O_CREAT, 0644);
    if (output_fd < 0) return -1;
    struct stat input_info, output_info;
    if (fstat(input_fd, &input_info) < 0 || fstat(output_fd, &output_info) < 0) {
        int saved = errno;
        close(output_fd);
        errno = saved;
        return -1;
    }
    if (input_info.st_dev == output_info.st_dev &&
        input_info.st_ino == output_info.st_ino) {
        close(output_fd);
        errno = EINVAL;
        return -1;
    }
    if (ftruncate(output_fd, 0) < 0) {
        int saved = errno;
        close(output_fd);
        errno = saved;
        return -1;
    }
    return output_fd;
}

#endif
