#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"
#define MAX_BUF_SIZE 512

int readinput(char buf[MAX_BUF_SIZE])
{
    char *p;
    p = buf;
    while (read(0, p, sizeof(char)) != 0)
    {
        if (*p == '\n')
        {
            *p = 0;
            return 0;
        }
        p++;
        if (strlen(p) >= MAX_BUF_SIZE)
        {
            fprintf(2, "Input is too long!\n");
            exit(1);
        }
    }
    *p = 0;
    return 1; // reach the end of input
}

int main(int argc, char *argv[])
{
    char buf[MAX_BUF_SIZE];
    char *args[MAXARG];
    int i;
    for (i = 0; i < argc; i++)
    {
        args[i] = argv[i + 1];
    }
    int end;
    do
    {
        end = readinput(buf);
        if (fork())
        {
            wait(0);
        }
        else
        {
            args[argc - 1] = buf;
            exec(argv[1], args);
            fprintf(2, "exec: %s failed\n", argv[1]);
            exit(1);
        }
    } while (!end);

    exit(0);
}