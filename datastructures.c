#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define HASHSIZE 101

struct hashtable {
    struct nlist *table[HASHSIZE];
    int numkeys;
};

// nodes stored in the hashtab array
struct nlist {
    struct nlist *next;
    char *key;
    void *val;
};

char **keys(struct hashtable *t) 
{
    char **k = malloc(t->numkeys*sizeof(char *));
    int kn = 0;
    for (int i = 0; i < HASHSIZE; i++) {
        struct nlist *n = t->table[i];
        if (n != NULL && n->key != NULL) {
            k[kn] = n->key;
            kn++;
        }
    }
    return k;
}

// primitive hashing function
unsigned hash(char *s)
{
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++)
	hashval = *s + 31 * hashval;
    return hashval % HASHSIZE;
}

struct hashtable *newhashtable()
{
    struct hashtable *t = malloc(sizeof(struct hashtable));
    t->numkeys = 0;
    return t;
}

void destroyhashtable(struct hashtable *t)
{
    free(t);
}

// finds the nlist node in the hashtable
void *lookup(struct hashtable *t, char *s)
{

    struct nlist *np;
    for (np = t->table[hash(s)]; np != NULL; np = np->next)
	if (strcmp(s, np->key) == 0)
	    return np->val;
    return NULL;
}

// finds the nlist node in the hashtable
void *nlistlookup(struct hashtable *t, char *s)
{
    struct nlist *np;
    for (np = t->table[hash(s)]; np != NULL; np = np->next)
	if (strcmp(s, np->key) == 0)
	    return np;
    return NULL;
}

struct nlist *install(struct hashtable *t, char *ikey, void *ival)
{
    struct nlist *np;
    unsigned hashval;
    
    // if not already in the hash table
    if ((np = nlistlookup(t, ikey)) == NULL) {

        // allocate memory for the node
	np = (struct nlist *) malloc(sizeof(*np));

	// either no more memory, or ikey is NULL
	if (np == NULL || (np->key = strdup(ikey)) == NULL)
	    return NULL;

	// create a hash for the incoming key
	hashval = hash(ikey);

	// the node's next points to its own value in the hash table
	np->next = t->table[hashval];

	// order is to prevent np from pointing to itself
	t->table[hashval] = np;
        t->numkeys++;
    }    
    else // if there is already a value for this key's hash 
	free((void *) np->val);

    // set the value
    np->val = ival;

    return np;

}

struct stack {
    int N;
    struct node {
        void *val;
        struct node *next;
    } *head;
};

int size(struct stack *s) { return s->N; }

int isempty(struct stack *s) { return s->N == 0; }

void push(struct stack *s, void *val)
{
    struct node *newnode = malloc(sizeof(struct node));
    newnode->val = val;
    newnode->next = s->head;
    s->head = newnode;
    int N = s->N;
    N++;
    s->N = N;
}

void *pop(struct stack *s)
{
    struct node *n = s->head;
    s->head = s->head->next;
    int N = s->N;
    N--;
    s->N = N;
    return n->val;
}

void *peek(struct stack *s)
{
    return s->head->val;
}

struct stack *newstack() 
{
    struct stack *s = malloc(sizeof(struct stack));
    s->N = 0;
    return s;
}

void destroystack(struct stack *s)
{
    free(s);
}


// resizing array

struct rarray {
    double *v;
    int size;
    int length;
};

struct rarray *getrarray() 
{

    int INITIAL_LENGTH = 2;

    struct rarray *ra = malloc(sizeof(struct rarray *));
    double *v = malloc(INITIAL_LENGTH*sizeof(double));

    for (int i = 0; i < INITIAL_LENGTH; i++) 
        v[i] = 0.0;
    
    ra->v = v;
    ra->size = 0;
    ra->length = INITIAL_LENGTH;

    return ra;
}

void destoryrarray(struct rarray *ra)
{
    free (ra->v);
    free (ra);
}

void resize(struct rarray *ra, int newlength)
{
    double *nv = malloc(newlength*sizeof(double));
    // copy all the old contents into the newly sized array
    for (int i = 0; i < ra->size; i++) {
        nv[i] = ra->v[i];
    }
    free(ra->v);
    ra->length = newlength;
    ra->v = nv;

}

void rarray_push(struct rarray *ra, double value)
{
    if (ra->size == ( ra->length / 2 )) {
        resize(ra, 2*ra->length);
    }

    ra->v[ra->size] = value;
    ra->size = ra->size + 1;
}
