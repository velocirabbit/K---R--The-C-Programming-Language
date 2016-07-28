/* 05E17 (p. 121) */
/**
*
* The Quicksort algorithm used in this program works as follows:
*   1. (base case) In a given range (initially the whole array to be sorted), if the
*      size of that range is 0 or 1, do nothing.
*   2. If the size of the range is smaller than a given threshold amount (default
*      20), that range is sorted using a shell sort algorithm instead (faster for
*      smaller arrays) with gap sizes of { 10, 4, 1 }.
*   3. If the size of the range is larger than the given threshold amount, first
*      find the pivot point to use by taking index of the range's ninther, which is
*      the median value OF THE median value of each third of the range.
*   4. Starting from both ends of the range and moving in, swap items around so that
*      items smaller than the pivot are at the front, items larger than the pivot
*      are at the end, and items equal to the pivot are between the two. The range
*      is now subdivided into three partitions.
*   5. Recursively go through steps 1-4 on the first and last partitions (the second
*      partition, with elements equal to the ninther pivot, is already sorted),
*      starting the smaller of the two partitions. Tail call recursion is performed
*      on the larger partition.
*/
#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define THRESHOLD   20      // Use insertion sort with arrays smaller than this
#define ARRSIZE     100000  // Size of array of numbers
#define MAXSTRLEN   1000    // Max length of a string
#define UPPER       1000    // Upper limit of the random numbers generated
#define SCREENWIDTH 80      // Width of the screen (used for printing)
#define DECIMAL     10      // Code for decimal option (used with itoa)
#define OPTIONSAT   1       // Index of argv where the options should start at
#define INTTYPE     1       // Sorting an int array as ints
#define STRTYPE     0       // Sorting an int array as strings
#define STRSTR      2       // Sorting a string array
#define DIVCHAR     '\\'    // Divider character for file paths in Windows
#define HEADARGS    0       // Position in header array of args
#define HEADNAME    1       // Position in header array of field name

// Possible option codes
#define USEFILE     'f'     // specify a file with lines of text to sort
#define NUMERIC     'n'     // Sort numerically, as opposed to as strings
#define REVERSE     'r'     // Sort in reverse order
#define PRINT       'p'     // Print the sorted array
#define MULTIPLE    'x'     // Do the search multiple times
#define IGNORE      'i'     // Fold upper and lower case together
#define DIRECTORY   'd'     // Compares only letters, numbers, and blanks
#define HEADER      'h'     // Sort by fields

typedef enum {False = 0, True} bool;

int getfile(char ***lines, char fn[], int len);
int randint(int n);
int widthof(int n);
int pivot(void *a, int lo, int hi, int(*comp)(int, int, int), int options[],
          int csv[]);
int median(void *a, int x, int y, int z, int(*comp)(int, int, int),
           int options[], int csv[]);
int *partition(void *a, int lo, int p, int hi, int(*comp)(int, int, int),
               int options[], int csv[]);
int numcmp(int i1, int i2, int r);
int tostrcmp(int i1, int i2, int r);
int strcmp2(char s1[], char s2[], int options[], int csv[]);
int dstrcmp(char s1[], char s2[]);
int quicksort(void *a, int lo, int hi, int t, int(*comp)(int, int, int),
              int options[], int csv[]);
int shellsort(void *a, int lo, int hi, int(*comp)(int, int, int),
              int options[], int csv[]);
int getCol(char header[], char name[], char delim);
void strlower(char s[]);
void swap(void *a, int type, int p, int q);
void getColVal(char *s, int csv[]);
double average(double *d, int n);
int getColPos(char line[], int col, char delim);  // UNUSED

const int GAPS[] = { 10, 4, 1, 0 };  // gap sizes to use for shell sorting
int options[] = {  // Parse options
    1,          // TIMES
    0,          // USEFILE
    STRTYPE,    // NUMERIC
    0,          // REVERSE
    0,          // PRINT
    0,          // IGNORE
    0,          // DIR
    0,          // HEADERS
};

