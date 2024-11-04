run: main
	./main
	
main: main.o IntVector.o
	gcc -Wall main.o IntVector.o -o main

main.o: src/main.c
	gcc -Wall -c src/main.c

IntVector.o: src/IntVector.c
	gcc -Wall -c src/IntVector.c

clean:
	rm -rf *.o main