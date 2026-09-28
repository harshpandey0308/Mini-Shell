#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/shm.h>

#define SIZE 100

int main(){
    key_t key = ftok("/tmp" , 'A');

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

    char buffer[10];
    strcpy(shaddr , "Hello hey");

    printf("the shared memory contain : %s.\n",shaddr);

    int s = shmdt(shaddr);

    if(s == -1){
        perror("shmdt failed");
        exit(EXIT_FAILURE);
    }

    return 0;
}