// Set options
#define X options[0]
#define F options[1]
#define N options[2]
#define R options[3]
#define P options[4]
#define I options[5]
#define D options[6]
#define H options[7]

int main(int argc, char *argv[]) {
    char ***headers = malloc(sizeof(char**));
    if (headers == NULL) {
        printf("\nERROR: Couldn't create header pointer\n");
        return 1;
    }
    int fnPos;
    if (argc > OPTIONSAT - 1) {
        for (int i = OPTIONSAT; i < argc; i++) {
            if (argv[i][0] == '-' && strlen(argv[i]) == 2) {
                switch (argv[i][1]) {
                    case USEFILE:       // Gets lines of text from a specified file
                        F = 1;
                        N = STRSTR;
                        fnPos = ++i;
                        break;
                    case NUMERIC:       // Sets option to sort numerically
                        N = F ? STRSTR : INTTYPE;
                        break;
                    case REVERSE:       // Sets option to sort in reverse order
                        R = 1;
                        break;
                    case PRINT:         // Sets option to print sorted array
                        P = 1;
                        break;
                    case MULTIPLE:      // Sets option to sort multiple times
                    {
                        if (F) {
                            printf("\nERROR: Option -x can only be used when sorting \
                                    arrays of\n");
                            printf("       random ints.\n");
                            return 1;
                        }
                        int error = 0;
                        if (++i >= argc) {
                            error = 1;
                        }
                        else {
                            int c;
                            for (c = 0; isdigit(argv[i][c]); c++);
                            if (argv[i][c] == 0)
                                X = atoi(argv[i]);
                            else
                                error = 1;
                            if (X == 0) {
                                printf("\nERROR: Usage of -%c option must use a \
                                        number >0\n", MULTIPLE);
                                return 1;
                            }
                        }
                        if (error) {
                            printf("\nERROR: Usage of -x option should be followed \
                                    by\n");
                            printf("         a number\n");
                            return 1;
                        }
                        break;
                    }
                    case IGNORE:        // Sets option to ignore letter case
                        I = 1;
                        break;
                    case DIRECTORY:     // Sets option to treat strings as filepaths
                        D = 1;
                        break;
                    case HEADER:        // Sets various header options
                    {
                        int err = 0;
                        int params = (i + 1 < argc && argv[i + 1][0] != '-') ? 2 : 1;
                        if ((headers = malloc(sizeof(char**) * (H + 1))) != NULL) {
                            if ((headers[H] = malloc(sizeof(char*) * params)) != NULL) {
                                for (int p = 0; p < params; p++) {
                                    if ((headers[H][p] = malloc(sizeof(char[MAXSTRLEN]))) != NULL)
                                        strcpy(headers[H][p], argv[i + p]);
                                    else
                                        err = 1;
                                }
                            }
                            else
                                err = 1;
                        }
                        else
                            err = 1;
                        if (err == 1) {
                            printf("ERROR: Could not save header parameters\n");
                            return 1;
                        }
                        H++;
                        i++;
                        break;
                    }
                    default:
                        printf("\nERROR: Unknown parameter '-%c'\n", argv[i][1]);
                        return 1;
                }
            }
            // Usage statement
            else {
                printf("\nUsage: 05E16 [-%c filename] [-%c] [-%c] [-%c] \n",
                    USEFILE, NUMERIC, REVERSE, PRINT);
                printf("             [-%c times] [-%c] [-%c]\n", MULTIPLE, IGNORE,
                    DIRECTORY);
                printf("             [-%c[options] fieldname]\n", HEADER);
                printf("\nOptions:\n");
                printf("\t-%c filename\tSpecify a file with text to sort,\n",
                    USEFILE);
                printf("\t\t\totherwise sorts arrays of randomly generated\n");
                printf("\t\t\tintegers. If set, -n is forced to False (sort\n");
                printf("\t\t\tarray as strings)\n");
                printf("\t-%c\t\tSort numerically (otherwise sorts as strings)\n",
                    NUMERIC);
                printf("\t-%c\t\tSort in reverse order\n", REVERSE);
                printf("\t-%c\t\tPrint out the contents of the sorted array\n",
                    PRINT);
                printf("\t-%c times\tPerform the quicksort multiple times,\n",
                    MULTIPLE);
                printf("\t\t\tinitializing a new unsorted array each time.\n");
                printf("\t\t\t'times' must be an int.\n");
                printf("\t\t\tThis option can only be used when sorting arrays\n");
                printf("\t\t\tof random ints.\n");
                printf("\t-%c\t\tIgnore letter cases when sorting by strings\n",
                    IGNORE);
                printf("\t-%c\t\tSort lines of text as file paths, so\n",
                    DIRECTORY);
                printf("\t\t\tpunctuation is ignored\n");
                printf("\t-%c fieldname [-options]\n", HEADER);
                printf("\t\t\tSort lines by the indicated field. The fieldnames\n");
                printf("\t\t\tare taken from the first row and cannot start\n");
                printf("\t\t\twith a '-'. If the field name is a number, it's\n");
                printf("\t\t\tinterpreted as that number column. List multiple\n");
                printf("\t\t\toptions together immediately after the '-h'. 'x'\n");
                printf("\t\t\tand 'f' cannot be used as header sort options,\n");
                printf("\t\t\tand header field sorts can only be performed on\n");
                printf("\t\t\t.csv files. Multiple header sorts will be done in\n");
                printf("\t\t\torder, and each subsequent one will only sort on\n");
                printf("\t\t\tsubsets of the whole array.\n");
                printf("\n");
                return 1;
            }
        }
        if (H && !F) {
            printf("\nERROR: Option -%c can only be used when\n",
                HEADER);
            printf("       sorting csv files.\n");
            return 1;
        }
    }

    // Perform quicksort trials, saving the time taken for each trial to timeSpent
    double *timeSpent = malloc(sizeof(double) * X);
    int nlines;
    if (timeSpent == NULL) {
        printf("\nERROR: Couldn't allocate memory space for timeSpent array\n");
        return 1;
    }
    for (int t = 0; t < X; t++) {
        clock_t begin, end;
        // Sorting lines of text from a file rather than an array of random ints
        if (F) {
            // Pull lines of text from specified file
            char **lines = malloc(sizeof(char**));
            if (lines == NULL) {
                printf("\nERROR: Couldn't create pointer to text from file.\n");
                return 1;
            }
            int n;
            if ((n = getfile(&lines, argv[fnPos], MAXSTRLEN)) <= 0) {
                printf("\nERROR: File '%s' is either empty or\n", argv[fnPos]);
                printf("       can't be read (n = %d)\n", n);
                return 1;
            }
            nlines = n;

            // Quicksort + timing
            if (H) {
                begin = clock();
                //TODO: Parse options for each header field to sort by before sorting
                //TODO: Debug sorting multiple fields
                int csv[2], prev;
                char delim = ',';  //TODO: Implement a way to ask for this
                csv[1] = delim;
                for (int h = 0; h < H; h++) {
                    csv[0] = getCol(lines[0], headers[h][1], csv[1]);
                    if (!csv[0]) {
                        printf("ERROR: Couldn't find column named %s\n", headers[h][1]);
                        return 1;
                    }
                    if (h == 0)
                        quicksort(lines, 1, n - 1, THRESHOLD,
                                  (int(*)(int, int, int))(tostrcmp), options,
                                  csv);
                    else {  // subsequent sorts
                        int p = 1;
                        while (p < n) {
                            int lo = p;
                            char t[MAXSTRLEN], cur[MAXSTRLEN];
                            strcpy(t, lines[lo]);
                            int prevcsv[] = { prev, delim };
                            for (p; p < n; p++) {
                                strcpy(cur, lines[p]);
                                if (strcmp2(t, cur, options, prevcsv) != 0)
                                    break;
                            }
                            int hi = --p;
                            quicksort(lines, lo, hi, THRESHOLD,
                                      (int(*)(int, int, int))(tostrcmp),
                                      options, csv);
                            p++;
                        }
                    }
                    prev = csv[0];
                }
                end = clock();
            }
            else {
                int blank[] = { -1, -1 };
                begin = clock();
                quicksort(lines, 0, n - 1, THRESHOLD, (int(*)(int, int, int))(tostrcmp),
                    options, blank);
                end = clock();
            }

            // Print results
            if (P) {
                for (int i = 0; i < n; i++)
                    printf("%s", lines[i]);
                printf("\n");
            }

            // Clean up pointers
            for (--n; n >= 0; n--)
                free(lines[n]);
            free(lines);
        }
        // Sorting array of random integers (default behavior)
        else {
            // Generate an array of ARRSIZE integers between 0 and UPPER
            int demo[ARRSIZE];
            for (int i = 0; i < ARRSIZE; i++)
                demo[i] = randint(UPPER);
            // Should really convert array of strings into array of ints if
            // N == STRTYPE, but oh well

            // Quicksort + timing
            int blank[] = { -1, -1 };
            begin = clock();
            quicksort(demo, 0, ARRSIZE - 1, THRESHOLD,
                (int(*)(int, int, int))(N ? numcmp : tostrcmp), options, blank);
            end = clock();

            // Print results
            if (P) {
                int width = 0;
                for (int i = 0; i < ARRSIZE; i++) {
                    width += widthof(demo[i]) + 2;  // '+2' accounts for ', '
                    if (width > SCREENWIDTH - 1) {
                        printf("\n");
                        width = widthof(demo[i]) + 2;  // '+2' accounts for ', '
                    }
                    printf("%d%s", demo[i], (i == ARRSIZE - 1) ? "\n\n" : ", ");
                }
            }
        }
        timeSpent[t] = (double)(end - begin) / CLOCKS_PER_SEC;
    }

    printf("\nTime spent sorting %d %s %d times %s:\n",
        F ? nlines : ARRSIZE, F ? "lines of text" : "numbers", X,
        (N == INTTYPE) ? "numerically" : "as strings");
    for (int t = 0; t < X; t++)
        printf("\tTrial %d:\t%.3f seconds\n", t + 1, timeSpent[t]);
    printf("\nAverage: %.3f seconds\n", average(timeSpent, X));

    free(timeSpent);
    free(headers);
    return 0;
}

