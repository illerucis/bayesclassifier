#include <stdio.h>
#include <stdlib.h>
#include "classifier.h"

int main() 
{    

    struct bclassifier *b;
    b = getclassifier();
    train(b, "gendertrainingdata.txt");

    double test[] = {6.0, 130.0, 8.0};
    printf("%d\n", classify(b, test));
    
    return 1;
}

