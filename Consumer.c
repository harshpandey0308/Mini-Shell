#include<semaphore.h>
#include<sys/mman.h>
#include<err.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<stdio.h>
#include<stdlib.h>
#include"share.h"

int main(){
    sem_t *sem_empty = sem_open("/p_empty" , 0);

    if(sem_empty == SEM_FAILED){
        err(EXIT_FAILURE , "semopen");
    }

    sem_t *sem_full = sem_open("/p_full" , 0);

    if(sem_full == SEM_FAILED){
        err(EXIT_FAILURE , "semopen full");
    }

    sem_t *sem_mutex = sem_open("/p_mutex" , 0);

    if(sem_mutex == SEM_FAILED){
        err(EXIT_FAILURE , "semopen mutex");
    }


    int shmfd = shm_open("/shm" , O_RDWR , 0);

    if(shmfd == -1){
        err(EXIT_FAILURE , "shmopen");
    }

    Shared_Mem *shm_map = mmap(NULL , sizeof(Shared_Mem) , PROT_READ | PROT_WRITE , MAP_SHARED , shmfd , 0);

    if(shm_map == MAP_FAILED){
        err(EXIT_FAILURE , "MMAP");
    }
    
    if(sem_wait(sem_full) == -1){
        err(EXIT_FAILURE , "semwait");
    }

    if(sem_wait(sem_mutex) == -1){
        err(EXIT_FAILURE , "semwait mutex");
    }

    char buffer[20];

    snprintf(buffer , sizeof(shm_map->buffer) , "%s" , shm_map->buffer[shm_map->read_index]);

    shm_map->read_index = (shm_map->read_index + 1)%SLOT;

    if(sem_post(sem_empty) == -1){
        err(EXIT_FAILURE , "sempost");
    }

    if(sem_post(sem_mutex) == -1){
        err(EXIT_FAILURE , "sempost mutex");
    }

    printf("updated read index = %d.\n",shm_map->read_index);
    printf("write_index = %d.\n",shm_map->write_index);
    printf("message received = %s.\n",buffer);

    return 0;
}