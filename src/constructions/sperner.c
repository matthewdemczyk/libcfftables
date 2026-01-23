#include "construction_internals.h"
#include "../../include/libcfftables/libcfftables.h"

#include <string.h>

#include "../CFF_Internals.h"
#include "construction_internals.h"


void spernerShortSrcFormatter(char *strBuffer)
{
    strcpy(strBuffer, "Sperner");
}

void spernerLongSrcFormatter(short *consParams, char *strBuffer)
{
    sprintf(strBuffer, "Sp(%hd)", consParams[0]);
}

void spernerConstructCFF(int d, int t)
{
    global_tables_array[d-1]->array[t].cff = cff_sperner((int) global_tables_array[d-1]->array[t].n);
}

CFF_Construction_And_Name_Functions spernerConstructionFunctions = {
    .shortSrcFormatter = spernerShortSrcFormatter,
    .longSrcFormatter = spernerLongSrcFormatter,
    .constructionFunction = spernerConstructCFF
};

cff_t* cff_sperner(int n)
{
    // first, determine t = min{s : choose(s, s/2) >= n }
    int s = 0;
    while (((long long) choose(s, s / 2)) < n) // cast choose(s,s/2) to suppress compiler warnings
    {
        s++;
    }
    int t = s;

    // allocate memory for the CFF and set its matrix to all 0s
    cff_t *cff = cff_alloc(1, t, n);

    // create the buffer for the current t/2-subset of {1 ... t}
    // and populate it with the first (lexicographic) set
    int subset[t / 2];
    for (int i = 0; i < t / 2; i++)
    {
        subset[i] = i;
    }

    // iterate over all of the t/2-subsets and assign them to a column in the cff
    int col = 0;
    do
    {
        // iterate over the subset and assign its points to the current CFF row
        for (int i = 0; i < t / 2; i++)
        {
            if (subset[i] < t && col < n)
            {
                cff_set_value(cff, subset[i], col, 1);
            }
        }
        col++;
    } while (kSubsetLexSuccessor(t, t / 2, subset));
    return cff;
}