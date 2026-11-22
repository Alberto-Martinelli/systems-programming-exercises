#include "records.h"

static int usage(const char *program) {
    fprintf(stderr, "Usage: %s read FILE INDEX\n"
                    "       %s write FILE INDEX REGISTRATION SURNAME NAME MARK\n",
                    program, program);
    return EXIT_FAILURE;
}

int main(int argc, char **argv) {
    if (argc < 2) return usage(argv[0]);
    int writing = strcmp(argv[1], "write") == 0;
    if ((!writing && strcmp(argv[1], "read") != 0) ||
        argc != (writing ? 8 : 4)) return usage(argv[0]);
    unsigned long index;
    if (parse_uint(argv[3], 1, 1000000, &index) < 0) return usage(argv[0]);

    record_t record = {0};
    if (writing) {
        unsigned long registration, mark;
        if (parse_uint(argv[4], 100000, 999999, &registration) < 0 ||
            parse_uint(argv[7], 0, 30, &mark) < 0 ||
            strlen(argv[5]) >= RECORD_NAME_BYTES ||
            strlen(argv[6]) >= RECORD_NAME_BYTES) return usage(argv[0]);
        record.id = (uint32_t)index;
        record.registration = (uint32_t)registration;
        record.mark = (uint32_t)mark;
        strcpy(record.surname, argv[5]);
        strcpy(record.name, argv[6]);
        if (!valid_record(&record)) return usage(argv[0]);
    }
    int fd = open(argv[2], writing ? O_RDWR : O_RDONLY);
    if (fd < 0) {
        perror(argv[2]);
        return EXIT_FAILURE;
    }
    struct stat info;
    if (fstat(fd, &info) < 0) {
        perror("fstat");
        close(fd);
        return EXIT_FAILURE;
    }
    if (!S_ISREG(info.st_mode) || info.st_size % RECORD_BYTES != 0 ||
        (off_t)index > info.st_size / RECORD_BYTES + (writing ? 1 : 0)) {
        fprintf(stderr, "Invalid file size or record index; writes may append one record\n");
        close(fd);
        return EXIT_FAILURE;
    }
    if (lseek(fd, (off_t)(index - 1) * RECORD_BYTES, SEEK_SET) < 0) {
        perror("lseek");
        close(fd);
        return EXIT_FAILURE;
    }
    int result = EXIT_SUCCESS;
    if (writing) {
        if (write_record(fd, &record) < 0) {
            perror("write record");
            result = EXIT_FAILURE;
        }
    } else {
        if (read_record(fd, &record) != 1 || record.id != index) {
            fprintf(stderr, "Invalid or missing record\n");
            result = EXIT_FAILURE;
        } else if (print_record(stdout, &record) < 0) {
            result = EXIT_FAILURE;
        }
    }
    if (close(fd) != 0) result = EXIT_FAILURE;
    return result;
}
