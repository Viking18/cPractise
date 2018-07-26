//
// Created by 经纬 on 2018/7/25.
//

#include <unistd.h>
#include "message.h"
#include <sys/msg.h>
#include <cstdio>

int main()
{
    key_t key;
    int     qid;
    message_t message;

    if( (key = ftok(filePath, 'z')) < 0)
    {
        printf("ftok failed");
        return -1;
    }

    if( (qid = msgget(key, IPC_EXCL)) < 0)
    {
        printf("get qid failed");
        return -1;
    }

    message.mtype = 777;
    int n = sprintf(message.buffer,"Hello. This is client!");
    msgsnd(qid, &message, n-1, 0);

    msgrcv(qid, message.buffer, 256, 888, 0);
    printf("message:%s\n", message.buffer);
}