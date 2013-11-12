#ifndef CLASSIFIER_H_INCLUDED
#define CLASSIFIER_H_INCLUDED

struct class {
    struct rarray **tdata;
    double **stats;
    double prob;
};

struct bclassifier {
    struct hashtable *classes;
    char **classnames;
    int nvars;
};

struct bclassifier *getclassifier();
void destroyclassifier(struct bclassifier *b);

void train(struct bclassifier *b, char filename[]);
int classify(struct bclassifier *b, double input[]);

#endif
