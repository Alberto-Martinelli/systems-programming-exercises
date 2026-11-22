#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(void){
    char line[100];
    int fd[2]; //file descriptor with reading and writing end
    if (pipe(fd)==-1){
        fprintf(stderr, "Error creating pipe\n");
        exit(1);
    }
    pid_t pid=fork();
    if(pid==0){
        //child writes on pipe
        close(fd[0]); //closes reading end
        while(1){
            printf("Insert a string: \n");
            scanf("%s", line);
            write(fd[1], (void *)line, (strlen(line)+1)*sizeof(char));
        }
    }
    else{
        //parent reads on pipe
        close(fd[1]); //closes writing end
        while(1){
            read(fd[0], line, 100*sizeof(char));
            printf("%s\n", line);
        }
    }
    return 0;
}