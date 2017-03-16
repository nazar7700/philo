#OPTIMIZE=-03 -DNDEBUG
DEBUGGABLE=-g
CFLAGS = -std=c99 -Wall -pedantic $(DEBUGGABLE) $(OPTIMIZE) -c -g
philo: random.o philo.o
	gcc -pthread random.o philo.o -o philo -lm
philo.o: philo.c random.h
	gcc $(CFLAGS) philo.c

random.o: random.c random.h
	gcc $(CFLAGS) random.c

clean:
	rm -f *.o philo
	
