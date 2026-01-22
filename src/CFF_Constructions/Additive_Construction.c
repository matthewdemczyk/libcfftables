#include "Construction_Function_Internals.h"

#include "../../include/libcfftables/libcfftables.h"
#include "../CFF_Internals.h"

#include <string.h>


void addShortSrcFormatter(char *strBuffer)
{
    strcpy(strBuffer, "Additive");
}

void addLongSrcFormatter(short *consParams, char *strBuffer)
{
    sprintf(strBuffer, "Add(%hd,%hd)", consParams[0], consParams[1]);
}

void addConstructCFF(int d, int t)
{
    cff_t *right = cff_table_get_by_t(d, global_tables_array[d-1]->array[t].consParams[0]);
    cff_t *left = cff_table_get_by_t(d, global_tables_array[d-1]->array[t].consParams[1]);
    global_tables_array[d-1]->array[t].cff = cff_additive(left, right);
}

CFF_Construction_And_Name_Functions addConstructionFunctions = {
    .shortSrcFormatter = addShortSrcFormatter,
    .longSrcFormatter = addLongSrcFormatter,
    .constructionFunction = addConstructCFF
};

cff_t* cff_additive(const cff_t *left, const cff_t *right)
{
    cff_t *result = cff_alloc(
        left->d,
        left->t + right->t,
        left->n + right->n
    );
    for (int r = 0; r < left->t; r++)
    {
        for (int c = 0; c < left->n; c++)
        {
            if (cff_get_value(left, r, c) == 1)
            {
                cff_set_value(result, r, c, 1);
            }
        }
    }
    for (int r = 0; r < right->t; r++)
    {
        for (int c = 0; c < right->n; c++)
        {
            if (cff_get_value(right, r, c) == 1)
            {
                cff_set_value(result, r + left->t, c + left->n, 1);
            }
        }
    }

    return result;
}

// the function to add additive construction parameters to the tables is in the file
// src/CFF_Constructions/Optimized_Kronecker_Construction.c
// function: void applyPairConstructions(CFF_Table *table, CFF_Table *d_minus_one_table, int cff_d);