/* OVERHEAD STUFF */
/**
* getfile: Reads file fn and puts the lines of text into string array lines.
*   Uses getline(), so the newline character '\n' at the end of each line is
*   included. Assumes lines is empty and needs to be resized.
*
* Returns: The number of lines read, or -1 if an error occured.
*/
int getfile(char ***lines, char fn[], int len) {
    FILE *fp = fopen(fn, "r");
    if (fp == NULL) {
        printf("\nERROR: Couldn't open and read file '%s'\n", fn);
        return -1;
    }

    int i = 0;
    do {
        i++;
        // These memory spaces will be freed in main()
        if ((*lines = realloc(*lines, sizeof(char**) * i)) == NULL) {
            printf("\nERROR: Couldn't reallocate memory space for string array\n");
            return -1;
        }
        if (((*lines)[i - 1] = malloc(sizeof(char) * len)) == NULL) {
            printf("\nERROR: Couldn't reallocate memory space for text line %d\n",
                len);
            return -1;
        }
    } while (fgets((*lines)[i - 1], len, fp) != NULL);

    return i - 1;
}

/**
* randint: Uses rand() to generate a random integer in the range [0, n). Does
*   some correcting for uniformity.
*
* Returns: randomly generated integer in the range [0, n)
*/
int randint(int n) {
    if ((n - 1) == RAND_MAX) {
        return rand();
    }
    else {
        // Remove all of the values that would cause a skew
        long end = RAND_MAX / n;  // truncate skew=
        end *= n;

        // Ignore results from rand() that would fall above the limit
        int r;
        while ((r = rand()) >= end);

        return r % n;
    }
}

