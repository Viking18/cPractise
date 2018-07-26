//
// Created by 经纬 on 2018/7/25.
//

#ifndef MYIPC_MESSAGE_H
#define MYIPC_MESSAGE_H

#define filePath "/etc/wfs"

struct message_t{
    long mtype;
    char buffer[256];
};

#endif //MYIPC_MESSAGE_H
