./bincmp/bincmp: ./bincmp/bincmp.c
	gcc -O2 -o ./bincmp/bincmp ./bincmp/bincmp.c

clean:
	rm ./bincmp/bincmp
