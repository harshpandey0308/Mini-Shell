#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<sys/types.h>
#include<string.h>
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
    size_t buf_len = 125;

    int len = snprintf(buffer , buf_len , "session id : %ld_%d\n producer_pid : %d\n consumer_pid : 0. \n state = INITIALIZING" ,tm.tv_sec , pid , pid);

    size_t total_written = 0;
    
    while(total_written < len){
        size_t remaining = len - total_written;
        ssize_t wr_flg = write(fd , buffer+total_written , remaining);

        if(wr_flg == -1){
            if(errno == EINTR){
                printf("failure occures due to the interrupt signal , error : %s (code : %d)", strerror(errno) , errno);
                continue;
            }
            else{
                printf("failure occurs due to some error signal.\n");
            }
        }
        else{
            printf("message write successfully.\n");
            total_written += wr_flg;
            buf_len = remaining;
        }
    }
    
    lseek(fd , 0 , SEEK_SET);

    char rd_buf[120];
    ssize_t rd_flg = read(fd , rd_buf , len);
    if(rd_flg == -1){
        err(EXIT_FAILURE , "read failed");
    }
    rd_buf[rd_flg] = '\0';

    printf("%s.\n", rd_buf);

    return 0;
}