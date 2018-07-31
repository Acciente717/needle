./bincmp/bincmp: ./bincmp/bincmp.cpp
	g++ -O2 -o ./bincmp/bincmp ./bincmp/bincmp.cpp

clean:
	rm ./bincmp/bincmp
