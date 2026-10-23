#include<semaphore.h>
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<unistd.h>
#include<err.h>
#include"share.h"

#define SLOT 4
#define SIZE 10

int main(){
    sem_t *sem_addr  = sem_open("/shm" , O_CREAT , 0644 , 0); // created a named semaphore

    if(sem_addr == SEM_FAILED){
        err(EXIT_FAILURE , "semaphore failed");
    }

    int shmfd = shm_open("/shm" , O_CREAT | O_RDWR , 0644);   // created shared memory FD 

    if(shmfd == -1){
        err(EXIT_FAILURE , "shmopen");
    }

    int shm_size = ftruncate(shmfd , SIZE);    // initialize the size of the buffer

    if(shm_size == -1){
        err(EXIT_FAILURE , "ftruncate");
    }

    void *map_mem = mmap(NULL , 10 , MAP_SHARED , shmfd , 5);   //map the shared memory to the virtual address space of the calling process

    if(map_mem == MAP_FAILED){
        err(EXIT_FAILURE , "mmap");
    }

    
    if(sem_wait(empty) == -1){
        err(EXIT_FAILURE , "semwait");
    }


    

}