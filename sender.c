#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/msg.h>

#define MAX_SIZE 100

typedef struct{
    long mtype;
    char mtext[MAX_SIZE];
}msgbuf;

int main(){
    size_t msgsz;

    msgbuf msgp;

    msgp.mtype = 1;
    strcpy(msgp.mtext , "hello , mq");

    msgsz = strlen(msgp.mtext);

    key_t key = ftok("/tmp" , 'A');

    if(key == -1){
        perror("ftok failed , no such process exist");
        exit(EXIT_FAILURE);
    }
    
    int msgqid = msgget(key , IPC_CREAT | 0666);

    if(msgqid == -1){
        perror("msgget() failed ");
        exit(EXIT_FAILURE);
    }
    else{
        printf("the queue found successfully and the qid is %d.\n",msgqid);
       
        int snd_flg = msgsnd(msgqid , &msgp , msgsz , 0);

        if(snd_flg == 0){
            printf("the message send successfully.\n");
        }
    }

    return 0;
}