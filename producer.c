#include<semaphore.h>
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/mman.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<signal.h>
#include<unistd.h>
#include<err.h>
#include<time.h>
#include<errno.h>
#include<string.h>
#include"share.h"

#define TIMED_OUT 40

int main(){
    struct timespec tp;


    sem_t *sem_consumer = sem_open("/p_consumer_alert" , O_CREAT | O_EXCL , 0644 , 0);
    if(sem_consumer ==  SEM_FAILED){
        err(EXIT_FAILURE , "semaphore failed");
    }



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

    if(sem_wait(sem_consumer) == -1){
        err(EXIT_FAILURE , "sem_consumer");
    }

    pid_t consumer_id = map_mem->consumer_id;


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
        if(fgets(buffer , sizeof(buffer) , stdin) == NULL){
            err(EXIT_FAILURE , "fgets");
        }

        snprintf(map_mem->buffer[map_mem->write_index] , 5 , "%s" , buffer);

        map_mem->write_index = (map_mem->write_index + 1)%SLOT;

        if(sem_post(sem_mutex) == -1){
            err(EXIT_FAILURE , "sempost");
        }

        if(sem_post(sem_full) == -1){
            err(EXIT_FAILURE , "sempost full");
        }

        printf("updated write_index = %d.\n",map_mem->write_index);

        printf("the message is %s.\n" , map_mem->buffer[i]);

        printf("read_index = %d.\n",map_mem->read_index);

    }
    
    int time_ = clock_gettime(CLOCK_REALTIME , &tp);
    if(time_ == -1){
        err(EXIT_FAILURE , "get time");
    }

    
    int timed_wait;
    tp.tv_sec += TIMED_OUT;
    while((timed_wait = sem_timedwait(sem_consumer , &tp)) != 0){
        if(errno == EINTR){
            printf("failed to perform the operation due to interrupt signal : %s (code : %d).\n",strerror(errno) , errno);
            continue;
        }
        else if(errno == ETIMEDOUT){
            printf("failed to perform the operation due to time out : %s (code : %d).\n",strerror(errno) , errno);
        }
        else{
            printf("failed to perform the operation due to some error : %s (code : %d).\n",strerror(errno) , errno);
        }
        break;
    }

    if(timed_wait == 0){
        printf("Operation Successfully Performed.\n");
    }

    int is_signal = kill(consumer_id , 0);

    if(is_signal == 0){
        printf("consumer exists and accessible");
    }
    else if(errno == ESRCH){
        printf("NO process with that PID exists : %s (error code : %d).\n" , strerror(errno) , errno);
    }
    else if(errno == EPERM){
        printf("Process exists but permission denied : %s (error code : %d).\n" , strerror(errno) , errno);
    }
    

    int mem_unmap = munmap(map_mem , sizeof(Shared_Mem));
    if(mem_unmap == -1){
        err(EXIT_FAILURE , "munmap");
    }

    int consumer_close = sem_close(sem_consumer);
    if(consumer_close == -1){
        err(EXIT_FAILURE , "sem_close");
    }

    int close_empty = sem_close(sem_empty);
    if(close_empty == -1){
        err(EXIT_FAILURE , "sem_close");
    }

    int full_close = sem_close(sem_full);
    if(full_close == -1){
        err(EXIT_FAILURE , "sem_Close");
    }

    int mutex_close = sem_close(sem_mutex);
    if(mutex_close == -1){
        err(EXIT_FAILURE  , "sem_close");
    }

    int consumer_unlink = sem_unlink("/p_consumer_alert");
    if(consumer_unlink == -1){
        err(EXIT_FAILURE , "consumer_unlink");
    }

    int empty_unlink = sem_unlink("/p_empty");
    if(empty_unlink == -1){
        err(EXIT_FAILURE , "SEMUNLINK");
    }

    int full_unlink = sem_unlink("/p_full");
    if(full_unlink == -1){
        err(EXIT_FAILURE , "SEM_UNLINK");
    }

    int mutex_unlink = sem_unlink("/p_mutex");
    if(mutex_unlink == -1){
        err(EXIT_FAILURE , "mutex Unlink");
    }

    int unlink = shm_unlink("/shm");
    if(unlink == -1){
        err(EXIT_FAILURE , "unlink shared memory");
    }

    int close_fd = close(shmfd);
    if(close_fd == -1){
        err(EXIT_FAILURE , "close");
    }

    int time_1 = clock_gettime(CLOCK_REALTIME , &tp);
    if(time_1 == -1){
        err(EXIT_FAILURE , "get time");
    }

    printf("the ending time is %ld.\n",tp.tv_sec);
    
    return 0;

}