all:
	gcc -std=c99 -c runclassifier.c -o runclassifier.o
	gcc -std=c99 -c classifier.c -o classifier.o
	gcc -std=c99 runclassifier.o classifier.o -o runclassifier
