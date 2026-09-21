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
    msgbuf msgp;

    msgbuf msgp2;

    key_t key = ftok("/tmp" , 'A');

    if(key == -1){
        perror("ftok failed");
        exit(EXIT_FAILURE);
    }

    int id = msgget(key , 0666);

    if(id == -1){
        perror("the msgget is not successfull");
        exit(EXIT_FAILURE);
    }
    else{
        printf("Queue created successfully and queue id is : %d\n",id);

        long msgtyp = 0;
        long msg2typ = -1;
         
        ssize_t msg_recv = msgrcv(id , &msgp , sizeof(msgp.mtext) , msgtyp , 0); //msgrcv(int msqid , void msgp[msgsz] , size_t msgsz , long msgtyp , int msgflg)
        
        if(msg_recv == -1){
          perror("message received failed");
          exit(EXIT_FAILURE);
        }
        else{
            msgp.mtext[msg_recv] = '\0';
             printf("the message type i asked for is %ld.\n",msgtyp);
            printf("the message type is %ld.\n",msgp.mtype);
            printf("Message received : %s" , msgp.mtext);
        }

        ssize_t msg_ = msgrcv(id , &msgp2 , sizeof(msgp2.mtext) , -1 , 0);

        if(msg_ == -1){
            perror("message 2 receive failed");
            exit(EXIT_FAILURE);
        }
        else{
            msgp2.mtext[msg_] = '\0';
            printf("the mesage type i requests : %ld\n", msg2typ);
            printf("message type the queue send : %ld\n",msgp2.mtype);
            printf("\n message received : %s",msgp2.mtext);
        }

    }

    return 0;
}