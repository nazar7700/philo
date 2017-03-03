#OPTIMIZE=-03 -DNDEBUG
DEBUGGABLE=-g
CFLAGS = -std=c99 -Wall -pedantic $(DEBUGGABLE) $(OPTIMIZE) -c -g
philo: philo.o
	gcc philo.o -o philo
philo.o:
	gcc $(CFLAGS) philo.c
clean:
	rm -f *.o philo
	
