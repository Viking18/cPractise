//
// Created by 经纬 on 2018/7/26.
//

#include <unistd.h>

#ifndef MYIPC_SHMQUEUE_H
#define MYIPC_SHMQUEUE_H


#define filePath "/Users/jingwei/.vimrc"
#define READABLE 'r'
#define WRITABLE 'w'

struct shmQueueControl
{
//    size_t      blockSize;
    char**       dataAddr;
    char**       maxDataAddr;
    int*        readIndex;
    int*        writeIndex;
    int*        maxLen;
    int*        blockSize;
};

class shmQueue
{
private:
    shmQueueControl * control;

public:
    int         init(size_t, void*);    // 服务器端用来创建
    int         get(void*);             // 服务端等用来获取已有的queue
    int         write(void *, size_t size);
    bool        read(void*);
//    bool        isFull();
    bool        isEmpty();

};


#endif //MYIPC_SHMQUEUE_H
