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
#include <unistd.h>

 //CONSTANTS
 #define CMDSIZE 20
 #define BUFSIZE 1024
 #define CSTRSIZE 100


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

//BUG: If the user type "ls -a -b " it will recognize " " as a command.
void getCommand(char *input, char **cmd){

		int size = strlen(input);

		char *input_copy = malloc((size+1)* sizeof (char));

		input_copy = strcpy(input_copy, input);

		int i = 0;

		char *space = NULL;

		char *aux = NULL;

		//processing string - put a space in the final.
		if(*(input_copy+size) != ' ') {
			*(input_copy+size) = ' '; 
			*(input_copy+size+1) = '\0'; 
		}

		aux = strstr(input_copy,"<");
		if(aux != NULL) *aux = '\0';

		aux = strstr(input_copy,">");
		if(aux != NULL) *aux = '\0';

		aux = strstr(input_copy,"|");
		if(aux != NULL) *aux = '\0';


		while(space != NULL || i == 0){

			space = strstr(input_copy, " ");

			if(space != NULL){

				*space = '\0';

				cmd[i] = input_copy;

				if(strlen(cmd[i]) == 0) i--; //this line avoid to create cmd with size 0 that happens when a user type additional spaces.

				input_copy = space+1;

			}

			i++;

		}
		
		cmd[i-1] = NULL;

}

int getInputRedirection(char *input, char *infile){
	
	char *dec = strstr(input,"<");

	int size = strlen(input);

	char *input_copy = malloc((size+1)* sizeof (char));

	input_copy = strcpy(input_copy, input);

	char *space = NULL;

	

	if(dec != NULL) {

		input_copy = dec+1;

		while(*input_copy == ' ') input_copy++; //processing the string eliminating the first spaces

		size = strlen(input_copy);

		if(*(input_copy+size) != ' ') {
			*(input_copy+size) = ' '; 
			*(input_copy+size+1) = '\0'; 
		}//inserting a space in the end.

		space = strstr(input_copy, " ");



		if(space != NULL){
			
			strcpy(infile, input_copy);
			space = strstr(infile, " ");
			*space = '\0';
			}

		return 1;

	}

	return 0;
	

}

int getOuputRedirection(char *input, char *outfile){
	
	char *dec = strstr(input,">>");

	int size = strlen(input);

	char *input_copy = malloc((size+1)* sizeof (char));

	input_copy = strcpy(input_copy, input);

	char *space = NULL;

	if(dec != NULL) {

		input_copy = dec+2;

		while(*input_copy == ' ') input_copy++; //processing the string eliminating the first spaces

		size = strlen(input_copy);

		if(*(input_copy+size) != ' ') {
			*(input_copy+size) = ' '; 
			*(input_copy+size+1) = '\0'; 
		}//inserting a space in the end.

		space = strstr(input_copy, " ");

		if(space != NULL){

			strcpy(outfile, input_copy);
			space = strstr(outfile, " ");
			*space = '\0';
			}

		return 1;

	}

	return 0;
	

}

int getOuputOverwrittenRedirection(char *input, char *outfile){
	
	char *dec = strstr(input,">");

	int size = strlen(input);

	char *input_copy = malloc((size+1)* sizeof (char));

	input_copy = strcpy(input_copy, input);

	char *space = NULL;

	if(dec != NULL) {

		input_copy = dec+1;

		while(*input_copy == ' ') input_copy++; //processing the string eliminating the first spaces

		size = strlen(input_copy);

		if(*(input_copy+size) != ' ') {
			*(input_copy+size) = ' '; 
			*(input_copy+size+1) = '\0'; 
		}//inserting a space in the end.

		space = strstr(input_copy, " ");

		if(space != NULL){

			strcpy(outfile, input_copy);
			space = strstr(outfile, " ");
			*space = '\0';
			}

		return 1;

	}

	return 0;
	

}

void splitPipeCommand(char *input, char *cmd1, char *cmd2){

	char *pipe = strstr(input, "|");
	*pipe = '\0';
	pipe++;

	while(*pipe == ' ') pipe++; //processing the string eliminating the first spaces

	
	strcpy(cmd1, input);
	strcpy(cmd2, pipe);

}



	