/**
* widthof: Finds the character width of int n
*
* Returns: width
*/
int widthof(int n) {
    int i;
    for (i = 1; n /= 10; i++);
    return i;
}
/* END OVERHEAD STUFF */

/* CSV COLUMN RELATED FUNCTIONS */
/**
 * getCol: Gets the column number of the column named 'name'. Uses the header
 *   to search. 'delim' signifies the character used to delineate a new column.
 *   Ignores delim characters within quotes (' or "), so those probably
 *   shouldn't be used as delim characters.
 *
 * Returns: the column position of column 'name' (1-indexed), 0 if it doesn't
 *   exist
 */
int getCol(char header[], char name[], char delim) {
    unsigned int i, j, col = 1;
    bool inQuote = False;
    for (i = 0; i < strlen(header); i++) {
        char q;
        if (!inQuote && (header[i] == '\'' || header[i] == '"')) {
            inQuote = True;
            q = header[i];
            i++;
        }
        for (j = 0; j < strlen(name) && header[i + j] == name[j]; j++) {
            if (inQuote && q == header[i + j])
                inQuote = False;
            if (!inQuote) {
                if (header[i + j] == delim)
                    break;
                else if (header[i + j] == '\'' || header[i + j] == '"') {
                    inQuote = True;
                    q = header[i];
                }
            }
        }
        if (inQuote && header[i + j] == q) {
            inQuote = False;
            i++;
        }
        if (name[j] == 0 && header[i + j] == delim)
                return col;
        else if (header[i + j] == 0)
            return 0;

        while (i < strlen(header) && header[++i] != delim);
        if (header[i] = 0)
            return 0;
        else
            col++;
    }
    return 0;
}

