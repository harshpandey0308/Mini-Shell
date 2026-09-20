#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<sys/msg.h>

#define MAX_SIZE 100

typedef struct{
    long mtype;
    char mtext[MAX_SIZE];
}msgbuf;

int main(){
    msgbuf msgp;

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
         
        size_t msg_recv = msgrcv(id , &msgp , sizeof(msgp.mtext) , 0 , 0);

        if(msg_recv == -1){
          perror("message received failed");
          exit(EXIT_FAILURE);
        }
        else{
            printf("Message received : %s" , msgp.mtext);
        }

    }

    return 0;
}