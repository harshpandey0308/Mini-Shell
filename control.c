#include<stdio.h>
#include<sys/types.h>
#include<sys/msg.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdbool.h>

int main(){
    struct msqid_ds buffer;

    key_t key = ftok("/tmp" , 'A');

    if(key == -1){
        perror("key failed");
        exit(EXIT_FAILURE);
    }

    int msqid = msgget(key , IPC_CREAT | 0666);

    if(msqid == -1){
        perror("queue id failed");
        exit(EXIT_FAILURE);
    }

    int success = msgctl(msqid , IPC_STAT , &buffer);

    if(success == -1){
        perror("message information failed");
        exit(EXIT_FAILURE);
    }

    printf("Number of Bytes used in queue : %lu.\n" , buffer.msg_cbytes);
    printf("Number of messages in queue : %lu.\n" , buffer.msg_qnum);
    printf("Maximum number of bytes in the queue : %lu.\n" , buffer.msg_qbytes);

    int destroy_queue = 0;

    printf("want to destroy queue : 1 for YES | 0 for NO : ");
    scanf("%d" , &destroy_queue);

    if(destroy_queue == 1){
        int destroy = msgctl(msqid , IPC_RMID , NULL);

        if(destroy == -1){
            perror("queue destruction failed");
            exit(EXIT_FAILURE);
        }

        printf("destroy successfully");
        exit(EXIT_SUCCESS);
    }

    return 0;

}