/* SORTING HELPER FUNCTIONS */
/**
* pivot: Obtain a pivot value for the subarray of v between lo and hi
*
* Returns: index position of value to use as pivot
*/
int pivot(void *a, int lo, int hi, int(*comp)(int, int, int), int options[],
          int csv[]) {
    int ptOne = (lo + hi) / 3;
    ptOne = (ptOne < lo) ? lo : ptOne;
    int ptTwo = (ptOne + hi) / 2;
    ptTwo = (ptTwo < ptOne) ? ptOne : ptTwo;

    // Find the median of each third of array v between lo and hi
    int first = median(a, lo, (lo + ptOne) / 2, ptOne, comp, options, csv);
    int second = median(a, ptOne + 1, (ptOne + 1 + ptTwo) / 2, ptTwo, comp,
        options, csv);
    int third = median(a, ptTwo + 1, (ptTwo + 1 + hi) / 2, hi, comp, options, csv);

    return median(a, first, second, third, comp, options, csv);
}

/**
* median: Obtains the median value for the three items at positions x, y, and z
*   in array v
*
* Returns: x, y, z (whichever is holds the median value), or -1 if something
*   goes wrong
*/
int median(void *a, int x, int y, int z, int(*comp)(int, int, int),
           int options[], int csv[]) {
    if (N == INTTYPE || N == STRTYPE) {
        int vX = ((int*)a)[x];
        int vY = ((int*)a)[y];
        int vZ = ((int*)a)[z];

        if ((*comp)(vX, vY, R) < 0) {
            if ((*comp)(vX, vZ, R) < 0)
                return ((*comp)(vY, vZ, R) < 0) ? y : z;
            else
                return x;
        }
        else {
            if ((*comp)(vY, vZ, R) < 0)
                return ((*comp)(vX, vZ, R) < 0) ? x : z;
            else
                return y;
        }
    }
    else if (N == STRSTR) {
        char *vX = ((char**)a)[x];
        char *vY = ((char**)a)[y];
        char *vZ = ((char**)a)[z];

        if (strcmp2(vX, vY, options, csv) < 0) {
            if (strcmp2(vX, vZ, options, csv) < 0)
                return (strcmp2(vY, vZ, options, csv) < 0) ? y : z;
            else
                return x;
        }
        else {
            if (strcmp2(vY, vZ, options, csv))
                return (strcmp2(vX, vZ, options, csv) < 0) ? x : z;
            else
                return y;
        }
    }
    else
        return -1;
}

