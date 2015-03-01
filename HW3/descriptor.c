/* 
 * File:   descriptor.c
 * Author: wingning
 *
 * Created on March 5, 2013, 10:36 AM
 * Basic example illustrates file descriptor
 * and file descriptor table and how we may
 * use them and dup2 to achieve I/O redirection
 * as being done in the bash shell
 * Modified Feb. 27 2015
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h> // for SSIZE_MAX

/*
 *  file descriptor and I/O redirection example
 */
int descriptor(){
    
    int fd, fd1;
    
    printf("Hello\n");
    printf("STDIN_FILENO VALUE = %d\n", STDIN_FILENO);
    printf("STDOUT_FILENO VALUE = %d\n", STDOUT_FILENO);
    printf("STDERR_FILENO VALUE = %d\n", STDERR_FILENO);
    if ( (fd = open("foo.txt", O_WRONLY | O_CREAT | O_TRUNC,  S_IRUSR | S_IWUSR)) == -1 ){
        
    }
    printf("foo.txt file descriptor VALUE = %d\n", fd);
    if ( (fd1 = dup(1)) == -1 ) {
        // error
    };
    printf("The file descriptor table entry index by fd1 (fd1 = %d) has the  same value as stdout descriptor VALUE = %d\n", fd1, fileno(stdout));
    // output goes to foo.txt
    // the file descriptor for foo.txt is returned by open system call
    // and is stored in fd
    // the dup2 system call copies the entry of descriptor table
    // referred to by fd to the entry referred to by stdout, which has value 1
    if ( dup2(fd, 1) == -1) {
        //error
    };
    // after the above statement, descriptor table entry 1 is the same
    // as that of the entry 3 or fd.
    printf("Hello\n");
    printf("STDIN_FILENO VALUE = %d\n", STDIN_FILENO);
    printf("STDOUT_FILENO VALUE = %d\n", STDOUT_FILENO);
    printf("STDERR_FILENO VALUE = %d\n", STDERR_FILENO);
    // restore, output goes to stdout
    if ( dup2(fd1, 1) == -1 ){
        // error
    };
    printf("Hello\n");
    printf("STDIN_FILENO VALUE = %d\n", STDIN_FILENO);
    printf("STDOUT_FILENO VALUE = %d\n", STDOUT_FILENO);
    printf("STDERR_FILENO VALUE = %d\n", STDERR_FILENO);
    // should close the file once we finish using it, so these file
    // descriptors may be reused.
    close(fd);
    close(fd1);

    return 0;

}

int main(int argc, char** argv) {
    descriptor();
    return 0;
}