/* 03E03.c (p. 63) */
#include <stdio.h>
#include <ctype.h>
#define MAXSTRLEN 1000

typedef enum { False, True } bool;

void expand(char *s1, char *s2);

main() {
    char s01[] = "f-x", s02[MAXSTRLEN];
    char s11[] = "1-7", s12[MAXSTRLEN];
    char s21[] = "-h", s22[MAXSTRLEN];
    char s31[] = "2-", s32[MAXSTRLEN];
    char s41[] = "x-R", s42[MAXSTRLEN];
    char s51[] = "9-5", s52[MAXSTRLEN];
    char s61[] = "a-9", s62[MAXSTRLEN];
    char s71[] = "a-B-c-", s72[MAXSTRLEN];
    char s81[] = "A-z0-9", s82[MAXSTRLEN];
    char s91[] = "-a-z", s92[MAXSTRLEN];

    expand(s01, s02), expand(s11, s12), expand(s21, s22), expand(s31, s32),
        expand(s41, s42), expand(s51, s52), expand(s61, s62), expand(s71, s72),
        expand(s81, s82), expand(s91, s92);

    printf("s02 = expand(\"%s\") = %s\n", s01, s02);
    printf("s12 = expand(\"%s\") = %s\n", s11, s12);
    printf("s22 = expand(\"%s\") = %s\n", s21, s22);
    printf("s32 = expand(\"%s\") = %s\n", s31, s32);
    printf("s42 = expand(\"%s\") = %s\n", s41, s42);
    printf("s52 = expand(\"%s\") = %s\n", s51, s52);
    printf("s62 = expand(\"%s\") = %s\n", s61, s62);
    printf("s72 = expand(\"%s\") = %s\n", s71, s72);
    printf("s82 = expand(\"%s\") = %s\n", s81, s82);
    printf("s92 = expand(\"%s\") = %s\n", s91, s92);

    printf("\n");
    system("pause");
    return 0;
}

/*
 * expand: Expands shorthand notations like a-z in the string s1 into the equivalent
 *   complete list abc...xyz in s2. Allows for letters of either case and digits,
 *   and can handle cases like a-b-c and a-z0-9 and -a-z. Leading or trailing - is
 *   taken literally.
 */
void expand(char *s1, char *s2) {
    int i, j;
    for (i = 0, j = 0; s1[i] != '\0'; i++) {
        if (s1[i] == '-') {
            // See what's before and after the - in s1
            char start = i > 0 && isalnum(s1[i - 1]) ? s1[i - 1] : NULL;
            char end = s1[i + 1] != '\0' && isalnum(s1[i + 1]) ? s1[i + 1] : NULL;

            bool startNum = isdigit(start), endNum = isdigit(end);

            // Only expand if both aren't null
            if (!(startNum != endNum) && !(start == NULL && end == NULL)) {
                if (start == NULL) {
                    char c = endNum ? '0' : islower(end) ? 'a' : 'A';
                    for (c; c < end; c++)
                        s2[j++] = c;
                }
                else if (end == NULL) {
                    char c = startNum ? '9' : islower(start) ? 'z' : 'Z';
                    for (++start; start <= c; start++)
                        s2[j++] = start;
                }
                else {
                    start = islower(end) ? tolower(start) : toupper(start);
                    int step = start < end ? 1 : -1;
                    for (start += step; step * start < step * end; start += step)
                        s2[j++] = start;
                }
            }
            else {
                s2[j++] = s1[i];
            }
        }
        else {
            s2[j++] = s1[i];
        }
    }
    s2[j] = '\0';
}