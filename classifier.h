#ifndef CLASSIFIER_H_INCLUDED
#define CLASSIFIER_H_INCLUDED

struct bclassifier {
    double *tdata;
    double *stats;
    double *probexisting;

    int ngroups;
    int nvars;
    int nsamples;
};

struct bclassifier *getclassifier();
void train(struct bclassifier *b, char filename[]);
int classify(struct bclassifier *b, double input[]);

#endif
