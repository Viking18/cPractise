//
// Created by 经纬 on 2018/7/26.
//

#include <sys/shm.h>
#include "shmQueue.h"
#include <stdio.h>
#include <string.h>


int shmQueue::init(size_t size, void * p)
{
    control = new shmQueueControl();

    control->dataAddr = (char**) p;
    control->maxDataAddr = control->dataAddr + sizeof(void**);
    control->readIndex =(int *) (control->maxDataAddr + sizeof(void**));
    control->writeIndex = control->readIndex + sizeof(int *);
    control->maxLen = control->writeIndex + sizeof(int *);
    control->blockSize = control->maxLen + sizeof(int *);


    *control->blockSize = 256;
    *control->maxLen = (size -(sizeof(shmQueueControl)))/(*control->blockSize); //256 B as a block
    *control->dataAddr =(char*) (control->blockSize + sizeof(int *));
    *control->maxDataAddr = *control->dataAddr + (*control->blockSize) * (*control->maxLen-1);
    *control->readIndex = 0;
    *control->writeIndex = 0;

    return 0;
}

int shmQueue::get(void * p)
{
    control = new shmQueueControl();

    control->dataAddr = (char**) p;
    control->maxDataAddr = control->dataAddr + sizeof(void**);
    control->readIndex =(int *) (control->maxDataAddr + sizeof(void**));
    control->writeIndex = control->readIndex + sizeof(int *);
    control->maxLen = control->writeIndex + sizeof(int *);
    control->blockSize = control->maxLen + sizeof(int *);


    return 0;
}

int shmQueue::write(void *p, size_t size)  // only write size<blocksize
{

    int     writeBlock = 1;
    if((*control->writeIndex + writeBlock)%(*control->maxLen) == *control->readIndex)
    {
        printf("queue is full\n");
        return -1;
    }

    //start writing
//    int     writeBlock = size/control->blockSize + 1;
//    char *  writeP =(char *)  *control->dataAddr + *control->blockSize * *control->writeIndex;
    char * writeP =(char *) (control->blockSize + sizeof(int*) + *control->blockSize * *control->writeIndex / sizeof(control->blockSize));

    //change block head status
    * (writeP+1) = 1;
    * (writeP+2) = 1;
    memcpy(writeP+3, p, size);
    * writeP = READABLE;

    //change status
    if(*control->writeIndex + writeBlock >= *control->maxLen)
    {
        *control->writeIndex = (*control->writeIndex + writeBlock) % *control->maxLen;
    } else {
        *control->writeIndex += writeBlock;
    }

    return 0;
}

bool shmQueue::read(void * p)
{
    if(isEmpty())
    {
//        printf("queue is empty\n");
        return false;
    }

    //get read address
//    char * readP =(char *) *control->dataAddr + *control->blockSize * *control->readIndex;
    char * readP = (char *) (control->blockSize + sizeof(int *) + *control->blockSize * *control->readIndex / sizeof(control->blockSize));

    if(*control->readIndex + *(readP+1) >= *control->maxLen)
    {
        *control->readIndex = (*control->readIndex + *(readP+1)) % *control->maxLen;
    }
    else {
        *control->readIndex += *(readP + 1);
    }

    //确定能读取的块 check block head
    while(*readP == READABLE)
    {
        memcpy(p, readP, *control->blockSize);
        *readP = WRITABLE;
        (readP += *control->blockSize)>*control->maxDataAddr? (readP=(char*)*control->dataAddr) : 0;
    }

    return true;
}

bool shmQueue::isEmpty()
{
    return *control->readIndex == *control->writeIndex;
}

