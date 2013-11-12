#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "datastructures.h"

const double PI = 3.14159265358979323846;

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

int getnumvars(char line[])
{
    int i = 0; int vars = 0;
    while (line[i] != '\0') {
        if (line[i] == ',') 
            vars++; 
        i++;
    }
    return vars;
}

void installrarrays(struct hashtable *t, char *key, int vars)
{
    struct rarray **s = (struct rarray **) malloc(vars*sizeof(struct rarray *));
    struct class *c = malloc(sizeof(struct class));

    for (int i = 0; i < vars; i++)
        s[i] = getrarray();
    
    c->tdata = s;
    c->prob = 1;

    install(t, key, c);
}

void getdatafromfile(struct bclassifier *b, char filename[])
{
    FILE *fp = NULL; 
    char *key = NULL;
    
    char line[256];
    fp = fopen(filename, "r");

    // first line to find the number of variables passed in
    fgets(line, 256, fp);
    b->nvars = getnumvars(line);

    int cvar = 0; int totsamples = 0;
    struct hashtable *t = newhashtable();

    while (1) {
        char *pch = strtok(line, ",");
        while (pch != NULL) {
            if (cvar == 0) {
                key = pch;
                if (lookup(t, key) == NULL) 
                    installrarrays(t, key, b->nvars);
                else {
                    struct class *c = (struct class *) lookup(t, key);
                    c->prob += 1;
                }
            }
            else {
                struct class *c = (struct class *) lookup(t, key);
                struct rarray *s = c->tdata[cvar - 1];
                rarray_push(s, atof(pch));
            }            
            pch = strtok(NULL, ","); 
            cvar++;
        }
        cvar = 0; 
        totsamples++;
        if (fgets(line, 80, fp) == NULL) break;
    }

    b->classnames = keys(t);
    b->classes = t;

    for (int i = 0; i < b->classes->numkeys; i++) {
        struct class *c = (struct class *) lookup(t, b->classnames[i]);
        c->prob = (c->prob) / totsamples;
    }

}

struct bclassifier *getclassifier()
{
    struct bclassifier *b = malloc(sizeof(struct bclassifier));
    b->classes = newhashtable();
    return b;
}

void destroyclassifier(struct bclassifier *b)
{
    free (b);
}

double getmean(double stats[], int nsamples)
{
    double sum = 0.0;
    for (int i = 0; i < nsamples; i++)
        sum += stats[i];
    return sum / nsamples;
}

double getvariance(double stats[], int nsamples)
{
    double var = 0.0;
    double mean = getmean(stats, nsamples);
    for (int i = 0; i < nsamples; i++) 
        var += (stats[i] - mean) * (stats[i] - mean);
    return ( 1 / ( nsamples - 1.0 ) ) * var; 
}

double getprob(double mean, double var, double val)
{
    double a = 1 / (sqrt(2*PI*var));
    double b = ( - (val - mean)*(val - mean) / (2*var) );
    return a*exp(b);
}

void train(struct bclassifier *b, char filename[])
{
    getdatafromfile(b, filename);

    struct hashtable *t = b->classes;
    char **classnames = b->classnames;

    for (int k = 0; k < t->numkeys; k++) {
        struct class *c = (struct class *) lookup(t, classnames[k]);
        double **stats = malloc(b->nvars*sizeof(double *));

        for (int v = 0; v < b->nvars; v++) {
            struct rarray *tdata = c->tdata[v];
            double *ms = malloc(2*sizeof(double));

            ms[0] = getmean(tdata->v, tdata->size);
            ms[1] = getvariance(tdata->v, tdata->size);
            stats[v] = ms;
            
        }
        c->stats = stats;
    }
}

int classify(struct bclassifier *b, double input[])
{

    double cp, prob;
    double cpmax = 0.0; int gmax = -1; 

    for (int g = 0; g < b->classes->numkeys; g++) {

        struct class *c = (struct class *) lookup(b->classes, b->classnames[g]);
        cp = c->prob;

        for (int v = 0; v < b->nvars; v++)
            cp = cp * getprob(c->stats[v][0], c->stats[v][1], input[v]);

        if (cp > cpmax) {
            cpmax = cp;
            gmax = g;
        }
    }

    return gmax;
}
