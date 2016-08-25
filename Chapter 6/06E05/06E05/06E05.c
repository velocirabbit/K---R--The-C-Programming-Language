/* 06E05.c (p. 145) */
#include <stdio.h>

#define HASHSIZE 101 // must be prime
#define HASHINIT 0
#define HASHA    31  // K&R implementation of string hashing
#define STRLEN 120

typedef struct s_nlist {  // table entry:
    struct s_nlist *next;   // next entry in chain
    char *name;             // defined name
    char *defn;             // replacement text
} nlist;
static nlist *hastab[HASHSIZE];  // pointer table

unsigned hash(char *s);  // hashing function

int main(void) {

}

/**
 * hash: Forms a hash value for string s, to be used as an index value in the
 *   hashtab array. This uses the Rabin-Karp rolling hash with init and a
 *   values as implemented in K&R.
 *
 * Returns: An unsigned int to be used as an index value in the hashtab array.
 */
unsigned hash(char *s) {
    unsigned hashval = HASHINIT;

    for (hashval = 0; *s != '\0'; s++)
        hashval = (*s + HASHA * hashval) % HASHSIZE;
    return hashval;
}