compile: src/parallel.c src/secuential.c
	gcc -o secuential.o src/secuential.c -I.
	mpicc -o parallel.o src/parallel.c -I.

clean:
	rm secuential.o
	rm parallel.o
