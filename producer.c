#include<semaphore.h>
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<unistd.h>
#include<err.h>

#define SLOT 4
#define SIZE 5

int main(){
    sem_t sem_addr  = sem_open("/shm" , O_CREAT , 0644 , 0);

    if(sem_addr == NULL){
        err(EXIT_FAILURE , "semaphore failed");
    }

    int shmfd = shm_open("/shm" , O_CREAT | O_RDWR , 0644);

    if(shmfd == -1){
        err(EXIR_FAILURE , "shmopen");
    }

    int shm_size = ftruncate(shmfd , 10);

    if(shm_size == -1){
        err(EXIT_FAILURE , "ftruncate");
    }

    void *map_mem = mmap(NULL , 10 , PROT_NONE , 5);

    if(map_mem == MAP_FAILED){
        err(EXIT_FAILURE , "mmap");
    }

    

    

}