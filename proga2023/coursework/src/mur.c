#include "mur.h"
#include <stdio.h>
#include <string.h>
#define NO_OF_CHARS 256

int max(int a, int b) { return (a > b) ? a : b; }

void stop_symb_heuristic(char *str, int size, int stopchar[NO_OF_CHARS])
{
    int i;
    for (i = 0; i < NO_OF_CHARS; i++)
        stopchar[i] = -1;

    for (i = 0; i < size; i++)
        stopchar[(int)str[i]] = i;
}

int boyer_mur(char *txt, char *pat)
{
    int count = 0;
    int m = strlen(pat);
    int n = strlen(txt);

    int stopchar[NO_OF_CHARS];

    stop_symb_heuristic(pat, m, stopchar);

    int shift = 0;
    while (shift <= (n - m))
    {
        int j = m - 1;
        while (j >= 0 && pat[j] == txt[shift + j])
            j--;

        if (j < 0)
        {
            printf("Find occurs at shift %d\n",shift);
            count++;
            shift += (shift + m < n) ? m - stopchar[(int)txt[shift + m]] : 1;
        }
        else
            shift += max(1, j - stopchar[(int)txt[shift + j]]);
    }

    return count;
}
