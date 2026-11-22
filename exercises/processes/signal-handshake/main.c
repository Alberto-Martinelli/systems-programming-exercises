#include "common.h"
#include <signal.h>
#include <sys/wait.h>

static int wait_signal(int expected) {
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, expected);
    int received;
    int error = sigwait(&set, &received);
    if (error != 0) {
        fprintf(stderr, "sigwait: %s\n", strerror(error));
        return -1;
    }
    return received == expected ? 0 : -1;
}

int main(int argc, char **argv) {
    unsigned long rounds = 3;
    if ((argc != 1 && argc != 2) ||
        (argc == 2 && parse_uint(argv[1], 1, 1000, &rounds) < 0)) {
        fprintf(stderr, "Usage: %s [ROUNDS]; rounds 1..1000\n", argv[0]);
        return EXIT_FAILURE;
    }
    sigset_t blocked, previous;
    sigemptyset(&blocked);
    sigaddset(&blocked, SIGUSR1);
    sigaddset(&blocked, SIGUSR2);
    if (sigprocmask(SIG_BLOCK, &blocked, &previous) < 0) {
        perror("sigprocmask");
        return EXIT_FAILURE;
    }
    setvbuf(stdout, NULL, _IONBF, 0);
    pid_t parent = getpid();
    pid_t child = fork();
    if (child < 0) {
        perror("fork");
        sigprocmask(SIG_SETMASK, &previous, NULL);
        return EXIT_FAILURE;
    }
    if (child == 0) {
        for (unsigned long i = 1; i <= rounds; ++i) {
            if (wait_signal(SIGUSR1) < 0 ||
                printf("CHILD %lu\n", i) < 0 || kill(parent, SIGUSR2) < 0)
                _exit(EXIT_FAILURE);
        }
        _exit(EXIT_SUCCESS);
    }
    int result = EXIT_SUCCESS;
    for (unsigned long i = 1; i <= rounds; ++i) {
        if (printf("PARENT %lu\n", i) < 0 || kill(child, SIGUSR1) < 0 ||
            wait_signal(SIGUSR2) < 0) {
            result = EXIT_FAILURE;
            kill(child, SIGTERM);
            break;
        }
    }
    int status;
    pid_t waited;
    do {
        waited = waitpid(child, &status, 0);
    } while (waited < 0 && errno == EINTR);
    if (waited < 0 || !WIFEXITED(status) || WEXITSTATUS(status) != 0)
        result = EXIT_FAILURE;
    if (sigprocmask(SIG_SETMASK, &previous, NULL) < 0) result = EXIT_FAILURE;
    return result;
}
