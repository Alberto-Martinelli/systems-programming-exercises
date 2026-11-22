#include "common.h"
#include <pthread.h>

enum { MAX_DEPTH = 6 };
typedef struct {
    unsigned int remaining;
    char path[MAX_DEPTH + 1];
    int result;
} node_t;

static int walk(node_t *node);

static void *thread_main(void *argument) {
    node_t *node = argument;
    node->result = walk(node);
    return NULL;
}

static int walk(node_t *node) {
    if (node->remaining == 0) {
        return printf("%s\n", node->path[0] == '\0' ? "root" : node->path) < 0;
    }
    pthread_t threads[2];
    node_t children[2] = {0};
    size_t length = strlen(node->path);
    int started = 0, result = 0;
    for (int i = 0; i < 2; ++i) {
        children[i].remaining = node->remaining - 1;
        memcpy(children[i].path, node->path, length);
        children[i].path[length] = i == 0 ? 'L' : 'R';
        children[i].path[length + 1] = '\0';
        int error = pthread_create(&threads[i], NULL, thread_main, &children[i]);
        if (error != 0) {
            fprintf(stderr, "pthread_create: %s\n", strerror(error));
            result = 1;
            break;
        }
        ++started;
    }
    /* Each child reads its own argument; both arguments live until these joins. */
    for (int i = 0; i < started; ++i) {
        int error = pthread_join(threads[i], NULL);
        if (error != 0) {
            fprintf(stderr, "pthread_join: %s\n", strerror(error));
            exit(EXIT_FAILURE);
        }
        if (children[i].result != 0) result = 1;
    }
    return result;
}

int main(int argc, char **argv) {
    unsigned long depth = 3;
    if ((argc != 1 && argc != 2) ||
        (argc == 2 && parse_uint(argv[1], 0, MAX_DEPTH, &depth) < 0)) {
        fprintf(stderr, "Usage: %s [DEPTH]; depth 0..%d\n", argv[0], MAX_DEPTH);
        return EXIT_FAILURE;
    }
    node_t root = {.remaining = (unsigned int)depth, .path = "", .result = 0};
    return walk(&root) == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
