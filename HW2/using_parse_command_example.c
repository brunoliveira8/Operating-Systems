/* C program illustrates using parsing comamnd line function */


#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#define BUFSIZE 1024
#define CSTRSIZE 100
#define CMDSIZE 30

#define DEBUG 1


int parse_command(char*, char**,char**,char*,char*);


int main (int argc, char* argv[])
{
	/* allocate space for arguments to parse_command */
	char infile[CSTRSIZE];
	char outfile[CSTRSIZE];
	char* cmd1[CMDSIZE];
	char* cmd2[CMDSIZE];
	int i;
	int k;

        cmd1[0]=NULL;
        cmd2[0]=NULL;
	
	if (argc == 2){
		i = parse_command(argv[1], cmd1, cmd2, infile, outfile);
                printf("return code is %d\n", i);
		if ( i < 9) {
                        k=0;
                        while (cmd1[k] != NULL)
                        {
                                printf("cmd1[%d] = %s\n", k, cmd1[k]);
                                k++;
                        };
                        k=0;
                        while (cmd2[k] != NULL)
                        {
                                printf("cmd2[%d] = %s\n", k, cmd2[k]);
                                k++;
                        };
                        if (strlen(infile)){
                            printf("input redirection file name: %s\n", infile);
                        }
                        if (strlen(outfile)){
                            printf("output redirection file name: %s\n", outfile);
                        }

		} else {
			printf("not yet handled\n");
			return 0;
		}
	} else {
		printf("usage error\n");
	}

        
  return 0;
};
