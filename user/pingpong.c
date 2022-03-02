#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

char buf1[8];
char buf2[8];

int
main(int argc, char *argv[])
{
    int p[2];
    pipe(p);
    char buf[64];

    if (fork() == 0) {
	// parent process
	write(p[1], "ping", strlen("ping"));
	read(p[0], buf, 4);
	printf("%d: received %s\n", getpid(), buf);
    } else {
	// child process
	read(p[0], buf, 4);
	printf("%d: received %s\n", getpid(), buf);
	write(p[1], "pong", strlen("pong"));
    }

    wait(p);
    exit(0);
}

