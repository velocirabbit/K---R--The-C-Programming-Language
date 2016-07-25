/* 04E10.c (p. 79) */
#define _CRT_SECURE_NO_WARNINGS
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXOPLEN 1000     // max operator and operand lengths
#define VSTACKSIZE 1000   // max valueStack size
#define STROPCOUNT 14     // number of string operands
#define ANS "ans"         // ANS variable name
#define ZERO "0.0"        // initial ANS variable value

#define STRERR 0        // indicates an error while reading a string operand
#define NUMBER '0'        // indicates a number was found in getop()
#define DUPE 'd'          // indicates a dupe() operand. Next two ops are p and c
#define SWAP 's'          // indicates a swap() operand. Next two ops are p1 and p2
#define DEL  'x'          // indicates a del() operand. Next op is p
#define PRINT 'p'         // indicates a print() operand. Next op is p
#define CLEAR 'c'         // indicates a clear() operand
#define SINE 'n'          // sin() operand
#define COS 'o'           // cos() operand
#define TAN 't'           // tan() operand
#define EXP 'e'           // exp() operand
#define POW 'w'           // pow() operand
#define LN 'l'            // ln() operand
#define LOG 'g'           // log() operand
#define MEMORIZE 'm'      // memorize() operand
#define RECALL 'r'        // recall() operand

typedef enum { False = 0, True } bool;

bool setup();
bool equals(char s1[], char s2[]);
int getop(char s[]);
double modulo(double n, double d);
double pop(void);
void push(double n);
void print(int p);
void dupe(int p, char c);
void swap(int p1, int p2);
void del(int p);
void clear(void);
void memorize(char s[], double n, bool flag);
bool recall(char s[]);
int getline(char line[], int max);

bool inputEmpty = True;
int vsPos = 0, iPos = 0;
int varCount = 1;
double valueStack[VSTACKSIZE];
char *input;
char **keys;
char **values;

char strOps[][STROPCOUNT] = { "dupe", "swap", "del", "print",
                              "clr", "sin", "cos", "tan",
                              "exp", "pow", "ln", "log",
                              "mem", "re" };

main() {
    if (setup() == False)
        return -1;
    bool err = False;
    int type;
    char opStr[MAXOPLEN];
    
    while (getline(input, MAXOPLEN) > 0) {
        // Parse the input operation string until we reach a newline
        inputEmpty = False;
        do {
            type = getop(opStr);
            switch (type) {
                case NUMBER:            // number
                    push(atof(opStr));
                    break;
                case '+':               // addition
                    push(pop() + pop());
                    break;
                case '*':               // multiplication
                    push(pop() * pop());
                    break;
                case '-':               // subtraction
                {
                    double op2 = pop();
                    push(pop() - op2);
                    break;
                }
                case '/': case '%':     // division and modulo
                {
                    double op2 = pop();
                    if (op2 != 0.0) {
                        if (type == '/')
                            push(pop() / op2);
                        else
                            push(modulo(pop(), op2));
                    }
                    else
                        printf("ERROR: Division by 0\n");
                    break;
                }
                case DUPE:              // dupe
                {
                    if (getop(opStr) == NUMBER) {
                        int p = atoi(opStr);
                        char c = getop(opStr);
                        dupe(p, c);
                    }
                    else
                        printf("ERROR: Could not duplicate value\n");
                    break;
                }
                case SWAP:              // swap
                {
                    int p1, p2;
                    if (getop(opStr) == NUMBER)
                        p1 = atoi(opStr);
                    else
                        printf("ERROR: Could not swap values\n");
                    if (getop(opStr) == NUMBER)
                        p2 = atoi(opStr);
                    else
                        printf("ERROR: Could not swap values\n");
                    swap(p1, p2);
                    break;
                }
                case DEL:               // del
                {
                    if (getop(opStr) == NUMBER) {
                        int p = atoi(opStr);
                        del(p);
                    }
                    else
                        printf("ERROR: Could not delete value\n");
                    break;
                }
                case PRINT:             // print
                {
                    if (getop(opStr) == NUMBER) {
                        int p = atoi(opStr);
                        print(p);
                    }
                    else
                        printf("ERROR: Could not print value\n");
                }
                case CLEAR:             // clear
                    clear();
                    break;
                case SINE:              // sin
                    push(sin(pop()));
                    break;
                case COS:               // cos
                    push(cos(pop()));
                    break;
                case TAN:               // tan
                    push(tan(pop()));
                    break;
                case EXP:               // exp
                    push(exp(pop()));
                    break;
                case POW:               // pow
                {
                    double op2 = pop();
                    push(pow(pop(), op2));
                    break;
                }
                case LN:                // natural log
                    push(log(pop()));
                    break;
                case LOG:               // base-10 log
                    push(log10(pop()));
                    break;
                case MEMORIZE:          // memorize
                {
                    if (getop(opStr) != NUMBER) {
                        if (iswspace(opStr[0]) || opStr[0] == '\n') {
                            printf("ERROR: Syntax for storing or recalling a variable is:\n");
                            printf("       (mem/re)->{name}\n");
                        }
                        else {
                            double p = pop();
                            memorize(opStr, p, False);
                            push(p);
                        }
                    }
                    else
                        printf("ERROR: Variables cannot have numbers as names\n");
                    break;
                }
                case RECALL:            // recall
                    if (getop(opStr) != NUMBER)
                        if (!recall(opStr))
                            printf("ERROR: Couldn't find variable named '%s'\n", opStr);
                    break;
                case '\n':              // submit
                {
                    if (!err) {
                        double p = pop();
                        printf("\t%.8g\n", p);
                        memorize(ANS, p, True);
                    }
                    else
                        err = False;
                    iPos = 0;
                    inputEmpty = True;
                    break;
                }
                default:                // default
                    printf("ERROR: Unknown command '%s'\n", opStr);
                    err = True;
                    break;
            }
        } while (!inputEmpty);
    }
}

