/*
 * File:   alarm_mutex.c
 * Author: wingning
 *
 * Created on April 16, 2013, 4:08 PM
 * For teaching csce3613 operating systems
 * Some of the code and logic was adopted from Programming with POSIX Threads by
 * David R. Butenhof
 *
 * pthread version that uses only two threads, a server and a client alarm.
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

/*
 * The "alarm" structure now contains the time_t (time since the
 * Epoch, in seconds) for each alarm, so that they can be
 * sorted. Storing the requested number of seconds would not be
 * enough, since the "alarm thread" cannot tell how long it has
 * been on the list.
 *
 * linked list node type
 *
 */
typedef struct alarm_tag {
    int                 seconds;
    char                message[MSGSIZE];
    time_t              time;   /* seconds from EPOCH */
     struct alarm_tag    *link;
} alarm_t;

pthread_mutex_t alarm_mutex = PTHREAD_MUTEX_INITIALIZER;
alarm_t *alarm_list = NULL;

//Add empty_condition
pthread_cond_t empty_cond = PTHREAD_COND_INITIALIZER;




/*
 * The alarm client thread routine.
 */
void *alarm_thread (void *arg)
{
    alarm_t *alarm;
    int sleep_time;
    time_t now;
    int status;

    struct timespec timeToWait;
    timeToWait.tv_sec = 1;


    /*
     * Loop forever, processing commands. The alarm thread will
     * be disintegrated when the process exits.
     */
    while (1) {
        status = pthread_mutex_lock (&alarm_mutex);
        // critical section starts
        if (status != 0)
            //err_abort (status, "Lock mutex");
            ;
        // alarm_list is global variable pointing to the first node of list
        alarm = alarm_list;

        /*
         * If the alarm list is empty, wait for one second. This
         * allows the main thread to run, and read another
         * command. If the list is not empty, remove the first
         * item. Compute the number of seconds to wait -- if the
         * result is less than 0 (the time has passed), then set
         * the sleep_time to 0.
         */
        if (alarm == NULL){
            pthread_cond_timedwait(&empty_cond, &alarm_mutex, &timeToWait);
            sleep_time = 1;
        }
        else {
            alarm_list = alarm->link;
            now = time (NULL);
            if (alarm->time <= now)
                sleep_time = 0;
            else
                sleep_time = alarm->time - now;
            }
        // critical section ends
        status = pthread_mutex_unlock (&alarm_mutex);
        if (status != 0)
            //err_abort (status, "Unlock mutex");
            ;

        if (sleep_time > 0)
            sleep (sleep_time);
        else
            sched_yield (); // system call

        /*
         * If a timer expired, print the message and free the
         * structure.
         */
        if (alarm != NULL) {
            printf ("\a");
            printf ("(%d) %s\n", alarm->seconds, alarm->message);
            free (alarm);
        }
    }
}


int main(int argc, char** argv) {
    int seconds;
    char message[MSGSIZE];
    alarm_t *alarm, **last, *next;
    pthread_t thread;
    int status;

    status = pthread_create (&thread, NULL, alarm_thread, NULL);
    if (status != 0)
        //err_abort (status, "Create alarm thread");
        ;

    while ( (seconds = getInput(message)) ){
        // making data to share with thread
        alarm = (alarm_t*)malloc (sizeof (alarm_t));
        if (alarm == NULL)
            //errno_abort ("Allocate alarm");
            ;
        alarm -> seconds = seconds;
        strcpy(alarm ->message, message);
        alarm->time = time (NULL) + alarm->seconds;

        status = pthread_mutex_lock (&alarm_mutex);
        if (status != 0)
            //err_abort (status, "Lock mutex");
            ;
        // critical section starts
        /*
         * Insert the new alarm into the list of alarms,
         * sorted by expiration time.
         */
        last = &alarm_list;
        next = *last;
        while (next != NULL) {
            if (next->time >= alarm->time) {
                alarm->link = next;
                *last = alarm;
                break;
            }
            last = &next->link;
            next = next->link;
        }
        /*
         * insert at the end if next is NULL.
         */
        if (next == NULL) {
            *last = alarm;
            alarm->link = NULL;
        }
        
        pthread_cond_signal(&empty_cond);
        // critical section ends
        status = pthread_mutex_unlock (&alarm_mutex);
        if (status != 0)
            //err_abort (status, "Unlock mutex");
            ;
    } // while loop

    return (EXIT_SUCCESS);
}




