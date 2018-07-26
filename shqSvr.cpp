//
// Created by 经纬 on 2018/7/26.
//
#include <sys/ipc.h>
#include "shmQueue.h"
#include <stdio.h>
#include <sys/shm.h>
#include <zconf.h>
#include <fcntl.h>
#include <errno.h>

int main(){
    key_t key;
    int     shmid;
    void*   shmp;

    int fd;
    if((fd = open(filePath,O_RDONLY))<=0)
        printf("open file error!\n");
    int err;
    if( (key = ftok(filePath, 'z'))<0)
    {
        printf("ftok failed!");
        return -1;
    }

    if((shmid= shmget(key, 10000, 0666|IPC_CREAT)) == -1)
    {
        printf("shm error. errno:%d", errno);
        return -1;
    }

     if((shmp = shmat(shmid, 0, 0)) == (void *)-1)
     {
         printf(" shm error. errno:%d", errno);
         return -1;
     }
    shmQueue* queue = new shmQueue;
    queue->init(10000, shmp);
//    queue->get(shmp);

    char    buffer[256];

    for(;;)
    {
        if( queue->read(buffer))
            printf("server:%d, get Message:%s\n", getpid(), buffer);
    }

    return 0;
}