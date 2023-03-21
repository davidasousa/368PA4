test: 
	gcc -std=c99 -pedantic -Wvla -Wall -Wshadow -o3 *.c -o pa4

valgrind: pa4
	valgrind ./pa4