/**
* partition: Returns an int[2] containing the two indices to use for
*   partitioning the subarray of array v between lo and hi. int[0] and int[1]
*   shouldn't ever be the same number, although they may be only one apart.
*
* Returns: int[2] of partition indices
*/
int *partition(void *a, int lo, int p, int hi, int(*comp)(int, int, int),
               int options[], int csv[]) {
    static int fail[] = { -1, -1 };  // just in case
    int i = lo - 1;
    int j = hi + 1;

    void *pivot = malloc(sizeof(char));
    int e = 0;
    if (pivot != NULL) {
        if (N == INTTYPE || N == STRTYPE) {
            if ((pivot = (int*)malloc(sizeof(int))) != NULL)
                *((int*)pivot) = ((int*)a)[p];
            else
                e = 1;
        }
        else if (N == STRSTR) {
            // Copy string at a[p] into pivot so pivot doesn't change
            if ((pivot = (char*)malloc(sizeof(char[MAXSTRLEN]))) != NULL)
                strcpy((char*)pivot, ((char**)a)[p]);
            else
                e = 1;
        }
    }
    else
        e = 1;
    if (e) {
        printf("\nERROR: Couldn't create temp %s pointer to pivot value while\n",
            (N == INTTYPE || N == STRTYPE) ? "int*" : "char*");
        printf("       partitioning subarray a[%d:%d] (pivot index = %d)\n",
            lo, hi, p);
        return fail;
    }

    while (1) {
        if (N == INTTYPE || N == STRTYPE) {
            // Starting from i, ignore any values <pivot
            do {
                i++;
            } while ((*comp)(((int*)a)[i], *((int*)pivot), R) < 0);

            // Starting from j, ignore any values >pivot
            do {
                j--;
            } while ((*comp)(((int*)a)[j], *((int*)pivot), R) > 0);
        }
        else if (N == STRSTR) {
            do {
                i++;
            } while (strcmp2(((char**)a)[i], (char*)pivot, options, csv) < 0);

            do {
                j--;
            } while (strcmp2(((char**)a)[j], (char*)pivot, options, csv) > 0);
        }

        // If no other pairs to swap found, return partition values
        if (i >= j) {
            int *result = malloc(sizeof(int[2]));
            if (result != NULL) {
                // Leave values equal to pivot value alone
                if (N == INTTYPE || N == STRTYPE) {
                    for (j; j >= lo && (*comp)(((int*)a)[j], *((int*)pivot),
                                               R) == 0; j--);
                    for (i; i <= hi && (*comp)(((int*)a)[i], *((int*)pivot),
                                               R) == 0; i++);
                }
                else if (N == STRSTR) {
                    for (j; j >= lo && strcmp2(((char**)a)[j], (char*)pivot,
                                               options, csv) == 0; j--);
                    for (i; i <= hi && strcmp2(((char**)a)[i], (char*)pivot,
                                               options, csv) == 0; i++);
                }
            }
            else {
                printf("\nERROR: Couldn't create pointer to partition values\n");
                printf("       while partitioning subarray a[%d:%d]\n", lo, hi);
                printf("       (i = %d, j = %d, p = %d)\n", i, j, p);
                return fail;
            }
            result[0] = j;
            result[1] = (i == j) ? i + 1 : i;  // So i and j aren't equal
            free(pivot);
            return result;
        }

        // If a pair to swap found, swap them
        swap(a, N, i, j);
    }
}

/**
* Swaps the values at indices p and q in array a
*/
void swap(void *a, int type, int p, int q) {
    if (type == INTTYPE || type == STRTYPE) {
        int temp = ((int*)a)[p];
        ((int*)a)[p] = ((int*)a)[q];
        ((int*)a)[q] = temp;
    }
    else if (type == STRSTR) {
        char temp[MAXSTRLEN];
        strcpy(temp, ((char**)a)[p]);
        strcpy(((char**)a)[p], ((char**)a)[q]);
        strcpy(((char**)a)[q], temp);
    }
}