int parse_command(char* line, char** cmd1, char** cmd2, char* infile, char* outfile){

	//RETURN 0 - if line is “quit” 
	if(strcmp(line, "quit") == 0) return 0;

	//RETURN 1 - a line without redirection and pipe 
	if(!hasPipe(line) && !hasRedirection(line)) {

		pid_t pid;

		getCommand(line, cmd1);

		//fork a child process
		pid = fork();

		if(pid < 0 ) { //error ocurred

			fprintf(stderr, "Fork Failed");
			return 1;
		}

		else if(pid == 0){ //child process

			execvp(cmd1[0], cmd1);
		}

		else { //parent process

			//parent will wait for the child to complete
			wait(NULL);
		}

		return 1;

	}

	//RETURN 2 - a line with only input redirection, without pipe and output redirection.
	if(!hasPipe(line) && hasRedirection(line) == 1) {

		getCommand(line, cmd1);

		getInputRedirection(line, infile);

		return 2;

	}

	//RETURN 3 - a line with output redirection(>>) and without pipe. WARNING: It could be input redirection too.
	if(!hasPipe(line) && hasRedirection(line) == 2) {

		getCommand(line, cmd1);
		getInputRedirection(line, infile);
		getOuputRedirection(line, outfile);
		

		return 3;
	}

	//RETURN 4 - a line with overwritten output redirection(>) and without pipe. WARNING: It could be input redirection too.
	if(!hasPipe(line) && hasRedirection(line) == 3) {

		getCommand(line, cmd1);
		getInputRedirection(line, infile);
		getOuputOverwrittenRedirection(line, outfile);
		
		return 4;

	}

	//RETURN 5 - a line with pipe and without redirection.
	if(hasPipe(line) && hasRedirection(line) == 0) {

		char line1[CSTRSIZE];
		char line2[CSTRSIZE];

		splitPipeCommand(line, line1, line2);

		getCommand(line1, cmd1);

		getCommand(line2, cmd2);

		return 5;

	}

	//RETURN 6 - a line with pipe and only input redirection.
	if(hasPipe(line) && hasRedirection(line) == 1) {

		char line1[CSTRSIZE];
		char line2[CSTRSIZE];

		splitPipeCommand(line, line1, line2);

		getCommand(line1, cmd1);

		getCommand(line2, cmd2);

		getInputRedirection(line1, infile);

		getInputRedirection(line2, infile);

	
		return 6;
	}


	//RETURN 7 - a line with pipe and output redirection(>>). WARNING: It could be input redirection too.
	if(hasPipe(line) && hasRedirection(line) == 2) {

		char line1[CSTRSIZE];
		char line2[CSTRSIZE];

		splitPipeCommand(line, line1, line2);

		getCommand(line1, cmd1);

		getCommand(line2, cmd2);

		getInputRedirection(line1, infile);

		getInputRedirection(line2, infile);

		getOuputRedirection(line1, outfile);

		getOuputRedirection(line2, outfile);


		return 7;

	}

	//RETURN 8 - a line with pipe and overwritten output redirection(>). WARNING: It could be input redirection too.
	if(hasPipe(line) && hasRedirection(line) == 3) {


		char line1[CSTRSIZE];
		char line2[CSTRSIZE];

		splitPipeCommand(line, line1, line2);

		getCommand(line1, cmd1);

		getCommand(line2, cmd2);

		getInputRedirection(line1, infile);

		getInputRedirection(line2, infile);

		getOuputOverwrittenRedirection(line1, outfile);

		getOuputOverwrittenRedirection(line2, outfile);


		return 8;

	}


	//RETURN 9 - something else yet to be handled by our program but is handled by the shell perhaps.
	printf("This function was not implement yet. Sorry :(\n");
	return 9;

}
//For some reason, the parse command is executed 2 times
void testParse_Command(char* line, char** cmd1){

	// RETURN 0 : TEST STATUS = INCOMPLETE : CONDITION TEST = OK : PARSING TEST = OK
	if(parse_command(line, NULL,NULL,NULL, NULL) == 0) printf("Quit!\n");

	// RETURN 1 : TEST STATUS = INCOMPLETE : CONDITION TEST = OK : PARSING TEST = NOT TEST
	else if (parse_command(line, cmd1, NULL,NULL, NULL) == 1) printf("No pipe and no redirection!\n");

	// RETURN 2 : TEST STATUS = INCOMPLETE
	else if (parse_command(line, NULL,NULL,NULL, NULL) == 2) printf("No pipe\nInput Redirection: <\n");

	// RETURN 3 : TEST STATUS = INCOMPLETE
	else if (parse_command(line, NULL,NULL,NULL, NULL) == 3) printf("No pipe\nOutput Redirection: >>\n");

	// RETURN 4 : TEST STATUS = INCOMPLETE
	else if (parse_command(line, NULL,NULL,NULL, NULL) == 4) printf("No pipe\nOutput Redirection: >\n");

	// RETURN 5 : TEST STATUS = INCOMPLETE
	else if (parse_command(line, NULL,NULL,NULL, NULL) == 5) printf("Pipe!\nNo Redirection!\n");

	// RETURN 6 : TEST STATUS = INCOMPLETE
	else if (parse_command(line, NULL,NULL,NULL, NULL) == 6) printf("Pipe!\nInput Redirection: <\n");

	// RETURN 7 : TEST STATUS = INCOMPLETE
	else if (parse_command(line, NULL,NULL,NULL, NULL) == 7) printf("Pipe!\nOutput Redirection: >>\n");

	// RETURN 8 : TEST STATUS = INCOMPLETE
	else if (parse_command(line, NULL,NULL,NULL, NULL) == 8) printf("Pipe!\nOutput Redirection: >\n");

}

