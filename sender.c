#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
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
    size_t msgsz1;

    size_t msgsz2;

    msgbuf msgp1;

    msgbuf msgp2;

    msgp1.mtype = 2;
    strcpy(msgp1.mtext , "puta madares");

    msgsz1 = strlen(msgp1.mtext);

    msgp2.mtype = 3;

    strcpy(msgp2.mtext , "i am harsh");

    msgsz2 = strlen(msgp2.mtext);

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
       
        int snd_flg = msgsnd(msgqid , &msgp1 , msgsz1 , 0);

        int snd2 = msgsnd(msgqid , &msgp2 , msgsz2 , 0);

        if(snd_flg == -1){
            perror("message send failed");
            exit(EXIT_FAILURE);
        }

        if(snd2 == -1){
            perror("message 2 send failed");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}