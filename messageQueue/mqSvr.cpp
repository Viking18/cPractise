//
// Created by 经纬 on 2018/7/25.
//

#include <sys/msg.h>
#include "message.h"
#include <stdio.h>
#include <unistd.h>

int main(){

    key_t   key;
    int     qid;
    message_t message;
    if( (key = ftok(filePath,'z'))<0)
    {
        printf("error ftok;");
        return -1;
    }

    if( (qid = msgget(key, IPC_CREAT))<0)
    {
        printf("error create msgQueue");
        return -1;
    }

    for(int i=0; i<5;i++)
    {
        msgrcv(qid, &message, 256, 777, 0);
        printf("PID:%d, type:%d, message: %s\n", getpid(), message.mtype, message.buffer);

        message.mtype = 888;
        sprintf(message.buffer, "I get your message.FROM %d\n", getpid());
        msgsnd(qid, &message, 256, 0);
    }
}