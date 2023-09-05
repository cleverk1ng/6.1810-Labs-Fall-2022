#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    char *args[MAXARG];
    for(int i = 1; i < argc; i++){
        args[i-1] = argv[i];
    }
    char buf[512], ch;
    args[argc-1] = buf;
    int index = 0;
    while(read(0, &ch, 1) == 1){
        if(ch != '\n'){
            buf[index++] = ch;
            continue;
        }
        buf[index] = '\0';
        index = 0;
        if(fork() == 0){
            exec(args[0], args);
        }
        wait(0);
    }
    exit(0);
}