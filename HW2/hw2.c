/*=============================================================================
 |    File Name:  hw2.c [in which directory on what computer]
 |
 |       Author:  Tarcisio Bruno Carneiro Oliveira
 |     Language:  ANSI C (tested using gcc on turing [might list version infor])
 |   To Compile:  gcc -Wall hw1.c
 |
 |        Class:  CSCE3613 
 |      Project:  Simple Shell Part II
 |   Assumption:  [any prerequisite or precondition that must be met]
 | Date Created:  1-23-2015
 |     Modified:  2-4-2015
 |
 +-----------------------------------------------------------------------------
 |
 |  Description:  This program implements a simple Shell.

 |	  Input:  No input (either from the keyboard or from a file) is
 |		required by this program.
 |
 |	 Output:  Depends on what is typed.
 |
 |    Algorithm:  The code is divided in independent functions that processing
 |			the string typed by the user. The code uses pratically contional and
 |			loop commands. The libraries string.h and stdlib.h are included to help.			 
 |
 |  Required Features Not Included:  The program adheres to all
 |		requirements stated in the program assignment, and all
 |		required features are included.
 |
 |   Known Bugs:  There are no known bugs remaining in this program.
 |
 +===========================================================================*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void display(int, char **);

char* getInput(char *argument[]){

	return argument[1];

}	

int hasPipe(char *line){

	if (strpbrk("|", line) != NULL) return 1;

	else return 0;

}

// 0 - No redirection, 1 - Only Input Redirection, 2 - Output Redirection: >, 3 - Output Redirection: >>
//NOTE: It is possible has input and ouput, in this case, output will be prefered
int hasRedirection(char *line){

	char *red = strstr(line, ">");

	if (strstr(line, "<") != NULL && red == NULL) return 1; //It has only input redirection

	else {
		//It could be input redirection too
		if(red != NULL && *(red+1) == '>') return 2;
		else if(red != NULL && *(red+1) != '>') return 3; 
		
	}


	return 0;
}	

//TEST STATUS: OK
void testHasRedirection(char *line){

	if(hasRedirection(line) == 0) printf("No redirection\n");

	else if(hasRedirection(line) == 1) printf("Input redirection: <\n");

	else if(hasRedirection(line) == 2) printf("Output redirection: >>\n");

	else if(hasRedirection(line) == 3) printf("Output redirection: >\n");

	else printf("Error in hasRedirection()!\n");
	
}
	

int parse_command(char* line, char** cmd1, char** cmd2, char* infile, char* outfile){

	//RETURN 0 - if line is “quit” 
	if(strcmp(line, "quit") == 0) return 0;

	//RETURN 1 - a line without redirection and pipe 
	if(!hasPipe(line) && !hasRedirection(line)) return 1;

	//RETURN 2 - a line with only input redirection, without pipe and output redirection.
	if(!hasPipe(line) && hasRedirection(line) == 1) return 2;

	//RETURN 3 - a line with output redirection(>>) and without pipe. WARNING: It could be input redirection too.
	if(!hasPipe(line) && hasRedirection(line) == 2) return 3;

	//RETURN 4 - a line with overwritten output redirection(>) and without pipe. WARNING: It could be input redirection too.
	if(!hasPipe(line) && hasRedirection(line) == 3) return 4;

	//RETURN 5 - a line with pipe and without redirection.
	if(hasPipe(line) && hasRedirection(line) == 0) return 5;

	//RETURN 6 - a line with pipe and only input redirection.
	if(hasPipe(line) && hasRedirection(line) == 1) return 6;

	//RETURN 7 - a line with pipe and output redirection(>>). WARNING: It could be input redirection too.
	if(hasPipe(line) && hasRedirection(line) == 2) return 7;

	//RETURN 8 - a line with pipe and overwritten output redirection(>). WARNING: It could be input redirection too.
	if(hasPipe(line) && hasRedirection(line) == 3) return 8;


	//RETURN 9 - something else yet to be handled by our program but is handled by the shell perhaps.
	printf("This function was not implement yet. Sorry :(\n");
	return 9;

}

void testParse_Command(char* line){

	// RETURN 0 : TEST STATUS = OK
	if(parse_command(line, NULL,NULL,NULL, NULL) == 0) printf("Quit!\n");

	// RETURN 1 : TEST STATUS = OK
	if (parse_command(line, NULL,NULL,NULL, NULL) == 1) printf("No pipe and no redirection!\n");

	// RETURN 2 : TEST STATUS = OK
	if (parse_command(line, NULL,NULL,NULL, NULL) == 2) printf("No pipe\nInput Redirection: <\n");

	// RETURN 3 : TEST STATUS = OK
	if (parse_command(line, NULL,NULL,NULL, NULL) == 3) printf("No pipe\nOutput Redirection: >>\n");

	// RETURN 4 : TEST STATUS = OK
	if (parse_command(line, NULL,NULL,NULL, NULL) == 4) printf("No pipe\nOutput Redirection: >\n");

	// RETURN 5 : TEST STATUS = OK
	if (parse_command(line, NULL,NULL,NULL, NULL) == 5) printf("Pipe!\nNo Redirection!\n");

	// RETURN 6 : TEST STATUS = OK
	if (parse_command(line, NULL,NULL,NULL, NULL) == 6) printf("Pipe!\nInput Redirection: <\n");

	// RETURN 7 : TEST STATUS = OK
	if (parse_command(line, NULL,NULL,NULL, NULL) == 7) printf("Pipe!\nOutput Redirection: >>\n");

	// RETURN 8 : TEST STATUS = OK
	if (parse_command(line, NULL,NULL,NULL, NULL) == 8) printf("Pipe!\nOutput Redirection: >\n");

}

void test(char *line, int i){

	switch(i){
	    case 0:
	       testHasRedirection(line);
	       break; 
	    case 1:
	       testParse_Command(line);
	       break;
	 
	    //default : 
	       
	}
}


int main(int argc, char *argv[])
{	
	char *line = getInput(argv);
	printf("Hello, World!\n");
	printf("The value of argc (number of command line arguments) is %d\n", argc);
        //display(argc, argv);
	
	test(line, 1);
	


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