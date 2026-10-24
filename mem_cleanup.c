#include<semaphore.h>
#include<sys/mman.h>
#include<stdio.h>

int main(){
    sem_unlink("p_empty");
    sem_unlink("p_full");
    sem_unlink("p_mutex");
    shm_unlink("/shm");

    puts("IPC resources are unlinked");

    return 0;
}