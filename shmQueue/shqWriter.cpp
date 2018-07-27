//
// Created by 经纬 on 2018/7/26.
//

#include <sys/ipc.h>
#include "shmQueue.h"
#include <stdio.h>
#include <sys/shm.h>
#include <zconf.h>
#include <errno.h>
#include "myThreadPool.h"

void  thread_do(void * args)
{
    shmQueue* shmQueue1 = (shmQueue*) args;
    char buffer[256];
    for(int i=0;i<5;i++) {
        sprintf(buffer, "thread:%d, message %d.",(long) pthread_self() %177, i);
        shmQueue1->write(buffer, sizeof(buffer));
    }
}

int main(){
    key_t key;
    int     shmid;
    void*   shmp;

    if( (key = ftok(filePath, 'z'))<0)
    {
        printf("ftok failed!");
        return -1;
    }

    if((shmid= shmget(key, 0, 0666)) == -1)
    {
        printf("shm error. errno:%d", errno);
        return -1;
    }

    if((shmp = shmat(shmid, 0, 0)) ==(void *) -1)
    {
        printf("shm error. errno:%d", errno);
        return -1;
    }
    shmQueue* queue = new shmQueue;
    queue->get(shmp);

    //用线城池去多写
    MyWorkQueue* queue1 = new MyWorkQueue(10);
    time_t time1 = 5;
    MyThreadPool* pool = new MyThreadPool(5,5,&time1,queue1);
    pool->init();
    for(int i=0;i<5;i++)
    {
        pool->submit(thread_do, queue);
    }

    for(int i=0;i<1000;i++);

//    char    buffer[256];
//    for(int i=0;i<10;i++)
//    {
//        sprintf(buffer, "client:%d, is sending message %d", getpid(), i);
//        queue->write(buffer, 256);
//    }

    return 0;
}
