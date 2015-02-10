/*=============================================================================
 |    File Name:  example.c [in which directory on what computer]
 |
 |       Author:  Tarcisio Bruno Carneiro Oliveira
 |     Language:  ANSI C (tested using gcc on turing [might list version infor])
 |   To Compile:  gcc -Wall example.c
 |
 |        Class:  CSCE3613 [needed only for school work]
 |      Project:  [if it is part of larger software]
 |   Assumption:  [any prerequisite or precondition that must be met]
 | Date Created:  1-14-2015
 |     Modified:  1-14-2015
 |
 +-----------------------------------------------------------------------------
 |
 |  Description:  A simple c program that handling command line arguments and
 |      illustrates the use of function, array, and c-style string.
 |
 |	  Input:  No input (either from the keyboard or from a file) is
 |		required by this program.
 |
 |	 Output:  Hello, World!
 |		  The value of argc (number of command line arguments) is [value]
 |                argv[] array element one per line
 |
 |    Algorithm:  A simple for loop in the display function.
 |		[should provide a high level algorithm or reference to it]
 |
 |  Required Features Not Included:  The program adheres to all
 |		requirements stated in the program assignment, and all
 |		required features are included.
 |
 |   Known Bugs:  There are no known bugs remaining in this program.
 |
 +===========================================================================*/

#include <stdio.h>

void display(int, char **);

int main(int argc, char *argv[])
{
	printf("Hello, World!\n");
	printf("The value of argc (number of command line arguments) is %d\n", argc);
        display(argc, argv);
	return 0;
}

        /*-----------------------display --------------------------------------
         |  Function display
	 |
	 |  Purpose:  output each c-style string stored in an array.
	 |
	 |  Parameters:
	 |	count (IN) - The number of elements in the array.
	 |   argument (IN) - The array.
	 |
	 |  Returns:  Nothing.  (This is a void function.)
	 |
	 *-------------------------------------------------------------------*/


void display(int size, char *argument[]){
    int i;
    for (i=0; i < size; i++){
            printf("argv[%d] = %s\n",i,argument[i]);
    }
}