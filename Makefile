all:
	gcc -c -o narecord.o narecord.c
	gcc -o narecord narecord.o -lasound
