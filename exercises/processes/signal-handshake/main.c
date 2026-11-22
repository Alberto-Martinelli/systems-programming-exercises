//Write a program that, using signals, synchronize two processes (a parent and it child) 
//in a way that they print alternatively a specific message.
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void func(int t);

int main(int argc, char *argv[]){
    pid_t pid;
    pid = fork();
    //signal handler: When I receive SIGINT, I do nothing
    signal(SIGCHLD, &func);
    if(pid == -1){
        fprintf(stderr, "Error forking the process\n");
        exit(EXIT_FAILURE);
    }
    while(1){
        if(pid == 0){
            //Child executes
            pause();
            sleep(1);
            fprintf(stdout, "Child Woke-up\n");
            kill(getppid(), SIGCHLD);   
        }
        else{
            //parent executes
            sleep(1);
            fprintf(stdout, "Father Woke-up\n");
            kill(pid/*child pid*/, SIGCHLD);
            pause();
        }
    }
    


    return EXIT_SUCCESS;
}

void func(int t){};