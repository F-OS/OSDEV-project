test:
	gcc -c test.c -g -o test.o
	gcc -c font.c -g -o font.o
	gcc test.o font.o -o test
	./test

clean:
	rm -f test.o test font.o
.PHONY: clean test