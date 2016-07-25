/* 03E02.c (p. 60) */
#include <stdio.h>
#define MAXSTRLEN 1000

void escape(char *s, char *t);
void unescape(char *s ,char *t);

main() {
    char s[] = "This is a\ttest string that\nwill test converting\tescaped sequences like newline and tab\ninto visible escape sequences like '\\n' and '\\t'. Cool!";
    char t[MAXSTRLEN];
    
    escape(s, t);
    printf("%s\n\n", t);
    system("pause");

    printf("%s\n\n", t);

    printf("\n");
    system("pause");
    return 0;
}

/*
 * escape: Converts characters like newline and tab into visible escape sequences like 
 *         '\n' and '\t' as it copies the string t into s
 */
void escape(char *s, char *t) {
    int i, j;
    for (i = 0, j = 0; s[i] != '\0'; i++) {
        switch (s[i]) {
            case '\n':
                t[j++] = '\\';
                t[j++] = 'n';
                break;
            case '\t':
                t[j++] = '\\';
                t[j++] = 't';
                break;
            case '\\':
                t[j++] = '\\';
                t[j++] = '\\';
                break;
            case '\'':
                t[j++] = '\\';
                t[j++] = '\'';
                break;
            case '\"':
                t[j++] = '\\';
                t[j++] = '\"';
                break;
            default:
                t[j++] = s[i];
                break;
        }
    }
    t[j++] = '\\';
    t[j++] = '0';
    t[j] = '\0';
}

/*
 * unescape: Converts visible escape sequences like '\n' and '\t' into characters like
 *           newline and tab as it copies the string t into s.
 */
void unescape(char *s, char *t) {
    int i, j;
    for (i = 0, j = 0; s[i] != '\0'; i++) {
        switch (s[i]) {
            case '\\':
                i++;
                switch (s[i]) {
                    case 'n':
                        t[j++] = '\n';
                        break;
                    case 't':
                        t[j++] = '\t';
                        break;
                    case '\\':
                        t[j++] = '\\';
                        break;
                    case '\'':
                        t[j++] = '\'';
                        break;
                    case '\"':
                        t[j++] = '\"';
                        break;
                    case '0':
                        t[j++] = '\0';
                        break;
                }
                break;
            default:
                t[j++] = s[i];
                break;
        }
    }
    t[j] = '\0';
}