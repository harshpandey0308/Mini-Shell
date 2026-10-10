#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<err.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<sys/ipc.h>
#include<sys/wait.h>

#define SIZE 100

typedef union semun{
    int val;
    struct semid_ds *buf;
    unsigned short *arr;
    struct seminfo *_buf;
}semun;

int main(){
    struct sembuf sops;
    semun sem;
    int empty = 0;
    int full = 0;

    sops.sem_num = 0;
    sops.sem_op = 0;
    sops.sem_flg = 0;

    key_t key = ftok("/tmp" , 'A');

    if(key == -1){
        err(EXIT_FAILURE , "ftok");
    }

    int shmid = shmget(key , SIZE , IPC_CREAT | 0666);

    if(shmid == -1){
       err(EXIT_FAILURE , "shmget");
    }

    int semid = semget(key , 2 , IPC_CREAT | 0666);

    if(semid == -1){
        err(EXIT_FAILURE , "semget");
    }

    int n=2;

    empty = n;

    for(int i=0 ; i<n ; i++){
       if(semctl(semid , i , SETVAL , sem) == -1){
        err(EXIT_FAILURE , "semctl");
       }
    }
    
    sem.array[0] = 3;
    sem.array[1] = 0;

    pid_t pid = fork();

    if(pid == 0){
        char *shmptr = shmat(shmid , NULL , 0);

        if(shmptr == (void *)-1){
            err(EXIT_FAILURE , "shmat");
        }

        sem.arr[0] = 3;

        sops.sem_op = -1;

        int ss = semop(semid , &sops , 1);

        if(ss == -1){
            err(EXIT_FAILURE , "semop");
        }

        if(ss == 0){
            char buff[6];

            strcpy(buff , shmptr);

            printf("received message : %s.\n",buff);
        }

        signal(empty);

        

        int dt = shmdt(shmptr);

        if(dt == -1){
            err(EXIT_FAILURE , "shmdt");
        }

        exit(0);
    }
    else if(pid > 0){
        char *shmaddr = shmat(shmid , NULL , 0);

        if(shmaddr == (void *)-1){
            err(EXIT_FAILURE , "shmat");
        }

        

        while(wait(&empty) == 0){
            char buffer[10];

            if(fgets(buffer , sizeof(buffer) , stdin) == NULL){
                err(EXIT_FAILURE , "fgets");
            }

            sprintf(shmaddr , "%s" , buffer);

            sops.sem_op = 1;

            int sp = semop(semid , &sops , 1);

            if(sp == -1){
                err(EXIT_FAILURE , "semop");
            }

            signal(full);

        }

        int dt1 = shmdt(shmaddr);

        if(dt1 == -1){
            err(EXIT_FAILURE , "shmdt");
        }

        shmctl(shmid , IPC_RMID , NULL);

        semctl(semid , 0 , IPC_RMID);

        exit(0);
    }

    return 0;

}