/**
* numcmp: Compares the two integers as numbers
*
* Returns: 0 if the two ints are equal, <0 if the i1<i2, >0 if i2<i1 (unless r = 1)
*/
int numcmp(int i1, int i2, int r) {
    return r ? i2 - i1 : i1 - i2;
}

/**
* tostrcmp: Compares the two integers as strings
*
* Returns: 0 if the two ints are equal, <0 if the i1<i2, >0 if i2<i1 (unless r = 1)
*/
int tostrcmp(int i1, int i2, int r) {
    char c1[MAXSTRLEN], c2[MAXSTRLEN];
    _itoa(i1, c1, DECIMAL);
    _itoa(i2, c2, DECIMAL);

    return r ? strcmp(c2, c1) : strcmp(c1, c2);
}

/**
* strcmp2: Basically just strcmp(), but can set options to ignore case in each
*   sentence or even compare in reverse order. csv is an int array where csv[0]
*   is the column number to use, and csv[1] is the character deliminator.
*
* Returns: 0 if the two strs are equal, <0 if the s1<s2, >0 if s2<s1 (unless r = 1)
*/
int strcmp2(char s1[], char s2[], int options[], int csv[]) {
    char c1[MAXSTRLEN], c2[MAXSTRLEN];
    strcpy(c1, s1);
    strcpy(c2, s2);
    if (I) {
        strlower(c1);
        strlower(c2);
    }
    if (csv[0] != -1) {
        getColVal(c1, csv);
        getColVal(c2, csv);
    }
    if (D)
        return R ? dstrcmp(c2, c1) : dstrcmp(c1, c2);
    else
        return R ? strcmp(c2, c1) : strcmp(c1, c2);
}

/**
* dstrcmp: Like strcmp, but suited for directory path comparison by only making
*   comparisons on letters, numbers, and blanks.
*
* Returns: 0 if the two paths are equal, <0 if the s1<s2, >0 if s2<s1 (unless r = 1)
*/
int dstrcmp(char s1[], char s2[]) {
    int i;
    for (i = 0; (isalnum(s1[i]) || isblank(s1[i]))
        && (isalnum(s2[i]) || isblank(s2[i])) && (s1[i] == s2[i]); i++);
    if (s1[i] == 0 && s2[i] == 0)
        return 0;
    else
        return s2[i] - s1[i];
}

/**
* strlower: Converts string s to all lowercase in place.
*/
void strlower(char s[]) {
    for (int i = 0; s[i] = tolower(s[i]); i++);
}

/**
 * getcol: Takes string s, and uses int[] csv to pull out the relevant column.
 *   csv is an int array where csv[0] is the column number to use, and csv[1]
 *   is the character deliminator.
 */
void getColVal(char *s, int csv[]) {
    int b = -1, e, col = 1, q = 0, i;
    for (i = 0; i < (int)strlen(s) && col <= csv[0]; i++) {
        if (!q) {
            if (s[i] == '\'' || s[i] == '"')
                q = 1;
            else if (s[i] == csv[1])
                col++;
            if (col == csv[0] && b == -1)
                b = i + 1;
        }
        else if (s[i] == '\'' || s[i] == '"')
            q = 0;
    }
    e = i;
    b--;
    for (i = 0; (i + b) < e; i++)
        s[i] = s[i + b];
    s[i] = 0;
}

