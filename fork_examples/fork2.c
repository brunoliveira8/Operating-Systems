/* without error check fork2.c */
#include <stdlib.h>  // for exit function
#include <stdio.h>   // for printf function
#include <unistd.h>  // for fork function

void doit() 
{
    fork();
    fork();
    printf("hello\n");
    return;
}

int main() 
{
    doit();
    printf("hello\n");
    exit(0);
}
/* $end forkprob4 */


