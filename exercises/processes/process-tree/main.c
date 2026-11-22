#include "common.h"
#include <sys/wait.h>

enum { MAX_HEIGHT = 4, MAX_BRANCHES = 4 };

static int grow(unsigned int height, unsigned int branches, const char *path) {
    if (height == 0)
        return printf("%s\n", path[0] == '\0' ? "root" : path) < 0;
    pid_t children[MAX_BRANCHES];
    unsigned int started = 0;
    int result = 0;
    for (unsigned int branch = 0; branch < branches; ++branch) {
        pid_t child = fork();
        if (child < 0) {
            perror("fork");
            result = 1;
            break;
        }
        if (child == 0) {
            char child_path[MAX_HEIGHT * 2 + 1];
            int length = snprintf(child_path, sizeof child_path, "%s%s%u",
                                  path, path[0] == '\0' ? "" : ".", branch);
            if (length < 0 || (size_t)length >= sizeof child_path) _exit(1);
            _exit(grow(height - 1, branches, child_path));
        }
        children[started++] = child;
    }
    for (unsigned int i = 0; i < started; ++i) {
        int status;
        pid_t waited;
        do {
            waited = waitpid(children[i], &status, 0);
        } while (waited < 0 && errno == EINTR);
        if (waited < 0 || !WIFEXITED(status) || WEXITSTATUS(status) != 0)
            result = 1;
    }
    return result;
}

int main(int argc, char **argv) {
    unsigned long height = 2, branches = 2;
    if ((argc != 1 && argc != 3) ||
        (argc == 3 && (parse_uint(argv[1], 0, MAX_HEIGHT, &height) < 0 ||
                       parse_uint(argv[2], 1, MAX_BRANCHES, &branches) < 0))) {
        fprintf(stderr, "Usage: %s [HEIGHT BRANCHES]; height 0..4, branches 1..4\n",
                argv[0]);
        return EXIT_FAILURE;
    }
    setvbuf(stdout, NULL, _IONBF, 0);
    return grow((unsigned int)height, (unsigned int)branches, "") == 0
               ? EXIT_SUCCESS : EXIT_FAILURE;
}
