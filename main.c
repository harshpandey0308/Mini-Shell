#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>

int main(){
    int arr[2];
    int arr0[2];

    int flag = pipe(arr);
    int flag0 = pipe(arr0);

    if(flag0 == 0){
        printf("pipe created successfully for child process.\n");
        printf("read fd = %d and write fd = %d.\n",arr0[0] , arr0[1]);
    }
    else{
        printf("pipe creation failed.\n");
        return 1;
    }

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
        printf("child process : %d and parent's pid = %d.\n",getpid() , getppid());
        printf("read fd = %d and write fd = %d.\n",arr[0],arr[1]);
        close(arr[1]);
        close(arr0[0]);

        int s_flag = write(arr0[1] , "R" , 2);
        if(s_flag > -1){
            printf("number of bytes written : %d.\n",s_flag);
        }
        else{
            printf("write is unsuccessfull.\n");
        }
        //keep(arr[0]);
        int byte = read(arr[0] , buffer , 11);
        
        buffer[byte] = '\0';
        printf("received message : %s\n",buffer);
        
        close(arr0[1]);
        close(arr[0]);
    }
    else{
        printf("\nPARENT PROCESS:\n");
        printf("parent process = %d and grandparent's pid = %d.\n",getpid(),getppid());
        printf("read fd = %d and write fd = %d.\n",arr[0],arr[1]);
        close(arr[0]);
        close(arr0[1]);
        //keep(arr[1]);

        char store[21];
        read(arr0[0] , store , 2);
        char *name = malloc(11*sizeof(char));
        printf("enter the message : \n");
        fgets(name , 11 , stdin);

        int n = strlen(name);

        write(arr[1] , name , n);
        //printf("sent message : 'Hello child'\n");
        close(arr0[0]);
        close(arr[1]);
    }



    
    return 0;
}