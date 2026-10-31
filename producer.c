#include<semaphore.h>
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<unistd.h>
#include<err.h>
#include"share.h"

int main(){
    sem_t *sem_empty  = sem_open("/p_empty" , O_CREAT | O_EXCL , 0644 , 4); // created a named semaphore

    if(sem_empty == SEM_FAILED){
        err(EXIT_FAILURE , "semaphore failed");
    }

    sem_t *sem_full  = sem_open("/p_full" , O_CREAT | O_EXCL , 0644 , 0);

    if(sem_full == SEM_FAILED){
        err(EXIT_FAILURE , "semaphore failed");
    }

    sem_t *sem_mutex  = sem_open("/p_mutex" , O_CREAT | O_EXCL , 0644 , 1);

    if(sem_mutex == SEM_FAILED){
        err(EXIT_FAILURE , "semaphore failed");
    }

    int shmfd = shm_open("/shm" , O_CREAT | O_EXCL | O_RDWR , 0644);   // created shared memory FD 

    if(shmfd == -1){
        err(EXIT_FAILURE , "shmopen");
    }

    int shm_size = ftruncate(shmfd , sizeof(Shared_Mem));    // initialize the size of the buffer

    if(shm_size == -1){
        err(EXIT_FAILURE , "ftruncate");
    }

    Shared_Mem *map_mem = mmap(NULL , sizeof(Shared_Mem) , PROT_READ | PROT_WRITE , MAP_SHARED , shmfd , 0);   //map the shared memory to the virtual address space of the calling process

    if(map_mem == MAP_FAILED){
        err(EXIT_FAILURE , "mmap");
    }

    map_mem->read_index = 0;
    map_mem->write_index = 0;

    printf("Read Index = %u.\n",map_mem->read_index);
    printf("Write Index = %u.\n",map_mem->write_index);
    printf("Size of shared memory = %ld.\n",sizeof(Shared_Mem));

    for(int i=0 ; i<4 ; i++){
       if(sem_wait(sem_empty) == -1){
          err(EXIT_FAILURE , "semwait");
        }

        if(sem_wait(sem_mutex) == -1){
            err(EXIT_FAILURE , "semwait mutex");
        }

        char buffer[5];

        printf("the message is .");
        scanf("%s",buffer);

        snprintf(map_mem->buffer[map_mem->write_index] , 5 , "%s" , buffer);

        map_mem->write_index = (map_mem->write_index + 1)%SLOT;

        if(sem_post(sem_mutex) == -1){
            err(EXIT_FAILURE , "sempost");
        }

        if(sem_post(sem_full) == -1){
            err(EXIT_FAILURE , "sempost full");
        }

        printf("updated write_index = %d.\n",map_mem->write_index);

        printf("the message is %s.\n" , map_mem->buffer[0]);

        printf("read_index = %d.\n",map_mem->read_index);

    }

    int mem_unmap = munmap(map_mem , sizeof(Shared_Mem));
    if(mem_unmap == -1){
        err(EXIT_FAILURE , "munmap");
    }

    int close_fd = close(shmfd);
    if(close_fd == -1){
        err(EXIT_FAILURE , "close");
    }
    
    return 0;

}