#include "kernel/types.h"
#include "user/user.h"

void primes(int p[2]){
    int first, buf;
    if(read(p[0], &first, 4) == 0){
        return;
    }
    printf("prime %d\n", first);
    int pp[2];
    pipe(pp);
    if(fork() > 0){
        close(pp[0]);
        while(read(p[0], &buf, 4) == 4){
            if(buf % first != 0){
                write(pp[1], &buf, 4);
            }
        }
        close(pp[1]);
        wait(0);
    }else{
        close(pp[1]);
        primes(pp);
        close(pp[0]);
    }
}

int main(int argc, char *argv[])
{
    int p[2];
    pipe(p);
    if(fork() > 0){
        close(p[0]);
        for(int i = 2; i <= 35; ++i){
            write(p[1], &i, 4);
        }
        close(p[1]);
        wait(0);
    }else{
        close(p[1]);
        primes(p);
        close(p[0]);
    }
    exit(0);
}
