//Ashpan Raskar - 400185326 - raskara
//Matt Lemcke - 400181639 - lemckem

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
// if sigalarm is called, print "Alarm"
void alarm_handler ( int signo ){
    if ( signo == SIGALRM ){
        printf ("Alarm\n");
    }
}

//If sigint is called (via CTRL+C) print CTRL+C pressed!
void sigint_handler ( int signo ){
    if ( signo == SIGINT ){
        printf ("CTRL+C pressed!\n");
    }
}

//If sigint is called (via CTRL+Z) print CTRL+Z pressed! then exit the program
void sigtstp_handler ( int signo ){
    if ( signo == SIGTSTP ){
        printf ("CTRL+Z pressed!\n");
        exit(1);
    }
}

int main ( void ){
// register the signal alarm handler
    if ( signal ( SIGALRM , alarm_handler ) == SIG_ERR ){
        printf ("failed to register alarm handler.") ;
        exit (1);
    }
// register the interrupt signal handler
    if ( signal ( SIGINT , sigint_handler ) == SIG_ERR ){
        printf ("failed to register sigint handler.") ;
        exit (1);
    }

    // register the terminal stop signal handler
    if ( signal ( SIGTSTP , sigtstp_handler ) == SIG_ERR ){
        printf ("failed to register sigstsp handler.") ;
        exit (1);
    }
    while (1) {
        alarm(2); //Call alarm every 2 seconds
        sleep(10); //sleep for 10 seconds
    }
}
