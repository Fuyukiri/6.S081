/*
Code adopted from https://www.cs.dartmouth.edu/~doug/sieve/sieve.pdf
@M. Douglas McIlroy
*/

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// p is the previous prime
void
cull(int p)
{
    int n;
    for (;;)
    {
        // read from previous child
        if (read(0, &n, sizeof(n)) == 0)
        {
            exit(0);
        }
        // We write next prime and filter all Composite number based on previous prime
        // e.g. read 4 from previous child, 4 % 2 == 0
        if (n % p != 0)
            write(1, &n, sizeof(n));
    }
}

/* connect stdin (k=0) or stdout (k=1) to pipe pd */
void
redirect(int k, int pd[2])
{
    close(k);
    dup(pd[k]);
    close(pd[0]);
    close(pd[1]);
}

void
sink()
{
    int pd[2];
    int p; /* a prime */
    for (;;)
    {
        // block
        if (read(0, &p, sizeof(p)) == 0)
        {
            exit(0);
        }
        printf("prime %d\n", p);
        pipe(pd);
        if (fork())
        {
            // close the old pipe, wire to new pipe and get number from new child
            redirect(0, pd);
            continue;
        }
        else
        {
            // Child Process only created when we get a prime number
            // currently, new child still wire to previous child process
            // so read from there, and write to parent process.
            redirect(1, pd);
            cull(p);
        }
    }
}

int
main()
{
    int pd[2]; /* pipe descriptors */
    pipe(pd);
    if (fork())
    { /* parent process */
        redirect(0, pd);
        sink();
    }
    else
    {
        /* child process */
        redirect(1, pd);
        int n;
        for (n = 2; n < 35; n++)
            write(1, &n, sizeof(n));
    }
    exit(0);
}