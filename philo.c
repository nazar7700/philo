// Nazar Stelmakh
// Assignment 6 - Philosopher Problem
// 23 March 2017
#define _POSIX_SOURCE
#define _XOPEN_SOURCE
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdbool.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>

#include "random.h"

#define NUM_PHILO 5
int chopsticks[NUM_PHILO];


void EatOrThink(int i, pid_t pid){

    //printf("Philosopher %i starting (process %d)\n", i, pid);

    int time_eating = 0;
    int time_thinking = 0;

    while(time_eating < 100){

        int time_to_think = randomGaussian(5, 3); // 11 , 7
        if (time_to_think < 0){
            time_to_think = 0;
        }
        printf("Philosopher %i thinking for %i seconds (total %i)\n",
            i,time_to_think, time_thinking);
        sleep(time_to_think);
        time_thinking += time_to_think;

        int left = i;
        int right = (i + 1) % NUM_PHILO;

        printf("Philosopher %i checking for chopsticks %i and %i\n",
            i,left,right );

        struct sembuf take[2];
        take[0].sem_num = left;
        take[0].sem_op = -1;
        take[0].sem_flg = 0;
        take[1].sem_num = right;
        take[1].sem_op = -1;
        take[1].sem_flg = 0;
        struct sembuf drop[2];
        drop[0].sem_num = left;
        drop[0].sem_op = 1;
        drop[0].sem_flg = 0;
        drop[1].sem_num = right;
        drop[1].sem_op = 1;
        drop[1].sem_flg = 0;        

        int time_to_eat = 0;
        semop(chopsticks[0], drop, 2);
        while((semop(chopsticks[0], take , 2) == -1)){
            time_to_eat = randomGaussian(9, 3); // 9 , 3
            if (time_to_eat < 0){
                time_to_eat = 0;
            }
            printf("Philosopher %i eating for %i seconds (total %i)\n", 
                i, time_to_eat, time_eating);
            sleep(time_to_eat);
            time_eating += time_to_eat;

        }

    }

    //sem_post(sem_t *sem) ==> unlocks the semaphore
    //sem_wait(sem_t *sem) ==> locks the sempaphore


}

void philo(){

    pid_t pid[NUM_PHILO];

    for(int i=0; i<NUM_PHILO;i++){
        if((chopsticks[i] = semget(IPC_PRIVATE, 1, S_IRUSR | S_IWUSR)) != 0){

        }
        
    }

    for (int i = 0; i < NUM_PHILO; ++i){
        pid_t childID = fork();
        //printf("Forked Philosopher %i (process %d)\n", i, childID);
        pid[i] = childID;
        if (pid[i] < 0){
            printf("Error\n");
            exit(1);
        }else if (pid[i] == 0){
            //printf("Child (%d): %d\n", i, getpid());
            EatOrThink(i, childID);
            printf("Philosopher %i is leaving the table\n", i);
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