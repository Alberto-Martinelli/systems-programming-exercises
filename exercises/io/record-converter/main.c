#include "records.h"

static int to_binary(const char *input_path, const char *output_path) {
    FILE *input = fopen(input_path, "r");
    if (input == NULL) {
        perror(input_path);
        return EXIT_FAILURE;
    }
    int output = open_distinct_output(fileno(input), output_path);
    if (output < 0) {
        perror(output_path);
        fclose(input);
        return EXIT_FAILURE;
    }
    int result = EXIT_SUCCESS;
    char line[256];
    unsigned long line_number = 0, next_id = 1;
    while (fgets(line, sizeof line, input) != NULL) {
        ++line_number;
        if (strchr(line, '\n') == NULL && !feof(input)) {
            fprintf(stderr, "Line %lu is too long\n", line_number);
            result = EXIT_FAILURE;
            break;
        }
        if (strspn(line, " \t\r\n") == strlen(line)) continue;
        record_t record = {0};
        if (parse_record(line, &record) < 0 || record.id != next_id) {
            fprintf(stderr, "Invalid record on line %lu; IDs must be sequential from 1\n",
                    line_number);
            result = EXIT_FAILURE;
            break;
        }
        if (write_record(output, &record) < 0) {
            perror("write record");
            result = EXIT_FAILURE;
            break;
        }
        ++next_id;
    }
    if (ferror(input)) {
        perror("read text");
        result = EXIT_FAILURE;
    }
    if (fclose(input) != 0) result = EXIT_FAILURE;
    if (close(output) != 0) result = EXIT_FAILURE;
    return result;
}

static int to_text(const char *input_path, const char *output_path) {
    int input = open(input_path, O_RDONLY);
    if (input < 0) {
        perror(input_path);
        return EXIT_FAILURE;
    }
    int output_fd = open_distinct_output(input, output_path);
    if (output_fd < 0) {
        perror(output_path);
        close(input);
        return EXIT_FAILURE;
    }
    FILE *output = fdopen(output_fd, "w");
    if (output == NULL) {
        perror("fdopen");
        close(input);
        close(output_fd);
        return EXIT_FAILURE;
    }
    int status, result = EXIT_SUCCESS;
    unsigned long next_id = 1;
    record_t record;
    while ((status = read_record(input, &record)) == 1) {
        if (record.id != next_id || print_record(output, &record) < 0) {
            fprintf(stderr, "Invalid record order or output error\n");
            result = EXIT_FAILURE;
            break;
        }
        ++next_id;
    }
    if (status < 0) {
        perror("read binary record");
        result = EXIT_FAILURE;
    }
    if (close(input) != 0) result = EXIT_FAILURE;
    if (fclose(output) != 0) result = EXIT_FAILURE;
    return result;
}

int main(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s to-binary|to-text INPUT OUTPUT\n", argv[0]);
        return EXIT_FAILURE;
    }
    if (strcmp(argv[1], "to-binary") == 0) return to_binary(argv[2], argv[3]);
    if (strcmp(argv[1], "to-text") == 0) return to_text(argv[2], argv[3]);
    fprintf(stderr, "Unknown conversion: %s\n", argv[1]);
    return EXIT_FAILURE;
}
