/* 
 * File:   alarm_basic.c
 * Author: wingning
 *
 * Created on April 16, 2013, 4:08 PM
 * For teaching csce3613 operating systems
 * Some of the code and logic was adopted from Programming with POSIX Threads by
 * David R. Butenhof
 */

#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

    while ( (seconds = getInput(message) )) {
        sleep (seconds);
        printf ("\a");
        printf ("(%d) %s\n", seconds, message);
    }
    return (EXIT_SUCCESS);
}

