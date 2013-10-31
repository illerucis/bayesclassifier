#include <stdio.h>
#include <stdlib.h>
#include "classifier.h"

int main() 
{    
    int ngroups = 2;
    int nvars = 3;
    int ssize = 4;

    double training[] = {
	// male height
	6.00, 5.92, 5.58, 5.92,
	// male weight
	180, 190, 170, 165,
	// male shoe size
	12, 11, 12, 10,

	// female height
	5.00, 5.50, 5.42, 5.75,
	// female weight
	150, 100, 130, 150,
	// female shoe size
	6, 8, 7, 9
    };

    struct bclassifier *b;
    b = getclassifier(training, ngroups, nvars, ssize);

    printf("%d\n", b->ngroups);
    /* printf("%d\n", b->nvars); */
    /* printf("%d\n", b->ssize); */

    return 1;
}

