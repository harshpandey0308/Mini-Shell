#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/shm.h>

#define SIZE 100

typedef struct Msg{
    bool lock;
    char buffer[10];
}Msg;

int main(){
    key_t key = ftok("/tmp" , 'A');

    if(key == -1){
        printf("key failed");
        exit(EXIT_FAILURE);
    }


    int shmid = shmget(key , SIZE , IPC_CREAT | 0666);

    if(shmid == -1){
        perror("Invalid id");
        exit(EXIT_FAILURE);
    }

    char *shaddr = shmat(shmid , NULL , 0);

    if(shaddr == (void *)-1){
      perror("failed");
      exit(EXIT_FAILURE);
    }
    
    Msg *shared = (Msg *)shaddr;

    shared->lock = 0;

    pid_t pid = fork();

    if(pid == 0){
        printf("PID of child process is %d.\n", getpid());

        while(shared->lock != 1){
            printf("child is waiting");
        }

        char buffer[10];

        strcpy(buffer , shared->buffer);
       
        printf("Child process reading from the shared memory : %s.\n",buffer);
    }
    else if(pid > 0){
        printf("PID of parent process is %d.\n",getpid());

        while(shared->lock != 0){
            printf("parent is waiting");
        }

        strcpy(shared->buffer , "Hello hey");

        shared->lock = 0;

    }
    else{
        perror("shared memory failed");
        exit(EXIT_FAILURE);
    }

    int s = shmdt(shaddr);

    if(s == -1){
        perror("shmdt failed");
        exit(EXIT_FAILURE);
    }

    return 0;
}