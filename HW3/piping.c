/* 
 * File:   piping.c
 * Author: wingning
 *
 * Created on March 5, 2013, 10:36 AM
 * Basic example illustrates file descriptor
 * and file descriptor table and how we may
 * use them and dup2 to achieve I/O redirection
 * as being done in the bash shell
 * Modified on February 27, 2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h> // for SSIZE_MAX

/*
 *  piping example
 *  a pipe has two descriptors: the read end and the write end
 *  os or kernel makes sure the two ends are synchronized.
 *  reading and writing to a pipe is similar to reading and writing
 *  to a file as the basic underlining structure is based on
 *  file descriptors and file systems.
 *
 *  When two processes share a pipe, one is reading and the other
 *  reading, we get the effect of a bash or unix or linux user level pipe
 *  e.g. ls | wc
 */

int piping(){

    int pipefd[2]; // return value from the pipe system call
    int fdw, fdr;
    char buf[80];

    // making a pipe
    if ( pipe(pipefd) == -1 ) {
        //error condition
    }
    // pipe[1] is the write end of the pipe
    // pipe[0] is the read end of the pipe

    // save a copy of stdout in entry fdw
    // you may want to check the fdw value
    fdw=dup(STDOUT_FILENO);
    if (fdw == -1){
        // error
    }
    printf("Hello-one\n"); // should go to stdout

    // changing descriptor table, making write end of pipe as stdout
    if ( dup2(pipefd[1], STDOUT_FILENO) == -1) {
        //error
    };
    // from now on wrting to stdout means writing to write end of the pipe
    printf("Hello-two\n"); // write to the pipe

    // save a copy of stdin in entry fdr
    // you may want to check the fdr value
    fdr=dup(STDIN_FILENO);
    if (fdr == -1){
        // error
    }

    // make read end of pipe as stdin
    if ( dup2(pipefd[0], STDIN_FILENO ) == -1) {
        //error
    };
    scanf("%s",buf); // read from the pipe

    // restore stdin and stdout from saved values
    if ( dup2(fdw, STDOUT_FILENO) == -1) {
        //error
    };
    if ( dup2(fdr, STDIN_FILENO) == -1) {
        //error
    };

    printf("Hello-three\n"); // stdout
    printf("Print what is read from the pipe: ");
    printf("%s\n", buf);

    // closing up
    close(fdw);
    close(fdr);
    close(pipefd[0]);
    close(pipefd[1]);

    return 0;
}

int main(int argc, char** argv) {   
    piping();
    return (EXIT_SUCCESS);
}

