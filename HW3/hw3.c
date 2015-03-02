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
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>//Open Constraints


 //CONSTANTS
 #define CMDSIZE 30
 #define BUFSIZE 1024
 #define CSTRSIZE 100


int hasPipe(char *line);

void getCommand(char *line, char **cmd);

int hasRedirection(char *line);

int getInputRedirection(char *line, char *infile);

int getOuputRedirection(char *line, char *outfile);

int getOuputOverwrittenRedirection(char *line, char *outfile);

void splitPipeCommand(char *line, char *cmd1, char *cmd2);	

int parse_command(char* line, char** cmd1, char** cmd2, char* infile, char* outfile);

//Execution functions

void exec_cmd(char** cmd1);

void exec_cmd_in(char** cmd1, char* infile);

void exec_cmd_opt_in_append(char** cmd1, char* infile, char* outfile);

void exec_cmd_opt_in_write(char** cmd1, char* infile, char* outfile);

void exec_pipe(char** cmd1, char** cmd2);

void exec_pipe_in(char** cmd1, char** cmd2, char* infile);

void exec_pipe_opt_in_append(char** cmd1,char** cmd2,char* infile,char* outfile);

void exec_pipe_opt_in_write(char** cmd1,char** cmd2,char* infile,char* outfile);


int main(int argc, char *argv[])
{	
	char line[BUFSIZE];
	char *cmd1[CMDSIZE];
	char *cmd2[CMDSIZE];
	char infile[CSTRSIZE];
	char outfile[CSTRSIZE];
	int i = 0;
	int parseReturn = 0;

	cmd1[0]=NULL;
    cmd2[0]=NULL;

    infile[0] = '\0';
	outfile[0] = '\0';

    printf("MyShell 1.0v by Tarcisio Oliveira\n");


    if(argc == 2){

    	
    	parseReturn = parse_command(argv[1], cmd1, cmd2, infile, outfile);

    	switch(parseReturn){

    		case 1:
				exec_cmd(cmd1);
      			break; 

    		case 2:
       			exec_cmd_in(cmd1, infile);
      			break; 

    		case 3:
       			exec_cmd_opt_in_append(cmd1, infile, outfile);
      			break; 

    		case 4:
       			exec_cmd_opt_in_write(cmd1, infile, outfile);
      			break; 

    		case 5:
       			exec_pipe(cmd1, cmd2);
      			break; 

    		case 6:
       			exec_pipe_in(cmd1, cmd2, infile);
      			break; 

    		case 7:
       			exec_pipe_opt_in_append(cmd1, cmd2, infile, outfile);
      			break; 

    		case 8:
       			exec_pipe_opt_in_write(cmd1, cmd2, infile, outfile);
      			break; 

    		default : 
       			printf("Not handled at this time!");//type here
		}

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
    
    else if(argc == 1){

    	while(1){
    		printf("myshell-%% ");
    		fgets(line,BUFSIZE,stdin);
    		
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
	
	else printf("usage error\n");

	return 0;
}

/*-----------------------hasPipe--------------------------------------
     |  Function hasPipe
	 |
	 |  Purpose:  Verify if there is a character pipe in the input.
	 |
	 |  Parameters:
	 |	line (IN) - String typed by the user in the prompt.
	 |  
	 |  Returns:  (0) There is not pipe
	 |			  (1) There is pipe
	 |
	 *-------------------------------------------------------------------*/

int hasPipe(char *line){

	if (strpbrk("|", line) != NULL) return 1;

	else return 0;

}

/*-----------------------hasRedirection--------------------------------------
     |  Function hasRedirection
	 |
	 |  Purpose:  Verify if there are some type of redirection characters
	 |
	 |  Parameters:
	 |	line (IN) - String typed by the user in the prompt.
	 |  
	 |  Returns:  (0) There is not redirection
	 |			  (1) Input redirection '<'
	 |			  (2) Output redirection '>>'
	 |			  (3) Overwritten output redirection '>' 
	 |
	 *-------------------------------------------------------------------*/

int hasRedirection(char *line){

	char *red = strstr(line, ">");

	if (strstr(line, "<") != NULL && red == NULL) return 1; 

	else {
		if(red != NULL && *(red+1) == '>') return 2;
		else if(red != NULL && *(red+1) != '>') return 3; 
		
	}


	return 0;
}	

 /*-----------------------getCommand --------------------------------------
         |  Function getCommand
	 |
	 |  Purpose:  Getting the command used in the command line.
	 |
	 |  Parameters:
	 |	line (IN) - String typed by the user in the prompt.
	 |  cmd (IN) - Variable that will keep the command typed by the user.
	 |
	 |  Returns:  Nothing.  (This is a void function.)
	 |
	 *-------------------------------------------------------------------*/

void getCommand(char *line, char **cmd){

		int size = strlen(line);

		char *input_copy = malloc((size+1)* sizeof (char));

		input_copy = strcpy(input_copy, line);

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

/*-----------------------getInputRedirection --------------------------------------
     |  Function getInputRedirection
	 |
	 |  Purpose: Getting the input redirection file name.
	 |
	 |  Parameters:
	 |	line (IN) - String typed by the user in the prompt.
	 |  infile (IN) - Variable that will keep the name of the file typed by the user.
	 |
	 |  Returns:  1 if there is an input redirection and 0 if there is not.
	 |
	 *-------------------------------------------------------------------*/

int getInputRedirection(char *line, char *infile){
	
	char *dec = strstr(line,"<");

	int size = strlen(line);

	char *input_copy = malloc((size+1)* sizeof (char));

	input_copy = strcpy(input_copy, line);

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

/*-----------------------getOutputRedirection--------------------------------------
     |  Function getOutputRedirection
	 |
	 |  Purpose:  Getting the output redirection file name.
	 |		 
	 |  Parameters:
	 |	line (IN) - String typed by the user in the prompt.
	 |  outfile (IN) - Variable that will keep the name of the file typed by the user.
	 |
	 |  Returns:  1 if there is an output redirection and 0 if there is not.
	 |
	 *-------------------------------------------------------------------*/
int getOuputRedirection(char *line, char *outfile){
	
	char *dec = strstr(line,">>");

	int size = strlen(line);

	char *input_copy = malloc((size+1)* sizeof (char));

	input_copy = strcpy(input_copy, line);

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

/*-----------------------getOuputOverwrittenRedirection--------------------------------------
     |  Function getOuputOverwrittenRedirection
	 |
	 |  Purpose:  Getting the overwritten output redirection file name.
	 |		 
	 |  Parameters:
	 |	line (IN) - String typed by the user in the prompt.
	 |  outfile (IN) - Variable that will keep the name of the file typed by the user.
	 |
	 |  Returns:  1 if there is an output redirection and 0 if there is not.
	 |
	 *-------------------------------------------------------------------*/
int getOuputOverwrittenRedirection(char *line, char *outfile){
	
	char *dec = strstr(line,">");

	int size = strlen(line);

	char *input_copy = malloc((size+1)* sizeof (char));

	input_copy = strcpy(input_copy, line);

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


 /*-----------------------splitPipeCommand--------------------------------------
     |  Function splitPipeCommand
	 |
	 |  Purpose:  Split the string typed by the user in two according to the pipe
	 |			symbol position.
	 |
	 |  Parameters:
	 |	line (IN) - String typed by the user in the prompt.
	 |  cmd1 (IN) - It keeps the commands before the pipe.
	 |  cmd2 (IN) - It keeps the commands after the pipe.
	 |
	 |  Returns:  Nothing.  (This is a void function.)
	 |
	 *-------------------------------------------------------------------*/
void splitPipeCommand(char *line, char *cmd1, char *cmd2){

	char *pipe = strstr(line, "|");
	*pipe = '\0';
	pipe++;

	while(*pipe == ' ') pipe++; //processing the string eliminating the first spaces

	
	strcpy(cmd1, line);
	strcpy(cmd2, pipe);
}

/*-----------------------parse_command--------------------------------------
     |  Function parse_command
	 |
	 |  Purpose:  Processing the string typed by the user.
	 |
	 |  Parameters:
	 |	line (IN) - String typed by the user in the prompt.
	 |  cmd1 (IN) - It keeps the commands before the pipe.
	 |  cmd2 (IN) - It keeps the commands after the pipe.
	 |  infile (IN) - Variable that will keep the name of the file typed by
	 |				  the user after the input redirection.
	 |	outfile (IN) - Variable that will keep the name of the file typed by
	 |				   the user after the output redirection.
	 |  
	 |  Returns:  (0) The user typed "quit"
	 |			  (1) A line without redirection and pipe 
	 |			  (2) A line with only input redirection, without pipe and output redirection.
	 |			  (3) A line with output redirection(>>) and without pipe. WARNING: It could be input redirection too.
	 |			  (4) A line with overwritten output redirection(>) and without pipe. WARNING: It could be input redirection too.
	 |			  (5) A line with pipe and without redirection.
	 |			  (6) A line with pipe and only input redirection.
	 |			  (7) A line with pipe and output redirection(>>). WARNING: It could be input redirection too.
	 |			  (8) A line with pipe and overwritten output redirection(>). WARNING: It could be input redirection too.
	 |			  (9) Something else yet to be handled by our program but is handled by the shell perhaps.
	 |
	 *-------------------------------------------------------------------*/

int parse_command(char* line, char** cmd1, char** cmd2, char* infile, char* outfile){

	//RETURN 0 - if line is “quit” 
	if(strcmp(line, "quit") == 0) return 0;

	//RETURN 1 - a line without redirection and pipe 
	if(!hasPipe(line) && !hasRedirection(line)) {

		getCommand(line, cmd1);

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

		//getInputRedirection(line2, infile);

	
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

		//getInputRedirection(line2, infile);

		//getOuputRedirection(line1, outfile);

		if(hasRedirection(line2) == 2) getOuputRedirection(line2, outfile);


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

		//getInputRedirection(line2, infile);

		//getOuputOverwrittenRedirection(line1, outfile);

		if(hasRedirection(line2) == 3) getOuputOverwrittenRedirection(line2, outfile);


		return 8;

	}

	//RETURN 9 - something else yet to be handled by our program but is handled by the shell perhaps.
	return 9;

}


//Execution functions

void exec_cmd(char** cmd1){

	pid_t pid;

	//fork a child process
	pid = fork();

	if(pid < 0 ) { //error ocurred

		fprintf(stderr, "Fork Failed");

	}

	else if(pid == 0){ //child process

		if(execvp(cmd1[0], cmd1) == -1 ) exit(1);
			
	}

	else { //parent process

		//parent will wait for the child to complete
		wait(NULL);
	}
}

void exec_cmd_in(char** cmd1, char* infile){

	pid_t pid;
	int fd;

	//Input Redirection
	//Opens the infile file and hold the file descriptor in fd
	if ( (fd = open(infile, O_RDONLY,  S_IRUSR | S_IWUSR)) == -1 ){
		printf("It was not possible to open the input file.\n");
		exit(1);        
	}

	//fork a child process
	pid = fork();
	if(pid < 0 ) { //error ocurred
		fprintf(stderr, "Fork Failed");
		exit(1);
	}

	else if(pid == 0){ //child process

		//Makes the stdin descriptor points to infile.
		if ( dup2(fd, 0) == -1) {
			printf("It was not possible to copy the file descriptor\n");
			exit(1);
		}

		if(execvp(cmd1[0], cmd1) == -1 ) exit(1);
			
	}
	else { //parent process
		//parent will wait for the child to complete
		wait(NULL);
		//close the descriptors
		close(fd);
	}
	

}

void exec_cmd_opt_in_append(char** cmd1, char* infile, char* outfile){
	pid_t pid;
	int fd,fd1,fd2,fd3;

	//Output Redirection
	//Keeps the stdout file descriptor information in fd1
	if ( (fd1 = dup(1)) == -1 ) {
	    printf("It was not possible to copy the file descriptor\n");
	    exit(1);
	}

	//Opens the outfile file and hold the file descriptor in fd
	if ( (fd = open(outfile, O_RDWR | O_CREAT | O_APPEND,  S_IRUSR | S_IWUSR)) == -1 ){
	    printf("It was not possible to open the output file.\n");
		exit(1);   
	}

	//Makes the stdout descriptor points to infile.
	if ( dup2(fd, 1) == -1) {
	    printf("It was not possible to copy the file descriptor\n");
		exit(1);
	}

	//Verifies if there is an infile, then repeats the output command but for the input
	if(infile[0] != '\0'){
		if ( (fd3 = dup(0)) == -1 ) {
		    printf("It was not possible to copy the file descriptor\n");
	    	exit(1);
		}

		if ( (fd2 = open(infile, O_RDONLY,  S_IRUSR | S_IWUSR)) == -1 ){
			printf("It was not possible to open the input file.\n");
			exit(1);    
		}

		if ( dup2(fd2, 0) == -1) {
		    printf("It was not possible to copy the file descriptor\n");
			exit(1);
		}
	 }

	//fork a child process
	pid = fork();

	if(pid < 0 ) { //error ocurred

		fprintf(stderr, "Fork Failed");

	}

	else if(pid == 0){ //child process
		
		if(execvp(cmd1[0], cmd1) == -1 ) exit(1);
			
	}

	else { //parent process

		//parent will wait for the child to complete
		wait(NULL);
	}

	// restore, output goes to stdout
	if ( dup2(fd1, 1) == -1 ){
	    // error
	};

	// restore, input goes to stdout
	if ( dup2(fd3, 0) == -1 ){
	    // error
	};

	//close the descriptors
	close(fd);
	close(fd1);

	if(infile[0] != '\0'){
		close(fd2);
		close(fd3);
	}


}

void exec_cmd_opt_in_write(char** cmd1, char* infile, char* outfile){

	pid_t pid;
	int fd,fd1,fd2,fd3;

	//Redirections
	//Keeps the stdout file descriptor information in fd1
	if ( (fd1 = dup(1)) == -1 ) {
	    printf("It was not possible to copy the file descriptor\n");
	    exit(1);
	};

	//Opens the outfile file and hold the file descriptor in fd
	if ( (fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC,  S_IRUSR | S_IWUSR)) == -1 ){
		printf("It was not possible to open the output file.\n");
		exit(1); 
	}
	
	//Makes the stdout descriptor points to infile.
	if ( dup2(fd, 1) == -1) {
	    printf("It was not possible to copy the file descriptor\n");
		exit(1);
	}


	//Verifies if there is an infile, then repeats the output command but for the input
	if(infile[0] != '\0'){
		if ( (fd3 = dup(0)) == -1 ) {
		    printf("It was not possible to copy the file descriptor\n");
	    	exit(1);
		}

		if ( (fd2 = open(infile, O_RDONLY,  S_IRUSR | S_IWUSR)) == -1 ){
			printf("It was not possible to open the input file.\n");
			exit(1);    
		}

		if ( dup2(fd2, 0) == -1) {
		    printf("It was not possible to copy the file descriptor\n");
			exit(1);
		}
	}


	//fork a child process
	pid = fork();

	if(pid < 0 ) { //error ocurred

		fprintf(stderr, "Fork Failed");

	}

	else if(pid == 0){ //child process
		
		if(execvp(cmd1[0], cmd1) == -1 ) exit(1);
			
	}

	else { //parent process

		//parent will wait for the child to complete
		wait(NULL);
	}

	// restore, output goes to stdout
	if ( dup2(fd1, 1) == -1 ){
	    // error
	};

	// restore, input goes to stdout
	if ( dup2(fd3, 0) == -1 ){
	    // error
	};

	//Close the descriptors
	close(fd);
	close(fd1);
	
	if(infile[0] != '\0'){
		close(fd2);
		close(fd3);
	}
}


void exec_pipe(char** cmd1, char** cmd2){

	pid_t pid;
	int pipefd[2]; // return value from the pipe system call
    int fdw, fdr;

    // making a pipe
    if ( pipe(pipefd) == -1 ) {
        printf("It was not possible to create the pipe\n");
		exit(1);
    }
    // pipe[1] is the write end of the pipe
    // pipe[0] is the read end of the pipe

    // save a copy of stdout in entry fdw
    // you may want to check the fdw value
    fdw=dup(STDOUT_FILENO);
    if (fdw == -1){
    	printf("It was not possible to copy the file descriptor\n");
		exit(1);      
    }

    // changing descriptor table, making write end of pipe as stdout
    if ( dup2(pipefd[1], STDOUT_FILENO) == -1) {
        printf("It was not possible to copy the file descriptor\n");
		exit(1);  
    }

	//fork a child process
	pid = fork();


	if(pid < 0 ) { //error ocurred

		fprintf(stderr, "Fork Failed");

	}

	else if(pid == 0){ //child process

		if(execvp(cmd1[0], cmd1) == -1 ) exit(1);
			
	}

	else { //parent process

		//parent will wait for the child to complete
		wait(NULL);
	}

	// restore stdout from saved values
    if ( dup2(fdw, STDOUT_FILENO) == -1) {
        printf("It was not possible to copy the file descriptor\n");
		exit(1); 
    }

 
	// save a copy of stdin in entry fdr
    // you may want to check the fdr value
    fdr=dup(STDIN_FILENO);
    if (fdr == -1){
        printf("It was not possible to copy the file descriptor\n");
		exit(1); 
    }

    
    // make read end of pipe as stdin
    if ( dup2(pipefd[0], STDIN_FILENO ) == -1) {
        printf("It was not possible to copy the file descriptor\n");
		exit(1); 
    }

    //this line is essential, without it, the code still waiting for an input
	close(pipefd[1]);

	//fork a child process
	pid = fork();

	if(pid < 0 ) { //error ocurred

		fprintf(stderr, "Fork Failed");

	}

	else if(pid == 0){ //child process
		if(execvp(cmd2[0], cmd2) == -1 ) exit(1);
	}

	else { //parent process
		//parent will wait for the child to complete
		wait(NULL);
	}

	// restore stdin and stdout from saved values
    if ( dup2(fdr, STDIN_FILENO) == -1) {
        printf("It was not possible to copy the file descriptor\n");
		exit(1); 
    }

	// closing up
    close(fdw);
    close(fdr);
    close(pipefd[0]);
}



void exec_pipe_in(char** cmd1, char** cmd2, char* infile){

	pid_t pid;
	int pipefd[2]; // return value from the pipe system call
    int fdw, fdr;
    int fd;

    // making a pipe
    if ( pipe(pipefd) == -1 ) {
        printf("It was not possible to create the pipe\n");
		exit(1);
    }
    // pipe[1] is the write end of the pipe
    // pipe[0] is the read end of the pipe

    // save a copy of stdout in entry fdw
    // you may want to check the fdw value
    fdw=dup(STDOUT_FILENO);
    if (fdw == -1){
    	printf("It was not possible to copy the file descriptor\n");
		exit(1);      
    }

    // changing descriptor table, making write end of pipe as stdout
    if ( dup2(pipefd[1], STDOUT_FILENO) == -1) {
        printf("It was not possible to copy the file descriptor\n");
		exit(1);  
    }


	//Opens the infile file and hold the file descriptor in fd
	if ( (fd = open(infile, O_RDONLY,  S_IRUSR | S_IWUSR)) == -1 ){

		printf("It was not possible to open the input file.\n");
		exit(1);
	        
	}

	//fork a child process
	pid = fork();


	if(pid < 0 ) { //error ocurred

		fprintf(stderr, "Fork Failed");

	}

	else if(pid == 0){ //child process

		//Makes the stdin descriptor points to infile.
		if ( dup2(fd, 0) == -1) {
			printf("It was not possible to copy the file descriptor\n");
			exit(1);
		}

		if(execvp(cmd1[0], cmd1) == -1 ) exit(1);
			
	}

	else { //parent process

		//parent will wait for the child to complete
		wait(NULL);
	}

	// restore stdout from saved values
    if ( dup2(fdw, STDOUT_FILENO) == -1) {
        printf("It was not possible to copy the file descriptor\n");
		exit(1); 
    }

 
	// save a copy of stdin in entry fdr
    // you may want to check the fdr value
    fdr=dup(STDIN_FILENO);
    if (fdr == -1){
        printf("It was not possible to copy the file descriptor\n");
		exit(1); 
    }

    
    // make read end of pipe as stdin
    if ( dup2(pipefd[0], STDIN_FILENO ) == -1) {
        printf("It was not possible to copy the file descriptor\n");
		exit(1); 
    }

    //this line is essential, without it, the code still waiting for an input
	close(pipefd[1]);

	//fork a child process
	pid = fork();

	if(pid < 0 ) { //error ocurred

		fprintf(stderr, "Fork Failed");

	}

	else if(pid == 0){ //child process
		if(execvp(cmd2[0], cmd2) == -1 ) exit(1);
	}

	else { //parent process
		//parent will wait for the child to complete
		wait(NULL);
	}

	// restore stdin and stdout from saved values
    if ( dup2(fdr, STDIN_FILENO) == -1) {
        printf("It was not possible to copy the file descriptor\n");
		exit(1); 
    }

	// closing up
    close(fdw);
    close(fdr);
    close(pipefd[0]);
    close(fd);

}

void exec_pipe_opt_in_append(char** cmd1,char** cmd2,char* infile,char* outfile){
	
	pid_t pid;
	int pipefd[2]; // return value from the pipe system call
    int fdw, fdr;
    int fd,fd1;

    // making a pipe
    if ( pipe(pipefd) == -1 ) {
        printf("It was not possible to create the pipe\n");
		exit(1);
    }
    // pipe[1] is the write end of the pipe
    // pipe[0] is the read end of the pipe

    // save a copy of stdout in entry fdw
    // you may want to check the fdw value
    fdw=dup(STDOUT_FILENO);
    if (fdw == -1){
    	printf("It was not possible to copy the file descriptor\n");
		exit(1);      
    }

    // changing descriptor table, making write end of pipe as stdout
    if ( dup2(pipefd[1], STDOUT_FILENO) == -1) {
        printf("It was not possible to copy the file descriptor\n");
		exit(1);  
    }

    //Opens the outfile file and hold the file descriptor in fd
    if(outfile[0] != '\0'){
		if ( (fd = open(outfile, O_RDWR | O_CREAT | O_APPEND,  S_IRUSR | S_IWUSR)) == -1 ){
		    printf("It was not possible to open the output file.\n");
			exit(1);   
		}
	}

	//Verifies if there is an infile, then repeats the output command but for the input
	if(infile[0] != '\0'){

		if ( (fd1 = open(infile, O_RDONLY,  S_IRUSR | S_IWUSR)) == -1 ){
			printf("It was not possible to open the input file.\n");
			exit(1);    
		}

		if ( dup2(fd1, 0) == -1) {
		    printf("It was not possible to copy the file descriptor\n");
			exit(1);
		}
	 }

	//fork a child process
	pid = fork();


	if(pid < 0 ) { //error ocurred

		fprintf(stderr, "Fork Failed");

	}

	else if(pid == 0){ //child process

		//Makes the stdin descriptor points to infile.
		if(infile[0] != '\0'){
			if ( dup2(fd1, 0) == -1) {
			    printf("It was not possible to copy the file descriptor\n");
				exit(1);
			}
	 	}

		if(execvp(cmd1[0], cmd1) == -1 ) exit(1);
			
	}

	else { //parent process

		//parent will wait for the child to complete
		wait(NULL);
	}

	// restore stdout from saved values
    if ( dup2(fdw, STDOUT_FILENO) == -1) {
        printf("It was not possible to copy the file descriptor\n");
		exit(1); 
    }

 
	// save a copy of stdin in entry fdr
    // you may want to check the fdr value
    fdr=dup(STDIN_FILENO);
    if (fdr == -1){
        printf("It was not possible to copy the file descriptor\n");
		exit(1); 
    }

    
    // make read end of pipe as stdin
    if ( dup2(pipefd[0], STDIN_FILENO ) == -1) {
        printf("It was not possible to copy the file descriptor\n");
		exit(1); 
    }

    //this line is essential, without it, the code still waiting for an input
	close(pipefd[1]);

	//fork a child process
	pid = fork();

	if(pid < 0 ) { //error ocurred

		fprintf(stderr, "Fork Failed");

	}

	else if(pid == 0){ //child process
		if(outfile[0] != '\0'){
			//Makes the stdout descriptor points to outfile.
			if ( dup2(fd, 1) == -1) {
			    printf("It was not possible to copy the file descriptor\n");
				exit(1);
			}
		}
		if(execvp(cmd2[0], cmd2) == -1 ) exit(1);
	}

	else { //parent process
		//parent will wait for the child to complete
		wait(NULL);
	}

	// restore stdin and stdout from saved values
    if ( dup2(fdr, STDIN_FILENO) == -1) {
        printf("It was not possible to copy the file descriptor\n");
		exit(1); 
    }

	// closing up
    close(fdw);
    close(fdr);
    close(pipefd[0]);
    close(fd);
    close(fd1);

}

void exec_pipe_opt_in_write(char** cmd1,char** cmd2,char* infile,char* outfile){
	pid_t pid;
	int pipefd[2]; // return value from the pipe system call
    int fdw, fdr;
    int fd,fd1;

    // making a pipe
    if ( pipe(pipefd) == -1 ) {
        printf("It was not possible to create the pipe\n");
		exit(1);
    }
    // pipe[1] is the write end of the pipe
    // pipe[0] is the read end of the pipe

    // save a copy of stdout in entry fdw
    // you may want to check the fdw value
    fdw=dup(STDOUT_FILENO);
    if (fdw == -1){
    	printf("It was not possible to copy the file descriptor\n");
		exit(1);      
    }

    // changing descriptor table, making write end of pipe as stdout
    if ( dup2(pipefd[1], STDOUT_FILENO) == -1) {
        printf("It was not possible to copy the file descriptor\n");
		exit(1);  
    }

    //Opens the outfile file and hold the file descriptor in fd
	if(outfile[0] != '\0'){
		if ( (fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC,  S_IRUSR | S_IWUSR)) == -1 ){
			printf("It was not possible to open the output file.\n");
			exit(1); 
		}
	}

	//Verifies if there is an infile, then repeats the output command but for the input
	if(infile[0] != '\0'){

		if ( (fd1 = open(infile, O_RDONLY,  S_IRUSR | S_IWUSR)) == -1 ){
			printf("It was not possible to open the input file.\n");
			exit(1);    
		}

	 }

	//fork a child process
	pid = fork();


	if(pid < 0 ) { //error ocurred

		fprintf(stderr, "Fork Failed");

	}

	else if(pid == 0){ //child process

		//Makes the stdin descriptor points to infile.
		if(infile[0] != '\0'){
			if ( dup2(fd1, 0) == -1) {
			    printf("It was not possible to copy the file descriptor\n");
				exit(1);
			}
	 	}

		if(execvp(cmd1[0], cmd1) == -1 ) exit(1);
			
	}

	else { //parent process

		//parent will wait for the child to complete
		wait(NULL);
	}

	// restore stdout from saved values
    if ( dup2(fdw, STDOUT_FILENO) == -1) {
        printf("It was not possible to copy the file descriptor\n");
		exit(1); 
    }

 
	// save a copy of stdin in entry fdr
    // you may want to check the fdr value
    fdr=dup(STDIN_FILENO);
    if (fdr == -1){
        printf("It was not possible to copy the file descriptor\n");
		exit(1); 
    }

    
    // make read end of pipe as stdin
    if ( dup2(pipefd[0], STDIN_FILENO ) == -1) {
        printf("It was not possible to copy the file descriptor\n");
		exit(1); 
    }

    //this line is essential, without it, the code still waiting for an input
	close(pipefd[1]);

	//fork a child process
	pid = fork();

	if(pid < 0 ) { //error ocurred

		fprintf(stderr, "Fork Failed");

	}

	else if(pid == 0){ //child process
		//Makes the stdout descriptor points to outfile.
		if(outfile[0] != '\0'){
			if ( dup2(fd, 1) == -1) {
			    printf("It was not possible to copy the file descriptor\n");
				exit(1);
			}
		}
		if(execvp(cmd2[0], cmd2) == -1 ) exit(1);
	}

	else { //parent process
		//parent will wait for the child to complete
		wait(NULL);
	}

	// restore stdin and stdout from saved values
    if ( dup2(fdr, STDIN_FILENO) == -1) {
        printf("It was not possible to copy the file descriptor\n");
		exit(1); 
    }

	// closing up
    close(fdw);
    close(fdr);
    close(pipefd[0]);
    close(fd);
    close(fd1);

}


  