/**
* Shell sort algorithm
*/
int shellsort(void *a, int lo, int hi, int(*comp)(int, int, int),
              int options[], int csv[]) {
    // Temp pointer to be used with shifting elements
    void *temp = malloc(sizeof(char));
    int e = 0;
    if (temp != NULL) {
        if (N == INTTYPE || N == STRTYPE) {
            if ((temp = (int*)malloc(sizeof(int))) == NULL)
                e = 1;
        }
        else {  // STRSTR
            if ((temp = (char*)malloc(sizeof(char[MAXSTRLEN]))) == NULL)
                e = 1;
        }
    }
    else
        e = 1;
    if (e) {
        printf("\nERROR: Couldn't cast temp pointer to %s while shellsorting\n",
            (N == INTTYPE || N == STRTYPE) ? "int*" : "char*");
        printf("       subarray a[%d:%d]\n", lo, hi);
        return 1;
    }

    // Actual Shellsorting starts here. For each gap size...
    for (int g = 0; GAPS[g] > 0; g++) {
        /* ... do a gapped insertion sort. The first item in each gapped subarray is
        trivially already sorted, so we start from lo+gap and work up from there */
        for (int i = lo + GAPS[g]; i <= hi; i++) {
            // Store a[i] in temp in case it needs to be shifted
            if (N == INTTYPE || N == STRTYPE)
                *((int*)temp) = ((int*)a)[i];
            else  // STRSTR
                strcpy((char*)temp, ((char**)a)[i]);

            /* Check each item sequentially, comparing them to all of the other
               elements in its gapped subarray that came before (if any) */
            int j;
            for (j = i; j >= lo + GAPS[g]; j -= GAPS[g]) {
                /* Shift larger elements up to where element i was, then put element
                   i in the new space so it's now ordered in its gapped sub */
                int res;
                if (N == INTTYPE || N == STRTYPE)
                    res = (*comp)(*((int*)temp), ((int*)a)[j - GAPS[g]], R);
                else  // STRSTR
                    res = strcmp2((char*)temp, ((char**)a)[j - GAPS[g]], options, csv);

                if (res < 0)
                    swap(a, N, j, j - GAPS[g]);
                else
                    break;
            }

            // Put temp where there is now an empty slot in j
            if (N == INTTYPE || N == STRTYPE)
                ((int*)a)[j] = *((int*)temp);
            else  // STRSTR
                strcpy(((char**)a)[j], temp);
        }
    }

    // Clean up
    free(temp);
    return 0;
}
/* END SORTING HELPER FUNCTIONS */

/**
* Recursive quicksort algorithm
*/
int quicksort(void *a, int lo, int hi, int t, int(*comp)(int, int, int),
              int options[], int csv[]) {
    // Base case. Do nothing if there are fewer than two elements in subarray
    if (lo >= hi)
        return 0;
    // Arrays smaller than threshold size t will be sorted with shell sort
    if (hi - lo + 1 <= t)  // '+1' to make it an inclusive range
        return shellsort(a, lo, hi, comp, options, csv);

    int p = pivot(a, lo, hi, comp, options, csv);              // pivot index
    int *parts = partition(a, lo, p, hi, comp, options, csv);  // partition indices

    // Recursion over the two partitions, starting with the smaller of the two
    if (parts[0] - lo <= hi - parts[1]) {
        quicksort(a, lo, parts[0], t, comp, options, csv);
        return quicksort(a, parts[1], hi, t, comp, options, csv);
    }
    else {
        quicksort(a, parts[1], hi, t, comp, options, csv);
        return quicksort(a, lo, parts[0], t, comp, options, csv);
    }
}

/**
* average: Finds the average of an array of doubles. n is the number of doubles
*   in the array.
*
* Returns: The average as a double
*/
double average(double *d, int n) {
    double avg = 0.0;
    for (int i = 0; i < n; i++)
        avg += d[i];
    return avg / (double)n;
}


/* UNUSED */
/**
* getColPos: Finds where in string line the column in position col starts.
*   The returned int will be the first character after the delimiter character
*   (0-indexed position).
*
* Returns: the char index in line (0-indexed) where column number col starts,
*   or -1 if it doesn't exist
*/
int getColPos(char line[], int col, char delim) {
    int c = 1, i;
    bool inQuote = False;
    char q;
    for (i = 0; i < (int)strlen(line); i++) {
        if (!inQuote) {
            if (line[i] == '\'' || line[i] == '"') {
                inQuote = True;
                q = line[i];
            }
            else if (line[i] == delim)
                c++;
            if (c == col)
                break;
        }
        else if (line[i] == q)
            inQuote = False;
    }
    if (line[i] == 0)
        return -1;
    else
        return i + 1;
}