void test(char *line, char** cmd1, int i){

	switch(i){
	    case 0:
	       testHasRedirection(line);
	       break; 
	    case 1:
	       testParse_Command(line, cmd1);
	       break;
	 
	    //default : 
	       
	}
}


int main(int argc, char *argv[])
{	
	char line[CSTRSIZE];
	char *cmd1[CMDSIZE];
	char *cmd2[CMDSIZE];
	char infile[CSTRSIZE];
	char outfile[CSTRSIZE];
	int i = 0;

	cmd1[0]=NULL;
    cmd2[0]=NULL;

    printf("Hello, World!\n");
		printf("The value of argc (number of command line arguments) is %d\n", argc);

    if(argc == 2){

    	

	    parse_command(argv[1], cmd1, cmd2, infile, outfile);

		while(cmd1[i] != NULL){

			printf("cmd1[%d]: %s\n", i, cmd1[i]);
			i++;
		}

	 	i  = 0;
	 	
		while(cmd2[i] != NULL){

			printf("cmd2[%d]: %s\n", i, cmd2[i]);
			i++;
		}

		if(strlen(infile) != 0 ) printf("input redirection file name: %s\n", infile);

		if(strlen(outfile) != 0 ) printf("output redirection file name: %s\n", outfile);

		return 1;
	}
    
    else {

    	while(1){
    		printf("myshell-%% ");
    		fgets(line,CSTRSIZE,stdin);
    		
    		line[strlen(line)-1] = '\0';

    		if(parse_command(line, cmd1, cmd2, infile, outfile) == 0) break;

    		while(cmd1[i] != NULL){

			printf("cmd1[%d]: %s\n", i, cmd1[i]);
			i++;
			}

		 	i  = 0;
		 	
			while(cmd2[i] != NULL){

				printf("cmd2[%d]: %s\n", i, cmd2[i]);
				i++;
			}

			if(strlen(infile) != 0 ) printf("input redirection file name: %s\n", infile);

			if(strlen(outfile) != 0 ) printf("output redirection file name: %s\n", outfile);

			//Clean these variables.
			infile[0] = '\0';
			outfile[0] = '\0';

   		}

    	return 1;
    }
	


	return 0;
}

  