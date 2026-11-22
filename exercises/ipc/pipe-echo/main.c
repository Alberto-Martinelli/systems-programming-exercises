#include "common.h"
#include <signal.h>
#include <sys/wait.h>

static int copy_stream(int input, int output) {
    unsigned char buffer[4096];
    for (;;) {
        ssize_t count = read(input, buffer, sizeof buffer);
        if (count < 0) {
            if (errno == EINTR) continue;
            return -1;
        }
        if (count == 0) return 0;
        if (write_all(output, buffer, (size_t)count) < 0) return -1;
    }
}

int main(int argc, char **argv) {
    if (argc != 1) {
        fprintf(stderr, "Usage: %s < INPUT > OUTPUT\n", argv[0]);
        return EXIT_FAILURE;
    }
    struct sigaction action = {0};
    action.sa_handler = SIG_IGN;
    sigemptyset(&action.sa_mask);
    if (sigaction(SIGPIPE, &action, NULL) < 0) {
        perror("sigaction");
        return EXIT_FAILURE;
    }
    int descriptors[2];
    if (pipe(descriptors) < 0) {
        perror("pipe");
        return EXIT_FAILURE;
    }
    pid_t child = fork();
    if (child < 0) {
        perror("fork");
        close(descriptors[0]); close(descriptors[1]);
        return EXIT_FAILURE;
    }
    if (child == 0) {
        close(descriptors[0]);
        int result = copy_stream(STDIN_FILENO, descriptors[1]);
        if (result < 0) perror("child copy");
        close(descriptors[1]);
        _exit(result == 0 ? EXIT_SUCCESS : EXIT_FAILURE);
    }
    close(descriptors[1]);
    int result = copy_stream(descriptors[0], STDOUT_FILENO);
    if (result < 0) perror("parent copy");
    close(descriptors[0]); // EOF or broken pipe lets the child finish.
    int status;
    pid_t waited;
    do {
        waited = waitpid(child, &status, 0);
    } while (waited < 0 && errno == EINTR);
    if (waited < 0) {
        perror("waitpid");
        return EXIT_FAILURE;
    }
    return result == 0 && WIFEXITED(status) && WEXITSTATUS(status) == 0
               ? EXIT_SUCCESS : EXIT_FAILURE;
}
