#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<sys/types.h>
#include<unistd.h>
#include<sys/shm.h>
#include<sys/sem.h>
#include<sys/ipc.h>

#define SIZE 100

struct sembuf{
    unsigned int sem_num;
    int sem_op;
    int sem_flg;
};

int main(){
    key_t key = ftok("/tmp" , 'A');  //key for getting identifier

    if(key == -1){
        printf("key failed");
        exit(EXIT_FAILURE);
    }

    int shmid = shmget(key , SIZE , IPC_CREAT | 0666);  // shared memory identifier

    int semid = semget(key , 1 , IPC_CREAT);   // semaphore set identifier

    if(shmid == -1){
        perror("Invalid id");
        exit(EXIT_FAILURE);
    }

    ssize_t sem = semctl(semid , 0 , SETVAL , 0);  // initializing value of semaphore 

    sem_buf *sops;  // a pointer to struct

    sops->sem_num = 0;
    sops->sem_op = 0;
    sops->sem_flg = 0;

    char *shaddr = shmat(shmid , NULL , 0); // getting address from shmat

    if(shaddr == (void *)-1){
      perror("failed");
      exit(EXIT_FAILURE);
    }

    pid_t pid = fork(); // generating child process

    if(pid == 0){
        printf("PID of child process is %d.\n", getpid());

        if(sops->sem_op > 0){
           char buffer[10];

           strcpy(buffer , shaddr);
        }
        else if(sops->sem_op < 0){
            
        }

        
       
        printf("Child process reading from the shared memory : %s.\n",buffer);
    }
    else if(pid > 0){
        printf("PID of parent process is %d.\n",getpid());

        strcpy(shaddr , "Hello hey");

        sops->sem_op = sops->sem_op - 1;

        int op_success = semop(semid , sops , 1);

        if(op_success == -1){
            perror("operation failed in parent process");
            exit(-1);
        }

        wait(NULL);

        exit(0);
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