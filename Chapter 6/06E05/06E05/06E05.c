/* 06E05.c (p. 145) */
#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASHSIZE 101 // must be prime
#define HASHINIT 0
#define HASHA    31  // K&R implementation of string hashing
#define STRLEN   120
#define DEFINE   "#define"
#define UNDEF    "#undef"

typedef struct s_nlist {  // table entry:
    struct s_nlist *next;   // next entry in chain
    char *name;             // defined name
    char *defn;             // replacement text
} nlist;
static nlist *hashtab[HASHSIZE];  // pointer table

int getword(char **line, char *w);
unsigned hash(char *s);  // hashing function
nlist *lookup(char *s);  // lookup function
nlist *install(char *name, char *defn);
char *strdup2(char *s);
void undef(char *name);
void trim(char **line);

int main(void) {
    char *line = malloc(sizeof(char[STRLEN]));
    char directive[STRLEN];
    char key[STRLEN];
    char value[STRLEN];

    while (fgets(line, STRLEN, stdin) != NULL) {
        if (line[0] != '#') {
            // Look up a defined key
            getword(&line, key);
            nlist *np;
            if ((np = lookup(key)) == NULL)
                printf("Could not find a key name '%s'\n", key);
            else {
                for (np; np != NULL; np = np->next)
                    if (strcmp(np->name, key) == 0)
                        printf("\t%s\n", np->defn);
            }
        }
        else if (getword(&line, directive) > 0) {
            // Execute a directive
            if (strcmp(directive, DEFINE) == 0) {
                getword(&line, key);
                trim(&line);
                strcpy(value, line);
                install(key, value);
            }
            else if (strcmp(directive, UNDEF) == 0) {

            }
            else {
                printf("ERROR: Unknown directive '%s'\n", directive);
                break;
            }
        }
    }

    return 0;
}

/**
 * getword: Gets the next word from string line and stores it into string w.
 *   Advanced pointer to string line past the word.
 *
 * Returns: The number of characters pulled (excluding null character)
 */
int getword(char **line, char *w) {
    int c = 0;
    trim(line);
    while (isalnum(**line) || **line == '_' || **line == '#') {
        *w++ = *(*line)++;
        c++;
    }
    *w = '\0';
    return c;
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

/**
 * lookup: Look for hash value s in hashtab.
 *
 * Returns: pointer to the nlist item
 */
nlist *lookup(char *s) {
    nlist *np;

    // Standard idiom for walking along a linked list:
    // for (ptr = head; ptr != NULL; ptr = ptr->next)
    for (np = hashtab[hash(s)]; np != NULL; np = np->next)
        if (strcmp(s, np->name) == 0)
            return np;  // found
    return NULL;        // not found
}

/**
 * install: Puts the defined name and its definition into the hashtab
 *
 * Returns: A pointer to the nlist item
 */
nlist *install(char *name, char *defn) {
    nlist *np;
    unsigned hashval;

    if ((np = lookup(name)) == NULL) {  // not found in hashtab
        np = (nlist*)malloc(sizeof(*np));
        if (np == NULL || (np->name = strdup2(name)) == NULL)
            return NULL;
        hashval = hash(name);
        np->next = hashtab[hashval];  // pushes any names with the same hash
        hashtab[hashval] = np;        // value back and takes its place
    }
    else                              // already there
        free((void*)np->defn);  // free previous defn
    if ((np->defn = strdup2(defn)) == NULL)
        return NULL;
    return np;
}

/**
 * strdup: Makes a dupilcate of string s into a new address locaiton
 *
 * Returns: Pointer to duplicate string
 */
char *strdup2(char *s) {
    char *p;

    p = (char*)malloc(strlen(s) + 1);  // +1 for '\0'
    if (p != NULL)
        strcpy(p, s);
    return p;
}

/**
 * undef: Removes a name and definition from the hashtab table
 */
void undef(char *name) {

}

/**
 * trim: Removes whitespace characters from the front and back of the line
 */
void trim(char **line) {
    while (isspace(*(*line)++));
    *(*line)--;  // go back a space to account for overshoot
    char *start = *line;  // save start position
    while (*start++ != '\0');  // go to end of line
    *start--;  // go back a space to account for overshoot
    while (isspace(*start--));
    if (isspace(*start))
        *start = '\0';
}