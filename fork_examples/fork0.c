/* $begin forkprob0 */
/* modified from computer system book site*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
void unix_error(char *msg) /* unix-style error */
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(0);
}
pid_t Fork(void)
{
    pid_t pid;

    if ((pid = fork()) < 0)
	   unix_error("Fork error");
    return pid;
}
int main()
{
    int x = 1;

    if (Fork() == 0)
	   printf("printf1: x=%d\n", ++x);
    printf("printf2: x=%d\n", --x);
    exit(0);
}
/* $end forkprob0 */


