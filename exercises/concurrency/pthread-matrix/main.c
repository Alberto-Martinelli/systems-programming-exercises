#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct info_s{
    int row;
    int x;
    int col;
    int** A;
    int** B;
    int** C;
} info_t;

void print_matrix(int**, int, int);
int** mat_alloc(int, int);
void* thread_function(void*);
void mat_mul (int **A, int **B, int r, int x, int c, int **C);

int main(void){
    int r=2, x=2, c=3;
    //allocate the matrices
    int** A = mat_alloc(r, x);
    int** B = mat_alloc(x, c);
    int** C = mat_alloc(r, c);

    //fill up the matrices
    A[0][0]=2; A[0][1]=4; A[1][0]=3; A[1][1]=7;
    B[0][0]=7; B[0][1]=0; B[1][0]=1; B[1][1]=2; B[0][2]=1; B[1][2]=1;

    mat_mul(A, B, r, x, c, C);

    printf("Product between ");
    print_matrix(A, r, x);
    printf("and ");
    print_matrix(B, x, c);
    printf("is ");
    print_matrix(C, r, c);

    return 0;
}

void* thread_function(void* par){
    int i, working_row, working_col, x, **A, **B, **C;
    info_t* coordinates;
    coordinates = (info_t*) par;
    working_row = coordinates->row;
    x = coordinates->x;
    working_col = coordinates->col;
    A = coordinates->A;
    B = coordinates->B;
    C = coordinates->C;

    int sum = 0;
    for(i=0; i<x; i++){
        sum += (A[working_row][i] * B[i][working_col]);
    }

    C[working_row][working_col] = sum;
    pthread_exit(NULL);
}

void mat_mul (int **A, int **B, int r, int x, int c, int **C){
    int i, j;
    int num_threads = r*c;
    //create an array of thread identifiers
    pthread_t* th = malloc(sizeof(pthread_t) * num_threads);

    //creat an array of structs to pass
    info_t* arr = malloc(sizeof(info_t)* num_threads);

    if(th==NULL || arr==NULL){
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }

    //create the threads
    int pos;
    for(i=0; i<r; i++){
        for(j=0; j<c; j++){
            //fill the struct (in the array) to be passed
            pos = i*c + j;
            // printf("Pos: %d\n", pos); //debug
            arr[pos].row = i;
            arr[pos].x = x;
            arr[pos].col = j;
            arr[pos].A = A;
            arr[pos].B = B;
            arr[pos].C = C;

            int val =pthread_create(&(th[pos]), NULL, thread_function, (void *)&(arr[pos]));
            if(val != 0){
                fprintf(stderr, "Error creating thread\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    
    //debug
    // for(i=0; i<num_threads; i++){
    //     printf("Thread: %lu\n", (unsigned long)th[i]);
    // }

    //wait for their termination and collect status
    void *status;
    int result;
    for(i=0; i<num_threads; i++){
        result = pthread_join(th[i], &status);
        if(result != 0){
            fprintf(stderr, "Error collecting thread status\n");
            exit(EXIT_FAILURE);
        }
    }

}

int** mat_alloc(int rows, int cols){
    int i;
    int** mat = (int**)malloc(sizeof(int*) * rows);
    if(mat==NULL){
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    for(i=0; i<rows; i++){
        mat[i] = (int*)malloc(sizeof(int) * cols);
        if(mat[i]==NULL){
            fprintf(stderr, "Memory allocation error\n");
            exit(EXIT_FAILURE);
        }
    }
    return mat;
}

void print_matrix(int** mat, int rows, int cols){
    int i, j;
    printf("matrix: \n");
    for(i=0; i<rows; i++){
        for(j=0; j<cols; j++){
            printf("%d ", mat[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}
