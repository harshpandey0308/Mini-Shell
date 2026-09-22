#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/msg.h>

#define MAX_SIZE 100

typedef enum{
    NORMAL = 1,
    STATUS = 2,
    URGENT = 3,
    CRITICAL = 4,
    VERY_CRITICAL = 5
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
        perror("the qid failed");
        exit(EXIT_FAILURE);
    }

    msgbuf msgp1;
    size_t msgsz1 = 0;

    msgbuf msgp2;
    size_t msgsz2 = 0;

    msgbuf msgp3; 
    size_t msgsz3 = 0;

    msgp1.mtype = 3;
    msgp2.mtype = 2;
    msgp3.mtype = 1;

    long msgtyp1 = 3;
    long msgtyp2 = 1;
    long msgtyp3 = 2;

    ssize_t rcv1 = msgrcv(qid , &msgp1 , sizeof(msgp1.mtext) , msgtyp1 , 0);
    
    if(rcv1 == -1){
        perror("message receiver failed");
        exit(EXIT_FAILURE);
    }
    else{
        msgp1.mtext[rcv1] = '\0';
        printf("the message received : %s.\n",msgp1.mtext);
        printf("msgtype received : %ld.\n" , msgp1.mtype);
        printf("msgtype expected : %ld.\n" , msgtyp1);
    }

    ssize_t rcv2 = msgrcv(qid , &msgp2 , sizeof(msgp2.mtext) , msgtyp2 , 0);
    if(rcv2 == -1){
        perror("message receiver 2 failed");
        exit(EXIT_FAILURE);
    }
    else{
        msgp2.mtext[rcv2] = '\0';
        printf("the message received : %s.\n",msgp2.mtext);
        printf("msgtype received : %ld.\n" , msgp2.mtype);
        printf("msgtype expected : %ld.\n" , msgtyp2);
    }

    ssize_t rcv3 = msgrcv(qid , &msgp3 , sizeof(msgp3.mtext) , msgtyp3 , 0);
    if(rcv3 == -1){
        perror("message receiver 3 failed");
        exit(EXIT_FAILURE);
    }
    else{
        msgp3.mtext[rcv3] = '\0';
        printf("the message received : %s.\n",msgp3.mtext);
        printf("msgtype received : %ld.\n" , msgp3.mtype);
        printf("msgtype expected : %ld.\n" , msgtyp3);
    }

    return 0;
}