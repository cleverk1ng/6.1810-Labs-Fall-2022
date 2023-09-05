#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char *fmtname(char *path)
{
    char *p;
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;
    return p;
}

void find(char *path, char *filename)
{
    struct dirent de;
    struct stat st;
    int fd;
    if ((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }
    if (fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }
    switch (st.type){
    case T_FILE:
        if (strcmp(fmtname(path), filename) == 0){
            printf("%s\n", path);
        }
        break;
    case T_DIR:
        char buf[512];
        memmove(buf, path, strlen(path));
        buf[strlen(path)] = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de)){
            if(de.inum == 0){
                continue;
            }
            memmove(buf + strlen(path) + 1, de.name, strlen(de.name));
            buf[strlen(path) + 1 + strlen(de.name)] = '\0';
            if(strcmp(".", de.name) == 0 || strcmp("..", de.name) == 0){
                continue;
            }
            find(buf, filename);
        }
        break;
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    find(argv[1], argv[2]);
    exit(0);
}

 