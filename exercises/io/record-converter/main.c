#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define LENGTH 30

typedef struct record_s {
    int line_num;
    long reg;
    char surname[LENGTH];
    char name[LENGTH];
    int mark;
}record_t;

void text_to_binary(char* txt_file, char* binary_file);
void binary_to_text(char* binary_file, char* txt_file2);

int main(int argc, char** argv){
    char filename1[20] = "data.txt";//argv[1];
    char filename2[20] = "data_2.bin";//argv[2];
    char filename3[20] = "data_3.txt";//argv[3];

    text_to_binary(filename1, filename2);
    binary_to_text(filename2, filename3);
    return 0;
}

void binary_to_text(char* binary_file, char* txt_file2){
    int fd = open(binary_file, O_RDONLY); //opens the binary file for reading 
    FILE *fp = fopen(txt_file2, "w"); //opens the txt file for writing
    if(fd==-1 || fp==NULL){
        fprintf(stderr, "Error opening file\n");
    }  
    
    record_t* ptr = (record_t*)malloc(1*sizeof(record_t));
    if (ptr==NULL){
            fprintf(stdout, "Memory allocation error\n");
            exit(EXIT_FAILURE);
    }
    
    while(read(fd, ptr, sizeof(record_t)) != 0){
        fprintf(fp, "%d %li %s %s %d\n", ptr->line_num, ptr->reg, ptr->surname, ptr->name, ptr->mark);
    }

    free(ptr);
    fclose(fp);
    close(fd);
}

void text_to_binary(char* txt_file, char* binary_file){

    FILE *fp = fopen(txt_file, "r"); //opens the txt file for reading
    int fd = open(binary_file, O_WRONLY); //opens the binary file for writing 
    if(fp == NULL || fd==-1){
        fprintf(stdout, "Error opening file\n");
        exit(EXIT_FAILURE);
    }
    
    record_t* ptr = (record_t*)malloc(1*sizeof(record_t));
    if (ptr==NULL){
            fprintf(stdout, "Memory allocation error\n");
            exit(EXIT_FAILURE);
    }

    while(fscanf(fp, "%d %li %s %s %d", &(ptr->line_num), &(ptr->reg), ptr->surname, ptr->name, &(ptr->mark))!=EOF){
        // printf("I read: %s %s\n", ptr->surname, ptr->name); for debug
        write(fd, ptr, sizeof(record_t));
    }


    free(ptr);
    close(fd);
    fclose(fp);
}