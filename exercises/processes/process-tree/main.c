#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char** argv){
    int h, n;
    setbuf(stdout, 0);
    // printf("Insert height h:\n>");
    // scanf("%d", &h);
    // printf("Insert degree n:\n>");
    // scanf("%d", &n);
    h=3;
    n=2;
    int i, j, pid;

    for(j=0; j<h; j++){
        pid = 1;
        for(i=0; i<n; i++){
            if(pid!=0)
                pid = fork();
        }
        if(pid!=0)
            exit(1);
    }
    printf("Last: %d\n", getpid());
    sleep(3);
    return 0;
}