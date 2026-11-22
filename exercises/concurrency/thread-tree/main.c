#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define NUM_THREADS 2
#define LEN 100

struct tS{
    int n;
    char str[100];
};

void *thread_function(void *par){
    //do a little syntax game to retrieve the passed values
    struct tS *tD;
    int n;
    char str[LEN];
    tD = (struct tS *) par;
    //passed values:
    n = tD->n;
    strcpy(str, tD->str);

    if(n==0){ //if this is the leaf thread, don't create two new threads but instead print the generation tree
        printf("%s\n", str); //print the generation tree
        return NULL; //terminate the thread
    }
    //Instead if this is not the leaf thread
    //create other 2 threads, and awaits for their termination
    pthread_t tid[NUM_THREADS];
    int i, result;
    //prepare the new n value to pass
    n--;
    //prepare the new str value to pass
    char myThreadNum[LEN];
    pthread_t tNum = pthread_self(); //store the current thread number to add to the string to pass
    sprintf(myThreadNum, "%lu", (unsigned long) tNum); //store the thread number in a string (to be put in the struct)
    strcat(str, " "); //concatenate (add) a space at the end of the string
    strcat(str, myThreadNum); //add to the generation string the current thread number
    //prepare the struct to pass
    struct tS newInfo;
    newInfo.n = n;
    strcpy(newInfo.str, str);
    //create the new threads
    for(i=0; i<NUM_THREADS; i++){
        result = pthread_create(&(tid[i]), NULL, thread_function, (void*) &newInfo);
        if(result != 0){
            fprintf(stderr, "Error creating thread\n");
            exit(EXIT_FAILURE);
        }
    }

    //wait for their termination and collect status
    void *status;
    for(i=0; i<NUM_THREADS; i++){
        result = pthread_join(tid[i], &status);
        if(result != 0){
            fprintf(stderr, "Error creating thread\n");
            exit(EXIT_FAILURE);
        }
    }
    return NULL; //thread terminates
}

int main(void){
    int n = 3; //input number

    pthread_t tid[NUM_THREADS]; //ids of the threads i'm gonna create
    int i, result;
    //prepare the n number to pass
    n--; //height of the tree
    //prepare the str to pass (with the thread number of the parent)
    char str[LEN];
    pthread_t tNum = pthread_self(); //store the thread number
    sprintf(str, "%lu", (unsigned long) tNum); //cast the thread number to a string (to be put in the struct)

    //prepare the struct to pass
    struct tS tD;
    tD.n = n;
    strcpy(tD.str, str);

    //create the threads
    for(i=0; i<NUM_THREADS; i++){
        //The parent thread creates two threads and waits for their termination.
        result = pthread_create(&(tid[i]), NULL, thread_function, (void*) &tD);
        if(result != 0){
            fprintf(stderr, "Error creating thread\n");
            exit(EXIT_FAILURE);
        }
    }
    
    //wait for their termination and collect status
    void *status;
    for(i=0; i<NUM_THREADS; i++){
        result = pthread_join(tid[i], &status);
        if(result != 0){
            fprintf(stderr, "Error creating thread\n");
            exit(EXIT_FAILURE);
        }
    }
    
    return 0;
}