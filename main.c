#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>

int main(){
    int arr[2];

    int flag = pipe(arr);

    if(flag == 0){
        printf("pipe created successfully.\n");
        printf("file descriptor 1 = %d and file descriptor 2 = %d.\n",arr[0],arr[1]);
    }
    else{
        printf("pipe creation failed.\n");
        return 1;
    }

    pid_t p_flag = fork();

    if(p_flag<0){
        perror("process creation failed.\n");
        exit(-1);
    }
    else if(p_flag == 0){
        char buffer[12];
        printf("CHILD PROCESS:\n");
        printf("child process : %ld and parent's pid = %ld.\n",getpid() , getppid());
        printf("read fd = %d and write fd = %d.\n",arr[0],arr[1]);
        close(arr[1]);
        //keep(arr[0]);
        read(arr[0] , buffer , 11);
        printf("received message : %s\n",buffer);
        close(arr[0]);
    }
    else{
        printf("\nPARENT PROCESS:\n");
        printf("parent process = %ld and grandparent's pid = %ld.\n",getpid(),getppid());
        printf("read fd = %d and write fd = %d.\n",arr[0],arr[1]);
        close(arr[0]);
        //keep(arr[1]);

        //write(arr[1] , "Hello child" , 11);
        //printf("sent message : 'Hello child'\n");
        //close(arr[1]);
    }



    
    return 0;
}