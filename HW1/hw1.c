/*=============================================================================
 |    File Name:  hw1.c [in which directory on what computer]
 |
 |       Author:  Tarcisio Bruno Carneiro Oliveira
 |     Language:  ANSI C (tested using gcc on turing [might list version infor])
 |   To Compile:  gcc -Wall hw1.c
 |
 |        Class:  CSCE3613 
 |      Project:  Simple Shell
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

//gitTest
 
       
char* getInput(char *argument[]);
 
void getCommand(char *input, char *command);

void getArguments(char *input, char *arguments);

void printArguments(char *arguments);

int hasPipe(char *input);

int getRedirection(char *input, char *file);

void splitPipeCommand(char *input, char *commands[]);


int main(int argc, char *argv[])
{	

	printf("Hello, World!\n");
	printf("The value of argc (number of command line arguments) is %d\n", argc);
	
	if(argc == 2){

		char *input = NULL;		
		input = getInput(argv);

		if(hasPipe(input)) {

			char* inputs[2];
			
			char *commands[2];
			commands[0] = malloc( 10 * sizeof (char));
			commands[1] = malloc( 10 * sizeof (char));
			
			char *arguments[2];
			arguments[0] = malloc( 100 * sizeof (char));
			arguments[1] = malloc( 100 * sizeof (char));

			char *files[2];
			files[0] = malloc( 100 * sizeof (char));
			files[1] = malloc( 100 * sizeof (char));


			splitPipeCommand(input, inputs);

			getCommand(inputs[0], commands[0]);
			printf("The user command or program is: %s\n", commands[0]);

			getArguments(inputs[0], arguments[0]);
			printArguments(arguments[0]);
			if(getRedirection(inputs[0], files[0])){printf("Output file: %s\n", files[0]);};


			printf("Pipe: yes\n");

			if(*inputs[1] == ' ') inputs[1] = inputs[1]+1;
			getCommand(inputs[1], commands[1]);
			printf("The user command or program is: %s\n", commands[1]);

			getArguments(inputs[1], arguments[1]);
			printArguments(arguments[1]);

			if(getRedirection(inputs[1], files[1])){printf("Output file: %s\n", files[1]);};

			

			free(commands[0]);
			free(arguments[0]);
			free(files[0]);
			free(commands[1]);
			free(arguments[1]);
			free(files[1]);

		}

		else {

			char *command = malloc( 10 * sizeof (char));
			char *arguments = malloc( 100 * sizeof (char));
			char *file = malloc( 100 * sizeof (char));

			getCommand(input, command);
			printf("The user command or program is: %s\n", command);

			getArguments(input, arguments);
			printArguments(arguments);

		
			if(getRedirection(input, file)){printf("Output file: %s\n", file);};

			free(command);
			free(arguments);
			free(file);

		}

		return 1;
	}

	return 0;
}



 /*-----------------------getInput --------------------------------------
         |  Function getInput
	 |
	 |  Purpose:  Get the argv[1] where the commands are stored.
	 |
	 |  Parameters:
	 |	argument (IN) - Array of strings where are stored the command lines parameters.
	 |
	 |
	 |  Returns:  argument[1] that is where the string with command are stored. 
	 |
	 *-------------------------------------------------------------------*/
       
char* getInput(char *argument[]){

return argument[1];

}

 /*-----------------------getCommand --------------------------------------
         |  Function getCommand
	 |
	 |  Purpose:  Split the string getting the command used in the command line.
	 |
	 |  Parameters:
	 |	input (IN) - String typed by the user in the prompt.
	 |  command (IN) - Variable that will keep the command typed by the user.
	 |
	 |  Returns:  Nothing.  (This is a void function.)
	 |
	 *-------------------------------------------------------------------*/

void getCommand(char *input, char *command){
		int size = strlen(input);
		int i;

		for(i = 0; i < size; i++){
			if(input[i] == ' ') break;

			else {
				command[i] = input[i];
			}
		}

		command[i] = '\0';
}

 /*-----------------------getArguments --------------------------------------
         |  Function getArguments
	 |
	 |  Purpose:  Split the string getting the arguments typed by the user
	 |
	 |  Parameters:
	 |	input (IN) - String typed by the user in the prompt.
	 |  arguments (IN) - Variable that will keep the arguments typed by the user.
	 |
	 |  Returns:  Nothing.  (This is a void function.)
	 |
	 *-------------------------------------------------------------------*/

