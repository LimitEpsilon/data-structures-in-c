test: test.o hash.o utils.o
	gcc -o test utils.o hash.o test.o

test.o: test.c
	gcc -c test.c

hash.o: src/hash.c
	gcc -c src/hash.c

utils.o: src/utils.c
	gcc -c src/utils.c

clean:
	rm *.o
	rm test 