/*
 * setup: Create memory space for the two variable-related string arrays keys[]
 *   and values[]
 */
bool setup() {
    input = malloc(sizeof(char[MAXOPLEN]));
    if (input == NULL) {
        printf("ERROR: Couldn't create memory space for input string\n");
        return False;
    }

    keys = malloc(sizeof(char*));
    values = malloc(sizeof(char*));
    if (keys == NULL || values == NULL) {
        printf("ERROR: Couldn't create memory space for variable arrays\n");
        return False;
    }
    keys[0] = malloc(sizeof(char[MAXOPLEN]));
    values[0] = malloc(sizeof(char[MAXOPLEN]));
    if (keys[0] == NULL || values[0] == NULL) {
        printf("ERROR: Couldn't create memory space for variable 'ans'\n");
        return False;
    }
    strcpy(keys[0], ANS);
    strcpy(values[0], ZERO);
    return True;
}

/*
 * equals: Compares strings s1 and s2 and sees if they're equal, returning a bool
 *   indicating the result.
 */
bool equals(char s1[], char s2[]) {
    if (strlen(s1) != strlen(s2))
        return False;
    for (int i = 0; s1[i] != 0 || s2[i] != 0; i++)
        if (s1[i] != s2[i])
            return False;
    return True;
}

/*
* getop: Gets the next operator or operand from the input. Returns code NUMBER ('0') if
*   a number was found, or the next character if not (including the EOF character). Can
*   get negative numbers.
*/
int getop(char s[]) {
    int i = 0;
    for (iPos; input[iPos] == ' ' || input[iPos] == '\t'; iPos++);

    if (!isdigit(input[iPos]) && input[iPos] != '.') {
        if (input[iPos] == '-' || input[iPos] == '+') {
            s[i] = input[iPos];
            if (isspace(input[++iPos]))
                return s[i];
            i++;
        }
        else if (isalpha(input[iPos])) {
            int m;
            for (i; isalpha(input[iPos]); i++)
                s[i] = input[iPos + i];
            iPos += i;
            s[i] = 0;
            for (m = 0; m < STROPCOUNT; m++)
                if (equals(s, strOps[m]))
                    break;
            switch (m) {
                case 0:     return DUPE;
                case 1:     return SWAP;
                case 2:     return DEL;
                case 3:     return PRINT;
                case 4:     return CLEAR;
                case 5:     return SINE;
                case 6:     return COS;
                case 7:     return TAN;
                case 8:     return EXP;
                case 9:     return POW;
                case 10:    return LN;
                case 11:    return LOG;
                case 12: case 13:
                {
                    if (input[iPos] == '-' && input[iPos + 1] == '>') {
                        iPos += 2;
                        return (m == 12) ? MEMORIZE : RECALL;
                    }
                    else {
                        printf("ERROR: Syntax for storing or recalling a variable is:\n");
                        printf("         (mem/re)->{name}\n");
                    }
                }
                default:    return STRERR;
            }
        }
        else {
            iPos++;
            return input[iPos - 1];
        }
    }

    // Is a number. This won't be reached if it isn't
    if (isdigit(input[iPos])) {
        iPos = i > 0 ? iPos - 1 : iPos;
        for (i; isdigit(input[iPos + i]); i++)
            s[i] = input[iPos + i];
        iPos += i;
    }
    if (input[iPos] == '.') {
        s[i++] = input[iPos];
        for (i; isdigit(input[iPos + i]); i++)
            s[i] = input[iPos + i];
        input += i;
    }
    s[i] = '\0';
    return NUMBER;
}

/*
* modulo: Literally just the modulo operator (%), but allows for floating point
*   operands. Returns a value as expected, and can be used with negative numbers.
*/
double modulo(double n, double d) {
    int nSign = (n < 0) ? -1 : 1;
    int dSign = (d < 0) ? -1 : 1;
    int m;
    for (m = 1; dSign * d * m <= nSign * n; m++);
    return n - (nSign * dSign * d * --m);
}

/*
 * pop: Returns the topmost value in the stack if available.
 */
double pop(void) {
    if (vsPos > 0)
        return valueStack[--vsPos];
    else
        printf("ERROR: valueStack is empty. Nothing to pop off\n");
    return 0.0;
}

