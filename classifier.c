#include <stdlib.h>
#include <math.h>

struct bclassifier {
    int ngroups;
    int nvars;
    int nsamples;

    double *tdata;
    double *stats;

};

struct bclassifier *getclassifier(double tdata[], int ngroups, int nvars, int nsamples)
{
    struct bclassifier *b = malloc(sizeof(struct bclassifier));
    double *atdata = malloc(ngroups*nvars*nsamples*sizeof(double));

    atdata = tdata;

    b->ngroups = ngroups;
    b->nvars = nvars;
    b->nsamples = nsamples;
    b->tdata = atdata;

    return b;
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
    double a = 1 / (sqrt(2*M_PI*var));
    double b = ( - (val - mean)*(val - mean) / (2*var) );
    return a*exp(b);
}

int tdataindex(int g, int v, int vmax, int smax) 
{
    return g*vmax*smax + v*smax;
}

int statsindex(int g, int v, int vmax)
{
    return tdataindex(g, v, vmax, 2);
}

void train(struct bclassifier *b)
{

    int ngroups = b->ngroups;
    int nvars = b->nvars;
    int nsamples = b->nsamples;
    
    double *stats = malloc(ngroups*nvars*2*sizeof(double));
    double *tdata = b->tdata;

    int ti; int si;
    double training[nsamples];

    for (int g = 0; g < ngroups; g++) {
	for (int v = 0; v < nvars; v++) {

	    ti = tdataindex(g, v, nvars, nsamples);
	    for (int i = ti; i < nsamples; i++)
		training[i] = *(tdata + i);

	    si = statsindex(g, v, nvars);
	    stats[si] = getmean(training, nsamples);
	    stats[si + 1] = getvariance(training, nsamples);
	}
    }

    b->stats = stats;
}

int classify(struct bclassifier *b, double input[])
{
    int ngroups = b->ngroups;
    int nvars = b->nvars;

    double *tdata = b->tdata;
    double cprobs[ngroups];    
    double cpmax = 0.0;
    double cp;

    int gmax = -1;

    // find the greatest numerator
    for (int g = 0; g < ngroups; g++) {
	cp = 1.0;
	double *garr = tdata + g;
	for (int v = 0; v < nvars; v++) {
	    double *varr = garr + v;
	    cp = cp*getprob(*(varr), *(varr + 1), input[v]);
	}
	if (cp > cpmax) {
	    cpmax = cp;
	    gmax = g;
	}
    }

    return gmax;

}

int main() 
{
    return 1;
}
