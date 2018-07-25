#include <iostream>
#include <unistd.h>
#include <string>
#include <fcntl.h>

using namespace std;
int main(int argc, char ** argv) {

//    if(argc!=2){
//        printf("usage:  main<filePath>");
//        return -1;
//    }

    int fd[2];
    if( pipe(fd) != 0)
    {
        printf("create pip error");
        return -1;
    }

//    char * filePath = argv[1];
    int pid;
    if( (pid = fork()) > 0) // parent process
    {
        printf("pid:%d, parent process \n", getpid());
        close(fd[0]);
        char buffer[30] = "there is something.\0";
        printf("parent proces, write:%s\n", buffer);
        write( fd[1], buffer, 30);
    } else if( pid == 0)
    {
        printf("pid:%d, child process \n", getpid());
        close(fd[1]);
        char buffer[30];
        read( fd[0], buffer, 30);
        printf("child process:%s\n",buffer);
    }

    return 0;
}