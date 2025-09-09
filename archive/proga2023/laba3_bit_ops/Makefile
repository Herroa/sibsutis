main: main.o coder.o command.o
	gcc -Wall -o main main.o command.o coder.o

main.o: src/main.c
	gcc -Wall -c src/main.c

command.o: src/command.c
	gcc -Wall -c src/command.c

coder.o: src/coder.c
	gcc -Wall -c src/coder.c



encode: main 
	./main encode src/points.txt units.bin
	hexdump -C units.bin

decode: main
	./main decode units.bin points.txt

clean:
	rm -rf main units.bin points.txt *.o