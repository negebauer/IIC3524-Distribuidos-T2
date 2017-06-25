compile: t2/t2.c
	gcc -o t2.o t2/t2.c -I.
	# gcc-7 -lpng -fopenmp -o parallel imagelib/imagelib.c t1/parallel.c -I.
