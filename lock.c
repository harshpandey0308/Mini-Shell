#include<stdio.h>
#include<stdlib.h>
#include<err.h>
#include<errno.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/file.h>
#include<sys/types.h>
#include<semaphore.h>

int main(){
    int fd = open("/tmp/producer_consumer.lock" , O_CREAT | O_RDWR , 0666);

    if(fd == -1){
        err(EXIT_FAILURE , "file descriptor");
    }

    while(flock(fd , LOCK_EX) == -1){
        if(errno == EINTR){
           continue;
        }

        perror("flock");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("lock acquired successfully\n");

    sleep(20);

    close(fd);

    return 0;

}