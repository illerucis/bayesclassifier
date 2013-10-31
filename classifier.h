#ifndef CLASSIFIER_H_INCLUDED
#define CLASSIFIER_H_INCLUDED

struct bclassifier {
    int ngroups;
    int nvars;
    int nsamples;

    double *tdata;
    double *stats;
};

struct bclassifier *getclassifier(double tdata[], int ngroups, int nvars, int nsamples);

#endif
