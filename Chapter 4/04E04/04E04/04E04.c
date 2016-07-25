/* 04E04.c (p. 79) */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define MAXOPLEN 1000     // max operator and operand lengths
#define VSTACKSIZE 1000   // max valueStack size
#define MAXBUFFSIZE 1000  // max char input buffer size
#define NUMBER '0'        // indicates a number was found in getop()
#define DUPE 'd'          // indicates a dupe() operand. Next two ops are p and c
#define SWAP 's'          // indicates a swap() operand. Next two ops are p1 and p2
#define DEL  'x'          // indicates a del() operand. Next op is p
#define PRINT 'p'         // indicates a print() operand. Next op is p
#define CLEAR 'c'         // indicates a clear() operand

int getop(char s[]);
int getch(void);
void ungetch(char c);
double modulo(double n, double d);
double pop(void);
void push(double n);
void print(int p);
void dupe(int p, char c);
void swap(int p1, int p2);
void del(int p);
void clear();

int vsPos = 0, buffPos = 0;
double valueStack[VSTACKSIZE];
char buff[MAXBUFFSIZE];

main() {
    int type;
    double op2;
    char opStr[MAXOPLEN];

    while ((type = getop(opStr)) != EOF) {
        switch (type) {
            case NUMBER:
                push(atof(opStr));
                break;
            case '+':
                push(pop() + pop());
                break;
            case '*':
                push(pop() * pop());
                break;
            case '/': case '%':
                op2 = pop();
                if (op2 != 0.0) {
                    if (type == '/')
                        push(pop() / op2);
                    else
                        push(modulo(pop(), op2));
                }
                else
                    printf("ERROR: Divide by zero\n");
                break;
            case '-':
                op2 = pop();
                push(pop() - op2);
                break;
            case DUPE:
                if (getop(opStr) == NUMBER) {
                    int p = atoi(opStr);
                    char c = getop(opStr);
                    dupe(p, c);
                }
                else {
                    printf("ERROR: Could not duplicate value\n");
                }
                break;
            case SWAP:
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
            case DEL:
            {
                if (getop(opStr) == NUMBER) {
                    int p = atoi(opStr);
                    del(p);
                }
                else
                    printf("ERROR: Could not delete value\n");
                break;
            }
            case PRINT:
            {
                if (getop(opStr) == NUMBER) {
                    int p = atoi(opStr);
                    print(p);
                }
                else
                    printf("ERROR: Could not print\n");
                break;
            }
            case CLEAR:
                clear();
                break;
            case '\n':
                printf("\t%.8g\n", pop());
                break;
            default:
                printf("ERROR: Unknown command '%s'\n", opStr);
                break;
        }
    }
    return 0;
}

/*
 * getop: Gets the next operator or operand from the input. Returns code NUMBER ('0') if
 *   a number was found, or the next character if not (including the EOF character). Can
 *   get negative numbers.
 */
int getop(char s[]) {
    int i = 0;
    while ((s[i] = getch()) == ' ' || s[i] == '\t');

    if (!isdigit(s[i]) && s[i] != '.') {
        // Is either an operator, a sign, or nothing useful
        if (s[i] == '-' || s[i] == '+') {
            s[++i] = getch();
            if (isspace(s[i])) {
                ungetch(s[i]);
                // The '-' or '+' was just an operator after all
                return s[--i];
            }
        }
        else
            return s[i];  // definitely not a number
    }

    // Is a number
    if (isdigit(s[i]))
        while (isdigit(s[++i] = getch()));
    if (s[i] == '.')
        while (isdigit(s[++i] = getch()));

    if (s[i] != EOF)
        ungetch(s[i]);
    s[i] = '\0';
    return NUMBER;
}

/*
 * getch: Gets the next character from the char input buffer. If empty, gets the next
 *   char from the input instead.
 */
int getch(void) {
    return (buffPos > 0) ? buff[--buffPos] : getchar();
}

/*
 * ungetch: Puts character c into the next free position in buff (unless it's full)
 */
void ungetch(char c) {
    if (buffPos > MAXBUFFSIZE)
        printf("ERROR: Char input buffer full! Cannot unget '%c'\n", c);
    else
        buff[buffPos++] = c;
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
    return 0.0;  // Only reached if valueStack is empty and pop() is attempted
}

/*
 * push: Puts double n into the next free position in the value stack (unless the stack
 *   is full).
 */
void push(double n) {
    if (vsPos < VSTACKSIZE)
        valueStack[vsPos++] = n;
    else
        printf("ERROR: valueStack full! Can't push %g onto it\n", n);
}

/*
 * print: Prints the number at position p in valueStack. If p >= vsPos, prints the last
 *   number instead.
 */
void print(int p) {
    if (p < vsPos)
        printf("valueStack[%d] = %.8g\n", p, valueStack[p]);
    else
        printf("ERROR: Can't print value at position %d - outside of valueStack\n", p);
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
    else
        printf("ERROR: Can't dupe value at position %d - outside of valueStack\n", p);
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
        printf("Error: Can't swap values at positions %d and %d.\n", p1, p2);
        printf("       One or both are outside of valueStack (vsPos = %d)", vsPos);
    }
}

/*
 * del: Deletes the value at position p in valueStack. Everything after is shifted down
 *   1 position. If p >= vsPos, nothing happens.
 */
void del(int p) {
    if (p < vsPos) {
        for (int i = p; i < vsPos - 1; i++)
            valueStack[i] = valueStack[i + 1];
        vsPos--;
    }
    else
        printf("ERROR: Can't delete value at position %d - outside of valueStack\n", p);
}

/*
 * clear: Sets vsPos = 0, effectively clearing valueStack
 */
void clear() {
    vsPos = 0;
}