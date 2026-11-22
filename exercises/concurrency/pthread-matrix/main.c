#include "common.h"
#include <pthread.h>

typedef struct {
    const int *a;
    const int *b;
    int *c;
    size_t row, col, inner, cols;
} cell_task_t;

static void *multiply_cell(void *argument) {
    const cell_task_t *task = argument;
    int sum = 0;
    for (size_t k = 0; k < task->inner; ++k) {
        sum += task->a[task->row * task->inner + k] *
               task->b[k * task->cols + task->col];
    }
    /* Each worker owns exactly one output cell; no output lock is needed. */
    task->c[task->row * task->cols + task->col] = sum;
    return NULL;
}

int main(int argc, char **argv) {
    unsigned long rows = 2, inner = 2, cols = 3;
    if ((argc != 1 && argc != 4) ||
        (argc == 4 && (parse_uint(argv[1], 1, 32, &rows) < 0 ||
                       parse_uint(argv[2], 1, 32, &inner) < 0 ||
                       parse_uint(argv[3], 1, 32, &cols) < 0)) ||
        rows * cols > 128) {
        fprintf(stderr, "Usage: %s [ROWS INNER COLS]; dimensions 1..32, at most 128 workers\n",
                argv[0]);
        return EXIT_FAILURE;
    }
    size_t count = rows * cols;
    int *a = malloc(rows * inner * sizeof *a);
    int *b = malloc(inner * cols * sizeof *b);
    int *c = calloc(count, sizeof *c);
    pthread_t *threads = malloc(count * sizeof *threads);
    cell_task_t *tasks = malloc(count * sizeof *tasks);
    if (a == NULL || b == NULL || c == NULL || threads == NULL || tasks == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(a); free(b); free(c); free(threads); free(tasks);
        return EXIT_FAILURE;
    }
    for (size_t i = 0; i < rows * inner; ++i) a[i] = (int)i + 1;
    for (size_t i = 0; i < inner * cols; ++i) b[i] = (int)i + 1;
    size_t started = 0;
    int result = EXIT_SUCCESS;
    for (size_t row = 0; row < rows && result == EXIT_SUCCESS; ++row) {
        for (size_t col = 0; col < cols; ++col) {
            size_t position = row * cols + col;
            tasks[position] = (cell_task_t){a, b, c, row, col, inner, cols};
            int error = pthread_create(&threads[position], NULL, multiply_cell,
                                       &tasks[position]);
            if (error != 0) {
                fprintf(stderr, "pthread_create: %s\n", strerror(error));
                result = EXIT_FAILURE;
                break;
            }
            ++started;
        }
    }
    /* Argument storage and matrices stay alive until every started thread joins. */
    for (size_t i = 0; i < started; ++i) {
        int error = pthread_join(threads[i], NULL);
        if (error != 0) {
            fprintf(stderr, "pthread_join: %s\n", strerror(error));
            exit(EXIT_FAILURE);
        }
    }
    if (result == EXIT_SUCCESS) {
        for (size_t row = 0; row < rows; ++row) {
            for (size_t col = 0; col < cols; ++col) {
                printf("%s%d", col == 0 ? "" : " ", c[row * cols + col]);
            }
            putchar('\n');
        }
    }
    free(a); free(b); free(c); free(threads); free(tasks);
    return result;
}
