#include<stdio.h>
#include<stdlib.h>
#include<err.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<sys/ipc.h>

#define SLOT 2
#define SIZE 20

typedef union semun{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *_buf;
}semun;

int main(){
    key_t key = ftok("/tmp" , 'B');

    if(key == -1){
        err(EXIT_FAILURE , "ftok");
    }

    int shmid = shmget(key , SLOT*SIZE , IPC_CREAT | 0666);

    if(shmid == -1){
        err(EXIT_FAILURE , "shmget");
    }

    int semid = semget(key , SLOT , IPC_CREAT | 0666);

    if(semid == -1){
        err(EXIT_FAILURE , "semget");
    }

    semun sem;

    sem.val = SLOT;

    if(semctl(semid , 0 , SETVAL , sem) == -1){
        err(EXIT_FAILURE , "semctl");
    }

    sem.val = 0;

    if(semctl(semid , 1 , SETVAL , sem) == -1){
        err(EXIT_FAILURE , "semctl");
    }

    pid_t pid = fork();

    if(pid == -1){
        err(EXIT_FAILURE , "pid");
    }

    if(pid == 0){
        char *shm = shmat(shmid , NULL , 0);

        if(shm == (void *)-1){
            err(EXIT_FAILURE , "shmat");
        }

        int consumer_index = 0;
        
        for(int i=0 ; i<SLOT ; i++){
            char buffer[20];
            struct sembuf sops;

            sops.sem_num = 1;
            sops.sem_op = -1;
            sops.sem_flg = 0;

            if(semop(semid , &sops , 1) == -1){
                err(EXIT_FAILURE , "semop full");
            }

            char *slot = shm + consumer_index*SIZE;

            strcpy(buffer , slot);

            printf("the received message is %s.\n",buffer);

            consumer_index = (consumer_index + 1)%SLOT;

            sops.sem_num = 0;
            sops.sem_op = 1;

            if(semop(semid , &sops , 1) == -1){
                err(EXIT_FAILURE , "semop empty");
            }
        }

        if(shmdt(shm) == -1){
          err(EXIT_FAILURE , "shmdt");
        }

        exit(EXIT_SUCCESS);
    }

    else{
        char *shmaddr = shmat(shmid , NULL , 0);

        if(shmaddr == (void *)-1){
            err(EXIT_FAILURE , "shmat");
        }

        int producer_index = 0;

        for(int i=0 ; i<SLOT ; i++){
            char buffer[SIZE];

            printf("enter the message : \n");

            if(fgets(buffer , sizeof(buffer) , stdin) == NULL){
                err(EXIT_FAILURE , "fgets");
            }

            struct sembuf sops;

            sops.sem_num = 0;
            sops.sem_op = -1;
            sops.sem_flg = 0;

            if(semop(semid , &sops , 1) == -1){
                err(EXIT_FAILURE , "semop empty");
            }

            char *slot = shmaddr + producer_index*SIZE;

            strcpy(slot , buffer);

            producer_index = (producer_index + 1)%SLOT;

            sops.sem_num = 1;
            sops.sem_op = 1;

            if(semop(semid , &sops , 1) == -1){
                err(EXIT_FAILURE , "semop full");
            }

        }
        
        wait(NULL);

        if(shmdt(shmaddr) == -1){
            err(EXIT_FAILURE , "shmdt");
        }

        shmctl(shmid , IPC_RMID , 0);

        semctl(semid , 0 , IPC_RMID);

        exit(EXIT_SUCCESS);
    }
}
