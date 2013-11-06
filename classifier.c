#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#define  M_PI   3.14159265358979323846

struct bclassifier {

    double *tdata;
    double *stats;
    double *probexisting;

    int ngroups;
    int nvars;
    int nsamples;

};

void buildinfrastructure(struct bclassifier *b, int ngroups, 
                         int nvars, int nsamples)
{

    double *tdata; double *stats; double *probexisting;

    b->ngroups = ngroups;
    b->nvars = nvars;
    b->nsamples = nsamples;
            
    probexisting = malloc(ngroups*sizeof(double));
    tdata = malloc(ngroups*nvars*nsamples*(sizeof(double)));
    stats = malloc(ngroups*nsamples*2*(sizeof(double)));

    b->probexisting = probexisting;
    b->tdata = tdata;
    b->stats = stats;

}

void buildgroups(struct bclassifier *b, char filename[])
{

    int ngroups, nvars, nsamples, cgroup, csamp, ti, i;
    char line[256]; 
    int *csamps;

    int linen = 0, coln = 0;
    double *tdata = NULL, *stats= NULL, *probexisting = NULL;

    char *pch = NULL;
    FILE *fp = NULL;

    fp = fopen(filename, "r");
    while (fgets(line, sizeof(line), fp)) {

        pch = strtok(line, ",");
        while (pch != NULL) {

            if (linen == 0) {
                if (coln == 0) ngroups = atoi(pch);
                else if (coln == 1) nvars = atoi(pch);
                else nsamples = atoi(pch);
            }
            else {
                if (coln == 0) cgroup = atoi(pch);
                else if (coln == nvars + 1) probexisting[cgroup] = atof(pch);
                else {
                    ti = cgroup*nvars*nsamples + (coln - 1)*nsamples + csamps[cgroup];
                    tdata[ti] = atof(pch);
                }
            }
            coln++;
            pch = strtok(NULL, ",");
        }

        if (linen == 0) {
            csamps = malloc(ngroups*sizeof(int));
            for (i = 0; i < ngroups; i++) csamps[i] = 0;

            buildinfrastructure(b, ngroups, nvars, nsamples);
            tdata = b->tdata;
            probexisting = b->probexisting;
        }
        else csamps[cgroup] = csamps[cgroup] + 1;

        coln = 0;
        linen++;
    }
    free (csamps);
}

struct bclassifier *getclassifier()
{
    struct bclassifier *b = malloc(sizeof(struct bclassifier));
    return b;
}

void destroyclassifier(struct bclassifier *b)
{
    free (b->tdata);
    free (b->stats);
    free (b->probexisting);
    free (b);
}

double getmean(double stats[], int start, int nsamples)
{
    double sum = 0.0;
    int i;
    for (i = start; i < start + nsamples; i++)
        sum += stats[i];
    return sum / nsamples;
}

double getvariance(double stats[], int start, int nsamples)
{
    double var = 0.0;
    double mean = getmean(stats, start, nsamples);
    int i;
    for (i = start; i < start + nsamples; i++) 
        var += (stats[i] - mean) * (stats[i] - mean);
    return ( 1 / ( nsamples - 1.0 ) ) * var; 
}

double getprob(double mean, double var, double val)
{
    double a = 1 / (sqrt(2*M_PI*var));
    double b = ( - (val - mean)*(val - mean) / (2*var) );
    return a*exp(b);
}

void train(struct bclassifier *b, char filename[])
{
    buildgroups(b, filename);

    int ngroups = b->ngroups;
    int nvars = b->nvars;
    int nsamples = b->nsamples;

    double *tdata = b->tdata;
    double *stats = b->stats;

    int start;
    int g, v;
    for (g = 0; g < ngroups; g++) {

        for (v = 0; v < nvars; v++) {
            
            start = g*nvars*nsamples + v*nsamples;

            double mean = getmean(tdata, start, nsamples);
            double var = getvariance(tdata, start, nsamples);

            stats[g*nvars*2 + v*2] = mean; 
            stats[g*nvars*2 + v*2 + 1] = var; 

        }
    }
    b->stats = stats;
}

int classify(struct bclassifier *b, double input[])
{

    double cp, prob; int start;

    double cpmax = 0.0; int gmax = -1; 

    double *stats = b->stats;
    double *probexisting = b->probexisting;

    int nvars = b->nvars;
    int ngroups = b->ngroups;
    int g, v;
    for (g = 0; g < ngroups; g++) {
        cp = probexisting[g];

        for (v = 0; v < nvars; v++) {
            start = g*nvars*2 + v*2;
            cp = cp*getprob(stats[start], stats[start + 1], input[v]);
        }

        if (cp > cpmax) {
            cpmax = cp; 
            gmax = g;
        }
    }
    return gmax;
}
