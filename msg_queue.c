#include<stdio.h>
#include<sys/msg.h>
#include<unistd.h>
#include<stdlib.h>

int main(){

    key_t key = ftok("/tmp" , 'A');

    int id = msgget(key , IPC_CREAT | 0666);

    if(id == -1){
        printf("Queue creation failed.\n");
        return 1;
    }
    else{
        printf("The Queue created successfully and the queue id is %d.\n",id);
    }

    key_t key2 = ftok("/tmp" , 'A');

    int qid = msgget(key2 , IPC_CREAT | 0400);

    if(qid == -1){
        printf("Queue creation failed.\n");
        return 1;
    }
    else{
        printf("The Queue created successfully and the queue id is %d.\n",qid);
    }

    return 0;
}
