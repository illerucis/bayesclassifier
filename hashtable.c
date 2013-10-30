#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* A Hash Table implementation from K&R */

#define HASHSIZE 101
static struct nlist* hashtab[HASHSIZE];

// nodes stored in the hashtab array
struct nlist {
    struct nlist *next;
    char *key;
    char *val;
};

// primitive hashing function
unsigned hash(char *s)
{
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++)
	hashval = *s + 31 * hashval;
    return hashval % HASHSIZE;
}

// finds the nlist node in the hashtable
struct nlist *lookup(char *s)
{
    struct nlist *np;
    for (np = hashtab[hash(s)]; np != NULL; np = np->next)
	if (strcmp(s, np->key) == 0)
	    return np;
    return NULL;
}

struct nlist *install(char *ikey, char *ival)
{
    struct nlist *np;
    unsigned hashval;
    
    // if not already in the hash table
    if ((np = lookup(ikey)) == NULL) {

	// allocate memory for the node
	np = (struct nlist *) malloc(sizeof(*np));

	// either no more memory, or ikey is NULL
	if (np == NULL || (np->key = strdup(ikey)) == NULL)
	    return NULL;

	// create a hash for the incoming key
	hashval = hash(ikey);

	// the node's next points to its own value in the hash table
	np->next = hashtab[hashval];

	// order is to prevent np from pointing to itself
	hashtab[hashval] = np;
    }    
    else // if there is already a value for this key's hash 
	free((void *) np->val);

    // set the value
    if ((np->val = strdup(ival)) == NULL)
	return NULL;
    
    return np;

}

int main() 
{    
    // store the value
    install("A", "ASDF");
    struct nlist *node = lookup("A");
    char *val = node->val;
    printf("%s\n", val);
    return 0;
}
