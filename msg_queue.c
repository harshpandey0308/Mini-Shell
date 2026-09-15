#include<stdio.h>
#include<sys/msg.h>
#include<unistd.h>
#include<stdlib.h>

int main(){

    int id = msgget(IPC_PRIVATE , 0666);

    if(id == -1){
        printf("Queue creation failed.\n");
        return 1;
    }
    else{
        printf("The Queue created successfully and the queue id is %d.\n",id);
    }

    return 0;
}