void getArguments(char *input, char *arguments){
		
		int size = strlen(input);
		int i;
		int j = 1;
		char *dec = NULL;
		char *input_copy = malloc((size+1)* sizeof (char));

		input_copy = strcpy(input_copy, input);
		dec = strstr(input_copy,">");

	
		//Processing the string
		if(dec!= NULL) *dec = '\0';
		if(input_copy[size-1] == ' ') input_copy[size-1] = '\0';
		arguments[0] = ' ';
		
		for(i = 0; i < size; i++){
			if(input_copy[i] == ' ' ) {
				while(input_copy[i+1] != ' ' && i < size){
					arguments[j] = input_copy[i+1];
					i++;
					j++;
				}
			arguments[j] = ' ';
			j++;
			}
		}

		free(input_copy);
}

/*-----------------------printArguments --------------------------------------
     |  Function printArguments
	 |
	 |  Purpose:  Print the string where the arguments are stored.
	 |
	 |  Parameters:
	 |	input (IN) - String typed by the user in the prompt.
	 |  arguments (IN) - Variable that will keep the arguments typed by the user.
	 |
	 |  Returns:  Nothing.  (This is a void function.)
	 |
	 *-------------------------------------------------------------------*/
void printArguments(char *arguments){
	
	int i;
	for(i = 0; i < strlen(arguments);i++){
			if(arguments[i] == ' ' && arguments[i+1] != '\0'){
				printf("The command line argument to the user command and program is: ");
				while(arguments[i+1] != ' ') {
					printf("%c", arguments[i+1]);
					i++;
				}
				printf("\n");
			}
		}

}

/*-----------------------hasPipe--------------------------------------
     |  Function hasPipe
	 |
	 |  Purpose:  Verify if there is a character pipe in the input.
	 |
	 |  Parameters:
	 |	input (IN) - String typed by the user in the prompt.
	 |  
	 |  Returns:  Nothing.  (This is a void function.)
	 |
	 *-------------------------------------------------------------------*/

int hasPipe(char *input){

	if (strpbrk("|", input) != NULL) return 1;

	else return 0;

}

/*-----------------------getRedirection --------------------------------------
     |  Function getRedirection
	 |
	 |  Purpose:  - Verify if there is a redirection
	 |		- Split the string getting the file where the redirection will be used.
	 |		- Print the type of redirection: > or >>.
	 |
	 |  Parameters:
	 |	input (IN) - String typed by the user in the prompt.
	 |  file (IN) - Variable that will keep the name of the file typed by the user.
	 |
	 |  Returns:  1 if there is a redirection and 0 if there is not.
	 |
	 *-------------------------------------------------------------------*/

int getRedirection(char *input, char *file){
	char *dec = strstr(input,">");
	int i = 0;

	if(dec!= NULL){

		if(*(dec+1) == '>') printf("Output redirection: >>\n");

		else printf("Output redirection: >\n");

		while(*dec!= '\0') {
			if(*dec != ' ' && *dec != '>'){
				file[i] = dec[0];
				i++;
			}
			dec++;
		}

		file[i] = '\0';
		return 1;
	} 

	return 0;
}

 /*-----------------------splitPipeCommand--------------------------------------
         |  Function splitPipeCommand
	 |
	 |  Purpose:  Split the string typed by the user in two according to the pipe
	 |			symbol position.
	 |
	 |  Parameters:
	 |	input (IN) - String typed by the user in the prompt.
	 |  commands (IN) - It is a pointer array with size two that keeps the
	 |				two commands divided for pipe.
	 |
	 |  Returns:  Nothing.  (This is a void function.)
	 |
	 *-------------------------------------------------------------------*/
void splitPipeCommand(char *input, char *commands[]){

	char *pipe = strstr(input, "|");
	*pipe = '\0';
	commands[0] = NULL;
	commands[1] = NULL;
	
	commands[0] = input;
	commands[1] = pipe+1;

}


