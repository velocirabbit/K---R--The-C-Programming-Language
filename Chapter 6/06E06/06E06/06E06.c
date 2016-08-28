/* 06E06.c (p. 145) */
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
    char **args;            // variable names
} nlist;
static nlist *hashtab[HASHSIZE];  // pointer table

int getword(char **line, char *w);
unsigned hash(char *s);  // hashing function
nlist *lookup(char *s);  // lookup function
nlist *install(char *name, char *defn, char **args);
char *strdup2(char *s);
char **checkargs(char *s);
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
                //TODO: Check for args and put into value
                for (np; np != NULL; np = np->next)
                    if (strcmp(np->name, key) == 0)
                        printf("\t%s\n", np->defn);
            }
        }
        else if (getword(&line, directive) > 0) {
            // Execute a directive
            if (strcmp(directive, DEFINE) == 0) {
                char **args;
                getword(&line, key);
                args = checkargs(key);
                trim(&line);
                strcpy(value, line);
                install(key, value, args);
            }
            else if (strcmp(directive, UNDEF) == 0) {
                getword(&line, key);
                undef(key);
            }
            else {
                printf("ERROR: Unknown directive '%s'\n", directive);
            }
        }
    }

    printf("\n");
    system("pause");
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
nlist *install(char *name, char *defn, char **args) {
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
    else {                            // already there
        free((void*)np->defn);  // free previous defn
        np->args = NULL;
    }
    if ((np->defn = strdup2(defn)) == NULL) {
        np->args = NULL;
        return NULL;
    }
    else
        np->args = args;
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
 * checkargs: Checks string s (name of a defined key) for any argument inputs
 *   and puts the variable names into a string array. Cuts out the arguments
 *   from the key name.
 *
 * Returns: String array of variable names
 */
char **checkargs(char *s) {
    char *args = strchr(s, '(');
    if (args == NULL)       // no arguments in this key!
        return NULL;
    *args++ = '\0';         // cut off the key name
    int numargs = 0;        // just in case it's empty between the ()
    char **argnames = malloc(sizeof(char*));
    for (args; *args != ')'; args++) {  // can stop when a ')' is found
        trim(&args);
        if (isalnum(*args) || *args == '_') {
            numargs++;
            argnames = realloc(argnames, sizeof(char*) * numargs);
            *argnames = malloc(sizeof(char[STRLEN]));
            int i;
            for (i = 0; i < STRLEN && (isalnum(*args) || *args == '_');
                                                                i++, args++) {
                argnames[numargs - 1][i] = *args;
            }
            argnames[numargs - 1][i] = '\0';
            trim(&args);
            if (*args == ',')
                args++;
        }
        else {
            printf("ERROR: Improper arg names for '%s'\n", s);
            return NULL;    // dunno what else to do with improper arg names
        }
    }
    return argnames;
}

/**
* undef: Removes a name and definition from the hashtab table
*/
void undef(char *name) {
    nlist *np;
    if ((np = lookup(name)) == NULL)
        printf("ERROR: Key '%s' is not defined\n", name);
    else {
        nlist *prev = NULL;
        int hashval = hash(name);
        for (np = hashtab[hashval]; np != NULL; np = np->next) {
            if (strcmp(name, np->name) == 0) {
                if (prev == NULL)
                    hashtab[hashval] = NULL;
                else
                    prev->next = np->next;
                free(np);  // free memory space
                break;
            }
            prev = np;
        }
    }
}

/**
* trim: Removes whitespace characters from the front and back of the line
*/
void trim(char **line) {
    while (isspace(*(*line)++));
    *(*line)--;                 // go back a space to account for overshoot
    char *start = *line;        // save start position
    while (*start++ != '\0');   // go to end of line
    *start--;                   // go back a space to account for overshoot
    while (isspace(*start--));
    if (isspace(*start))
        *start = '\0';
}