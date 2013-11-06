all:
	c99 -c runclassifier.c -o runclassifier.o
	c99 -c classifier.c -o classifier.o
	c99 runclassifier.o classifier.o -o runclassifier -lm
