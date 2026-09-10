#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>

int main(){
    int arr[2];

    int flag = pipe(arr);

    if(flag == 0){
        printf("pipe created successfully.\n");
        printf("file descriptor 1 = %d and file descriptor 2 = %d.\n",arr[0],arr[1]);
        return 0;
    }
    else{
        printf("pipe creation failed.\n");
        return 1;
    }

}