/*
* push: Puts double n into the next free position in the value stack (unless the stack
*   is full).
*/
void push(double n) {
    if (vsPos < VSTACKSIZE)
        valueStack[vsPos++] = n;
    else
        printf("ERROR: valueStack full! Can't push on %g", n);
}

/*
* print: Prints the number at position p in valueStack. If p >= vsPos, prints the last
*   number instead.
*/
void print(int p) {
    if (p < vsPos)
        printf("valueStack[%d] = %.8g\n", p, valueStack[p]);
    else {
        printf("ERROR: Can't print value at position %d\n", p);
        printf("       Outside of valueStack's range (vsPos = %d)\n", vsPos);
    }
}

/*
* dupe: Duplicates the number at position p in valueStack. c is a code that determines
*   where the duplicated value will go:
*
*      'a': adjacent to the original. Every value after p is shifted up by 1, and the
*             dupe takes position p + 1
*      'z': end of the stack. The dupe will take position vsPos (default)
*
*      anything else: Uses the default ('z'), but prints an error message indicating so
*
*   If p >= vsPos, nothing happens.
*/
void dupe(int p, char c) {
    if (p < vsPos) {
        switch (c) {
            case 'a':
                for (int i = vsPos; i > p; i--)
                    valueStack[i] = valueStack[i - 1];
                vsPos++;
                break;
            case 'z':
                push(valueStack[p]);
                break;
            default:
                printf("ERROR: Unrecognized dupe() code '%c' - placing at end instead\n", c);
                push(valueStack[p]);
                break;
        }
    }
    else {
        printf("ERROR: Can't dupe value at position %d\n", p);
        printf("       Outside of valueStack's range (vsPos = %d)\n", vsPos);
    }
}

/*
* Swaps the values at positions p1 and p2 in valueStack. If either >= vsPos, nothing
*   happens.
*/
void swap(int p1, int p2) {
    if (p1 < vsPos && p2 < vsPos) {
        double t = valueStack[p1];
        valueStack[p1] = valueStack[p2];
        valueStack[p2] = t;
    }
    else {
        printf("ERROR: Can't swap values at positions %d and %d.\n", p1, p2);
        printf("       One or both are outside of valueStack's range (vsPos = %d)\n", vsPos);
    }
}

/*
* del: Deletes the value at position p in valueStack. Everything after is shifted down
*   1 position. If p >= vsPos, nothing happens.
*/
void del(int p) {
    if (p < vsPos) {
        for (int i = 0; i < vsPos - 1; i++)
            valueStack[i] = valueStack[i + 1];
        vsPos--;
    }
    else {
        printf("ERROR: Can't delete value at position %d\n", p);
        printf("       Outside of valueStack's range (vsPos = %d)\n", vsPos);
    }
}

/*
* clear: Sets vsPos = 0, effectively clearing valueStack
*/
void clear(void) {
    vsPos = 0;
}

/*
 * memorize: Stores double n in the variables array with key s. variables acts as
 *   an array of dictionary-like tuples, where the first value is the key and the
 *   second is a double stored as a string. The variable name 'ans' is protected
 *   to mean the most recently printed value. Flag is used to see who is storing
 *   a variable named 'ans'.
 */
void memorize(char s[], double n, bool flag) {
    if (flag == False && equals(ANS, s)) {
        printf("ERROR: Cannot store with variable named 'ans'\n");
        return;
    }
    int i;
    bool found = False;
    for (i = 0; i < varCount; i++) {
        if (equals(keys[i], s)) {
            found = True;
            snprintf(values[i], MAXOPLEN, "%f", n);
            break;
        }
    }
    if (!found) {
        i = varCount;
        varCount++;

        keys = realloc(keys, varCount * sizeof(char*));
        values = realloc(values, varCount * sizeof(char*));
        if (values == NULL || keys == NULL) {
            printf("ERROR: Couldn't reallocate memory for variable arrays\n");
            return;
        }
        keys[i] = malloc(sizeof(char[MAXOPLEN]));
        values[i] = malloc(sizeof(char[MAXOPLEN]));
        if (keys[i] == NULL) {
            printf("ERROR: Couldn't reallocate memory for variable key '%s' with value %f\n", s, n);
            return;
        }

        strcpy(keys[i], s);
        snprintf(values[i], MAXOPLEN, "%f", n);
    }
}

/*
 * recall: Recalls the value stored under the variable with name s in the variables
 *   array, then pushes it to the value stack directly. If the variable doesn't
 *   exist, recall() returns False.
 */
bool recall(char s[]) {
    for (int i = 0; i < varCount; i++) {
        if (equals(keys[i], s)) {
            double d = atof(values[i]);
            push(d);
            return True;
        }
    }
    return False;
}

/*
 * getline: Gets a line of input, returning the number of characters read
 *   (or 0 if an error)
 */
int getline(char line[], int max) {
    int i = 0;
    for (i = 0; i < max && (line[i] = getchar()) != EOF && line[i] != '\n'; i++);
    return i;
}