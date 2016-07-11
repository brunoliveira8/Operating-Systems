/*
 * File:   main.c
 * Author: wingning
 *
 * Created on April 16, 2013, 4:08 PM
 * For teaching csce3613 operating systems
 * Some of the code and logic was adopted from Programming with POSIX Threads by
 * David R. Butenhof
 *
 * pthread version that is similar to fork version in concept
 */

#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

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

typedef struct alarm_tag {
    int         seconds;
    char        message[MSGSIZE];
} alarm_t;

void *alarm_thread (void *arg)
{
    alarm_t *alarm = (alarm_t*)arg;
    int status;

    status = pthread_detach (pthread_self ());
    if (status != 0)
        //err_abort (status, "Detach thread");
        ;
    sleep (alarm->seconds);
    printf ("\a");
    printf ("\n(%d) %s\n", alarm->seconds, alarm->message);
    free (alarm);
    return NULL;
}

int main(int argc, char** argv) {
    int seconds;
    char message[MSGSIZE];
    alarm_t *alarm;
    pthread_t thread;
    int status;

    while ( (seconds = getInput(message)) ){
        // making data to share with thread
        alarm = (alarm_t*)malloc (sizeof (alarm_t));
        if (alarm == NULL)
            //errno_abort ("Allocate alarm");
            ;
        alarm -> seconds = seconds;
        strcpy(alarm ->message, message);
        // create and run thread
        status = pthread_create (&thread, NULL, alarm_thread, alarm);
        if (status != 0)
            //err_abort (status, "Create alarm thread");
            ;
    }
    return (EXIT_SUCCESS);
}


