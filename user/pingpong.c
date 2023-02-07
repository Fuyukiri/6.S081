#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
main()
{
    int p[2];
    pipe(p);

    if (fork() == 0)
    {
        char byte;
        read(p[0], &byte, 1);
        close(p[0]);
        printf("%d: received ping\n", getpid());
        write(p[1], &byte, 1);
        close(p[1]);
        exit(0);
    }
    else
    {
        char byte;
        write(p[1], &byte, 1);
        close(p[1]);
        wait(0);
        read(p[0], &byte, 1);
        close(p[0]);
        printf("%d: received pong\n", getpid());
        exit(0);
    }

    return 0;
}