#OPTIMIZE=-03 -DNDEBUG
DEBUGGABLE=-g
CFLAGS = -std=c99 -Wall -pedantic $(DEBUGGABLE) $(OPTIMIZE) -c -g
philo: philo.o random.o
	gcc philo.o -o philo
philo.o: philo.c random.h
	gcc $(CFLAGS) philo.c

random.o: random.c random.h
	gcc -lm $(CFLAGS) random.c 

clean:
	rm -f *.o philo
	
