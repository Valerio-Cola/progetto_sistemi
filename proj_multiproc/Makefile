CFLAGS = -Wall 

all: final clean
	
final: main.o funzioni.o
	@gcc $(CFLAGS) -o final main.o funzioni.o -lm
	@echo "Programma compilato correttamente, eseguire il comando './final' per eseguirlo"
	
main.o: proj_multiprocesso.c funzioni.h
	@gcc $(CFLAGS) -c proj_multiprocesso.c -o main.o

funzioni.o: funzioni.c funzioni.h
	@gcc $(CFLAGS) -c funzioni.c -o funzioni.o

clean: 
	@rm -f *.o
