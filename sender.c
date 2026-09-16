#include<stdio.h>
#include<unistd.h>
#include<sys/msg.h>

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

    return 0;
}