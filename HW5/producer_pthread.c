/* 
 * File:   producer_pthread.c
 * Author: wingning
 *
 * Created on April 22, 2013, 9:11 AM
 * using pthread to code a simple consumer and producer problem
 * to illustrate mutex and condition variables
 *
 * Various delays are introduced to illustrate waiting by both
 * producer and consumer.
 *
 * Since C does not have bool type, integer is used as logic variables
 *
 */


#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define LINESIZE 80
#define MSGSIZE 24

// simple buffer data
int data;
int empty=1;
int full=0;

// global flag
int done = 0;


pthread_mutex_t data_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty_cond = PTHREAD_COND_INITIALIZER;


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

void *consumer (void *arg)
{
    int status;
    /*
     * Loop forever, processing and consuming
     */
    while (done != 1) {
        status = pthread_mutex_lock (&data_mutex);
        // should check status, error checking code is omitted
        // critical section starts
        while (empty == 1){
            printf("\nConsumer waiting...\n");
            status = pthread_cond_wait(&empty_cond, &data_mutex);
        }

        printf("\nGot the data: %d\n", data);
        // should do the following: but this will not illustrate producer wait.
        //full = 0;
        //empty = 1;
        //status = pthread_cond_signal(&full_cond);

        // critical section endss
        status = pthread_mutex_unlock (&data_mutex);
        // should check status, error checking code is omitted

        // try to make pruducer wait as if we consume the data slowly
        sched_yield ();
        sleep (data*10);

        status = pthread_mutex_lock (&data_mutex);
        // should check status, error checking code is omitted
        // critical section starts

        printf("\nDone consuming data: %d and sending signal\n", data);
        full = 0;
        empty = 1;
        status = pthread_cond_signal(&full_cond);

        // critical section ends
        status = pthread_mutex_unlock (&data_mutex);
        // should check status, error checking code is omitted
        
    }
    return NULL;
}

/*
 * 
 */
int main(int argc, char** argv) {
    int seconds;
    char message[MSGSIZE];
    pthread_t thread;
    int status;
    void *result;

    status = pthread_create (&thread, NULL, consumer, NULL);
    // should check status, error checking code is omitted
    /*
     * Loop as long as user does not type control d, processing and producing
     */
    while ( 1 ){
        // making data and sharing with consumer thread

        // slow down the next prompt
        sleep(3);
        if ( (seconds = getInput(message)) == 0){
            done = 1; // done is shared, should have handled it a CS
            break;
        }
        
        status = pthread_mutex_lock (&data_mutex);
        // should check status, error checking code is omitted
        // critical section begins
        
        while (full == 1){
            printf("\nProducer waiting...\n");
            status = pthread_cond_wait(&full_cond, &data_mutex);
        }
        // at this point, the data buffer is available to store and a signal from the consumer has been received

        data = seconds;
        printf("\nDone producing data: %d and sending signal\n", data);
        empty = 0;
        full = 1;
        status = pthread_cond_signal(&empty_cond);

        // critical section ends
        status = pthread_mutex_unlock (&data_mutex);
        // should check status, error checking code is omitted

       
    } // while loop

    // wait for the consumer to finish first
    status = pthread_join(thread, &result);
    // should check status, error checking code is omitted

    printf("\nAll Done ...\n");
    return (EXIT_SUCCESS);
}


