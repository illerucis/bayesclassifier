#include <math.h>

struct bclassifier {
    double *tdata;
    double *stats;
    int ngroups;
    int nvars;
};

double getmean(double stats[])
{
    double sum = 0.0;
    for (int i = 0; i < tss; i++)
	sum += stats[i];
    return sum / tss;
}

double getvariance(double stats[])
{
    double var = 0.0;
    double mean = getmean(stats);
    for (int i = 0; i < tss; i++) 
	var += (stats[i] - mean) * (stats[i] - mean);
    return ( 1 / ( tss - 1.0 ) ) * var; 
}

double getprob(double mean, double var, double val)
{
    double a = 1 / (sqrt(2*M_PI*var));
    double b = ( - (val - mean)*(val - mean) / (2*var) );
    return a*exp(b);
}

struct bclassifier train(struct bclassifier b)
{

    int ngroups = b->ngroups;
    int nvars = b->nvars;

    double stats[ngroups][nvars][2];
    double *tdata = b->tdata;

    double stats[ngroups][nvars][2];
    for (int g = 0; g < ngroups; g++) {
	double *garr = *(tdata + g);
	for (int v = 0; v < nvars; v++) {
	    stats[g][v][0] = getmean(*(garr + v));
	    stats[g][v][1] = getvariance(*(garr + v));
	}
    }

    b->stats = stats;
    return b;

}

int classify(struct bclassifier, double input[])
{
    int ngroups = b->ngroups;
    int nvars = b->nvars;

    double cprobs[ngroups];    
    double cpmax = 0.0;
    double cp;

    int gmax = -1;

    // find the greatest numerator
    for (int g = 0; g < ngroups; g++) {
	cp = 1.0;
	double *garr = *(tdata + g);
	for (int v = 0; v < nvars; v++) {
	    double *varr = *(garr + v);
	    cp = e*getprob(*(varr), *(varr + 1), input[v]);
	}
	if (cp > cpmax) {
	    cpmax = cp;
	    gmax = g;
	}
    }
    return gmax;
}


