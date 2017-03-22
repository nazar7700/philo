Name: Nazar Stelmakh
Email: nazar.stelmakh@wsu.edu

Description: The following program is a representation of the dining
philosopher problem. It forks into multiple processes (philosophers)
and each one will take turns picking up and dropping chopsticks (semephores)
in order to "eat". There are 2 global variables if the user has the 
need to change the number of philosophers (default 5) or the time 
to eat (default 100 sec).

Run/Compile: There is a makefile included.
bash$ make
bash$ ./philo

Archive:
philo.c...................................Main program
random.c..................................Random Gaussian implementation
random.h..................................Header file for random.c
makefile..................................Compiler
README.txt................................This file


