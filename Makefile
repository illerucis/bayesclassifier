all:
	gcc -c runclassifier.c -o runclassifier.o 
	gcc -c classifier.c -o classifier.o -lm
	gcc runclassifier.o classifier.o -o runclassifier 
