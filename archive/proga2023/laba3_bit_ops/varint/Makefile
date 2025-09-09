data: compressed.dat uncompressed.dat
	xxd -b uncompressed.dat
	xxd -b compressed.dat

compressed.dat uncompressed.dat: a.out
	./a.out

a.out: varint.c
	gcc -Wall varint.c

clean:
	rm -rf a.out uncompressed.dat compressed.dat