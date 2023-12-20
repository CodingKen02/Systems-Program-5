/*
----------------------------------------------------------
Program: program5.c
Date: September 26, 2023
Student Name & NetID: Kennedy Keyes kfk38
Description: This program opens the input file, "angl.dat", 
to read one set of roll, pitch, and yaw values each second. 
Then, it should verify if each the roll and pitch are inside
the range of -20.0 to 20.0 and report the results. Then closes.
Note: This program has been modified to use signals and interval timers.
----------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <math.h>
#include <signal.h>
#include <sys/time.h>

#define RANGE_LOWER -20.0
#define RANGE_UPPER 20.0

int testError(int val, const char *msg) // function for error handling
{
    if (val == -1)
    {
        perror(msg);
        exit(EXIT_FAILURE);
    }
    return val;
}

void handleTimer(int signo)
{
    if (signo == SIGALRM)
    {
        int fd, rd;
        double roll, pitch, yaw;

        fd = testError(open("angl.dat", O_RDONLY), "open"); // open the file

        // reading and processing the values from the file
        rd = testError(read(fd, &roll, sizeof(double)), "read");
        rd = testError(read(fd, &pitch, sizeof(double)), "read");
        rd = testError(read(fd, &yaw, sizeof(double)), "read");

        if (rd <= 0)
        {
            close(fd);
            exit(EXIT_SUCCESS); // end the program when reading is done
        }

        printf("Roll: %.03lf, Pitch: %.03lf, Yaw: %.03lf\n", roll, pitch, yaw);

        if (roll >= RANGE_LOWER && roll <= RANGE_UPPER && pitch >= RANGE_LOWER && pitch <= RANGE_UPPER)
        {
            printf("Roll: %.03lf, Pitch: %.03lf are inside the range\n", roll, pitch);
        }
        else
        {
            printf("Roll: %.03lf, Pitch: %.03lf are outside the range\n", roll, pitch);
        }

        close(fd);
    }
}

int main(int argc, char *argv[])
{
    struct itimerval timer;
    struct sigaction sa;

    sa.sa_handler = handleTimer;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    testError(sigaction(SIGALRM, &sa, NULL), "sigaction");

    timer.it_interval.tv_sec = 1;
    timer.it_interval.tv_usec = 0;
    timer.it_value = timer.it_interval;

    testError(setitimer(ITIMER_REAL, &timer, NULL), "setitimer");

    while (1)
    {
        pause(); // wait for the timer signal
    }

    exit(EXIT_SUCCESS);
}