#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/file.h>
#include<err.h>
#include<errno.h>

int main(){
    int fd = open("/tmp/producer_consumer.lock" , O_RDWR);
    if(fd == -1){
        err(EXIT_FAILURE , "file descriptor");
    }

    while(flock(fd , LOCK_EX) == -1){
        if(errno == EINTR){
            continue;
        }

        perror("flock id");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("consumer acquires lock successfully");

    close(fd);

    return 0;

}
