//
// Created by 经纬 on 2018/7/26.
//

#include <sys/shm.h>
#include "shmQueue.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int shmQueue::init(size_t size, void * p)
{
    control = new shmQueueControl();

    control->dataAddr = (char**) p;
    control->maxDataAddr = control->dataAddr + 1;
    control->readIndex =(int *) (control->maxDataAddr + 1);
    control->writeIndex = control->readIndex + 1;
    control->maxLen = control->writeIndex + 1;
    control->blockSize = control->maxLen + 1;


    *control->blockSize = 256;
    *control->maxLen = (size -(sizeof(shmQueueControl)))/(*control->blockSize); //256 B as a block
    *control->dataAddr =(char*) (control->blockSize + 1);
    *control->maxDataAddr = *control->dataAddr + (*control->blockSize) * (*control->maxLen-1)/ sizeof(control->dataAddr);
    *control->readIndex = 0;
    *control->writeIndex = 0;

    return 0;
}

int shmQueue::get(void * p)
{
    control = new shmQueueControl();

    control->dataAddr = (char**) p;
    control->maxDataAddr = control->dataAddr + 1;
    control->readIndex =(int *) (control->maxDataAddr + 1);
    control->writeIndex = control->readIndex + 1;
    control->maxLen = control->writeIndex + 1;
    control->blockSize = control->maxLen + 1;


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
    int oldWriteIndex, newWriteIndex;
    do {
        oldWriteIndex = *control->writeIndex;
        newWriteIndex = oldWriteIndex + writeBlock;
    }while( !__sync_bool_compare_and_swap(control->writeIndex, oldWriteIndex, newWriteIndex));
    char * writeP =(char *) (control->blockSize + 1 + *control->blockSize * oldWriteIndex / sizeof(control->blockSize));

    //change block head status
    * (writeP+1) = 1; //总blocks
    * (writeP+2) = 1; //当前block编号
    memcpy(writeP+3, p, size);
    * writeP = READABLE;

    //change status
//    if(*control->writeIndex + writeBlock >= *control->maxLen)
//    {
//        *control->writeIndex = (*control->writeIndex + writeBlock) % *control->maxLen;
//    } else {
//        *control->writeIndex += writeBlock;
//    }

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
    char * readP = (char *) (control->blockSize + 1 + *control->blockSize * *control->readIndex / sizeof(control->blockSize));

    if(*control->readIndex + *(readP+1) >= *control->maxLen)
    {
        *control->readIndex = (*control->readIndex + *(readP+1)) % *control->maxLen;
    }
    else {
        *control->readIndex += *(readP + 1);
    }

    if(!checkBlock(readP))
        usleep( 5000 );  //等待5ms

    if(!checkBlock(readP))
        return false; //仍然未就绪，说明该块坏了

    //确定能读取的块 check block head
    while(*readP == READABLE)
    {
        memcpy(p, readP, *control->blockSize);
        *readP = WRITABLE;
        (readP += *control->blockSize/ sizeof(readP))>*control->maxDataAddr? (readP=(char*)(control->blockSize+1)) : 0;
    }

    return true;
}

bool shmQueue::isEmpty()
{
    return *control->readIndex == *control->writeIndex;
}

bool shmQueue::checkBlock(void *p) {
    char * pchar = (char *) p;
    return *(pchar+1) &&
           *(pchar+2) &&
           *(pchar+2) <= *(pchar+1) &&
           *(pchar) == READABLE;
}

