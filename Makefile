all : compress decompress

compress: Compresseur.o
	gcc -Wall -ansi -pedantic -std=c99 Compresseur.o -o compress
Compresseur.o : Compresseur.c
	gcc -Wall -ansi -pedantic -std=c99 -c Compresseur.c
decompress : Decompresseur.o
	gcc -Wall -ansi -pedantic -std=c99 -g -lm Decompresseur.o -o decompress
Decompresseur.o : Decompresseur.c
	gcc -Wall -ansi -pedantic -std=c99 -c -g -lm Decompresseur.c
clean:
	rm *.o && rm compress && rm decompress
