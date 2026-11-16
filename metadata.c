#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<sys/types.h>
#include<errno.h>
#include<err.h>

int main(){
    pid_t pid = getpid();

    if(pid == -1){
        err(EXIT_FAILURE , "pid crash");
    }

    struct timespec tm;

    if(clock_gettime(CLOCK_REALTIME , &tm) == -1){
        err(EXIT_FAILURE , "time failed");
    }

    int fd = open("/tmp/producer-consumer.meta" , O_CREAT | O_RDWR | O_TRUNC , 0644);

    if(fd < 0){
        err(EXIT_FAILURE , "file descriptor crash");
    }

    char buffer[125];

    int len = snprintf(buffer , 125 , "session id : %ld_%d\n producer_pid : %d\n consumer_pid : 0. \n state = INITIALIZING" ,tm.tv_sec , pid , pid);
    ssize_t wr_flg = write(fd , buffer , len);
    if(wr_flg == -1){
      err(EXIT_FAILURE , "write failed");
    }

    lseek(fd , 0 , SEEK_SET);

    char rd_buf[120];
    ssize_t rd_flg = read(fd , rd_buf , len);
    if(rd_flg == -1){
        err(EXIT_FAILURE , "read failed");
    }
    rd_buf[rd_flg-1] = '\0';

    printf("%s.\n", rd_buf);

    return 0;
}