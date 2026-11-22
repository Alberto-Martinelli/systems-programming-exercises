#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define LEN 30

typedef struct record_s {
    int line_num;
    long reg;
    char surname[LEN];
    char name[LEN];
    int mark;
}record_t;

record_t* search_student(char* filename, int student_num);
void overwrite_student(char* filename, record_t* ptr, int student_num);

int main(int argc, char** argv){
    char choice;
    int student_num, end=0;
    char filename[20] = "data_2.bin";
    printf("Menu: \n");
    printf("1. R <student number>\n");
    printf("2. W <student number>\n");
    printf("3. E (end program)\n");

    while(end==0){
        printf("Enter your choice:\n>");
        scanf(" %c %d", &choice, &student_num);
        // getchar();  // Consume any remaining newline characters in the input buffer
        // choice = 'E';
        // student_num = 2;
        switch(choice){
            case 'R':
                printf("You selected Option 1.\n");
                record_t* ptr = search_student(filename, student_num);
                printf("\n%d %li %s %s %d\n", ptr->line_num, ptr->reg, ptr->surname, ptr->name, ptr->mark);
                free(ptr);
                break;
            case 'W':
                printf("You selected Option 2.\nPlease provide data fields for the student %d\n", student_num);
                record_t* ptr2 = (record_t*)malloc(1*sizeof(record_t));
                scanf("%d %li %s %s %d", &(ptr2->line_num), &(ptr2->reg), ptr2->name, ptr2->surname, &(ptr2->mark));
                overwrite_student(filename, ptr2, student_num);
                free(ptr2);
                break;
            case 'E':
                end = 1;
                break;
        }
    }
    return 0;
}

void overwrite_student(char* filename, record_t* ptr, int student_num){
    int fd = open(filename, O_WRONLY);
    if(fd==-1){
        fprintf(stderr, "Error opening file\n");
        exit(EXIT_FAILURE);
    }
    lseek(fd, sizeof(record_t)*(student_num-1), SEEK_SET); //move the file pointer
    write(fd, ptr, sizeof(record_t));

    close(fd);
}

record_t* search_student(char* filename, int student_num){
    record_t* ptr = (record_t*)malloc(1*sizeof(record_t));
    int fd = open(filename, O_RDONLY);
    if(fd==-1){
        fprintf(stderr, "Error opening file\n");
        exit(EXIT_FAILURE);
    }
    lseek(fd, sizeof(record_t)*(student_num-1), SEEK_SET); //move the file pointer
    read(fd, ptr, sizeof(record_t));

    
    close(fd);
    return ptr;
}