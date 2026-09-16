#include<stdio.h>
#include<unistd.h>
#include<sys/msg.h>

int main(){
    key_t key = ftok("/tmp" , 'A');

    int id = msgget(key , 0666);

    if(id == -1){
        printf("Queue creation is unsuccessfull.\n");
        return 1;
    }
    else{
        printf("Queue created successfully and queue id is : %d\n",id);
    }
}