/*
 * File:   alarm_process.c
 * Author: wingning
 *
 * Created on April 16, 2013, 4:08 PM
 * For teaching csce3613 operating systems
 * Some of the code and logic was adopted from Programming with POSIX Threads by
 * David R. Butenhof
 *
 * fork version
 */

#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define LINESIZE 80
#define MSGSIZE 24

/*
 * return 0 if end of file or error
 * return second with message containing a string otherwise
 */
int getInput(char *message){
    int seconds;
    char line[LINESIZE];

    // loop if the line is empty
    while ( 1 ) {
        printf ("Alarm> ");
        if (fgets (line, sizeof (line), stdin) == NULL) return 0;
        if (strlen (line) <= 1) continue;
        /*
         * Parse input line into seconds (%d) and rest of characters upto 24
         * into message (%24[^\n]).
         * Example valid inputs:
         * 2sec. alarm
         * 2 set 2 second alarm
         * Example bad inputs:
         * set 2 second
         * 2
         * must begin with a number
         */
        if (sscanf (line, "%d %24[^\n]", &seconds, message) < 2) {
            fprintf (stderr, "Bad command\n");
            fprintf (stderr, "Type the number of seconds followed by a nonempty string\n");
        } else {
            return seconds;
        }
    }

}
int main(int argc, char** argv) {
    int seconds;
    char message[MSGSIZE];
    pid_t pid;

    while ( (seconds = getInput(message)) ) {
        pid = fork ();
        if (pid == -1)
            //errno_abort ("Fork");
            ;
        if (pid == (pid_t)0) {
            /*
             * If we're in the child, wait and then print a message
             */
            sleep (seconds);
            printf ("\a");
            printf ("\n(%d) %s\n", seconds, message);
            exit (0);
        } else {
            /*
             * In the parent, call waitpid() to collect any children that
             * have already terminated.
             */
            do {
                pid = waitpid ((pid_t)-1, NULL, WNOHANG);
                if (pid == (pid_t)-1)
                    //errno_abort ("Wait for child");
                    ;
            } while (pid != (pid_t)0);
        }
    }
    return (EXIT_SUCCESS);
}

