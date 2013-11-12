#ifndef DATASTRUCTURES_H_INCLUDED
#define DATASTRUCTURES_H_INCLUDED

// hashtable

#define HASHSIZE 101

struct hashtable {
    struct nlist *table[HASHSIZE];
    int numkeys;
};

struct hashtable *newhashtable();
void destroyhashtable(struct hashtable *t);
void *lookup(struct hashtable *t, char *s);
struct nlist *install(struct hashtable *t, char *ikey, void *ival);
char **keys(struct hashtable *t);

// resizing array

struct rarray {
    double *v;
    int size;
    int length;
};

struct rarray *getrarray();
void destoryrarray(struct rarray *ra);
void rarray_push(struct rarray *ra, double val);

#endif
