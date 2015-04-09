/******************************************************************************
* FILE: hello.c
* DESCRIPTION:
*   A "hello world" Pthreads program.  Demonstrates thread creation and
*   termination.
* AUTHOR: Blaise Barney
* LAST REVISED: 08/09/11
******************************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int compare_and_swap(int* reg, int oldval, int newval)
{

  int old_reg_val = *reg;
  if (old_reg_val == oldval)
     *reg = newval;

  return old_reg_val;
}

void Proc1(int *lock)
{   
   do {
      while(compare_and_swap(lock, 0, 1) != 0);
      //Critical Section
      *lock = 1;
   }while(1);
}

void *Proc2(int *lock)
{   
  do {
      while(compare_and_swap(lock, 1, 0) != 1);
      //Critical Section
      *lock = 0;
  }while(1);
 
}


int main(int argc, char *argv[])
{
  pthread_t threads[2];
  int p1, p2, lock;
   
  p1 = pthread_create(&threads[0], NULL, Proc1, &lock);
  if (p1){
    printf("ERROR; return code from pthread_create() is %d\n", p1);
    exit(-1);
  }

  p2 = pthread_create(&threads[1], NULL, Proc2, &lock);
  if (p2){
    printf("ERROR; return code from pthread_create() is %d\n", p1);
    exit(-1);
  }


   /* Last thing that main() should do */
   pthread_exit(NULL);
}
