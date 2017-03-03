// Nazar Stelmakh
// Assignment 6 - Philosopher Problem
// 23 March 2017
#define _XOPEN_SOURCE
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "random.h"

#define NUM_PHILO 5


void philo(){

    pid_t pid[NUM_PHILO];

    for (int i = 0; i < NUM_PHILO; ++i){
        pid[i] = fork();
        if (pid[i] < 0){
            printf("Error\n");
            exit(1);
        }else if (pid[i] == 0){
            printf("Child (%d): %d\n", i + 1, getpid());
            //call something that behaves like a philosopher
            exit(1);
        }        
    }
    for (int i = 0; i < NUM_PHILO; ++i){
        wait(NULL);
    }
}


int main(int argc, char* argv[]){

    philo();

    return 0;
}