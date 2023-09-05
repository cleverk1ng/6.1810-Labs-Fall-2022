#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int p[2];
    pipe(p);
    if(fork() == 0){
        read(p[0], 0, 1);
        printf("%d: received ping\n", getpid());
        write(p[1], "B", 1);
        close(p[0]);
        close(p[1]);
    }else{
        write(p[1], "A", 1);
        read(p[0], 0, 1);
        printf("%d: received pong\n", getpid());
        close(p[0]);
        close(p[1]);
        wait(0);
    }
    exit(0);
}