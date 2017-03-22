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
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include "random.h"

#define NUM_PHILO 5
#define REQ_TIME_TO_EAT 100
int chopsticks;


void EatOrThink(int i){
    srand(i); // Set seed for rand()

    int time_eating = 0; // Total time eating
    int time_thinking = 0; // Total time thinking

    while(time_eating < REQ_TIME_TO_EAT){

        int time_to_think = randomGaussian(11, 7); // 11 , 7
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

        int operation = semop(chopsticks, take, 2);

        if (operation == 0) {

            time_to_eat = randomGaussian(9, 3); // 9 , 3
            if (time_to_eat < 0){
                time_to_eat = 0;
            }
            printf("Philosopher %i eating for %i seconds (total %i)\n", 
                i, time_to_eat, time_eating);
            sleep(time_to_eat);
            time_eating += time_to_eat;
            if(semop(chopsticks, drop, 2) == -1){
                fprintf(stderr, "Error trying to drop chopsticks for philo %d\n", i);
                exit(1);
            }
        }
        if (operation == -1){
                fprintf(stderr, "Error trying to pickup chopsticks for philo %d\n",i);
                exit(1);
            }
    }
    printf("Philosopher %i ate for a total of %i seconds and thought for %i seconds\n", i, time_eating, time_thinking);
}

void philo(){

    pid_t pid[NUM_PHILO];
   
    if((chopsticks = semget(IPC_PRIVATE, NUM_PHILO, IPC_CREAT | 0666)) == -1){ // Create semaphores
        fprintf(stderr, "ERROR\n");
        exit(1);
    }
    for(int i = 0; i < NUM_PHILO; i++){
        if((semctl(chopsticks, i, SETVAL, 1)) == -1){  //Go through and set the valud of the chopstick to 1
            fprintf(stderr, "ERROR\n");
            exit(1);
        }
    }
    for (int i = 0; i < NUM_PHILO; ++i){ // Fork all the philosopher children
        pid_t childID = fork();
        printf("Forked Philosopher %i (process %d)\n", i, childID);
        pid[i] = childID;
        if (pid[i] < 0){
            printf("Error\n");
            exit(1);
        }else if (pid[i] == 0){
            EatOrThink(i);
            printf("------------------------------------\n");
            printf("Philosopher %i is leaving the table\n", i);
            printf("------------------------------------\n");
            exit(1);
        }        
    }
    for (int i = 0; i < NUM_PHILO; ++i){ // Wait on all philosophers to finish
        wait(NULL);
    }
    
    if(semctl(chopsticks, NUM_PHILO, IPC_RMID) == -1){ //Clean up and check to make sure it didn't fail
        fprintf(stderr, "Error trying to clean semephore\n");
        exit(1);
    }
}

int main(int argc, char* argv[]){
    philo();
    return 0;
}
