#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/msg.h>

#define MAX_SIZE 100

typedef enum{
    NORMAL = 1,
    STATUS = 2,
    URGENT = 3,
    CRITICAL = 4,
    VERY_CRITICAL  = 5
}MSG_TYP;

typedef struct{
    long mtype;
    char mtext[MAX_SIZE];
}msgbuf;

int main(){
    key_t key = ftok("/tmp" , 'A');

    if(key == -1){
        perror("queue creation failed");
        exit(EXIT_FAILURE);
    }

    int qid = msgget(key , IPC_CREAT | 0666);

    if(qid == -1){
        perror("the queue id failed");
        exit(EXIT_FAILURE);
    }

    msgbuf msgp1;
    size_t msgsz1;

    msgbuf msgp2;
    size_t msgsz2;

    msgbuf msgp3;
    size_t msgsz3;

    msgp1.mtype = 1;
    msgp2.mtype = 2;
    msgp3.mtype = 3;

    strcpy(msgp1.mtext , "hello");
    strcpy(msgp2.mtext , ", I am Harsh");
    strcpy(msgp3.mtext , ", your personal health care companion");

    msgsz1 = strlen(msgp1.mtext);
    msgsz2 = strlen(msgp2.mtext);
    msgsz3 = strlen(msgp3.mtext);

    ssize_t snd1 = msgsnd(qid , &msgp1 , msgsz1 , 0);
    if(snd1 == -1){
        perror("sending message 1 failed");
        exit(EXIT_FAILURE);
    }else{
        printf("the message of type %ld is send.\n",msgp1.mtype);
    }

    ssize_t snd2 = msgsnd(qid , &msgp2 , msgsz2 , 0);
    if(snd2 == -1){
        perror("sending message 2 failed");
        exit(EXIT_FAILURE);
    }
    else{
        printf("the message of type %ld is send.\n",msgp2.mtype);
    }

    ssize_t snd3 = msgsnd(qid , &msgp3 , msgsz3 , 0);
    if(snd3 == -1){
        perror("sending message 3 failed");
        exit(EXIT_FAILURE);
    }
    else{
        printf("the message of type %ld is send.\n",msgp3.mtype);
    }

    return 0;
}