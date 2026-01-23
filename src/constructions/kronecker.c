#include "construction_internals.h"
#include "../../include/libcfftables/libcfftables.h"
#include "../CFF_Internals.h"

#include <string.h>

void kroneckerShortSrcFormatter(char *strBuffer)
{
    strcpy(strBuffer, "Kronecker");
}

void kroneckerLongSrcFormatter(short *consParams, char *strBuffer)
{
    sprintf(strBuffer, "Kr(%hd,%hd)", consParams[0], consParams[1]);
}

void kroneckerConstructCFF(int d, int t)
{
    cff_t *right = cff_table_get_by_t(d, global_tables_array[d-1]->array[t].consParams[0]);
    cff_t *left = cff_table_get_by_t(d, global_tables_array[d-1]->array[t].consParams[1]);
    global_tables_array[d-1]->array[t].cff = cff_kronecker(left, right);
}

CFF_Construction_And_Name_Functions kroneckerConstructionFunctions = {
    .shortSrcFormatter = kroneckerShortSrcFormatter,
    .longSrcFormatter = kroneckerLongSrcFormatter,
    .constructionFunction = kroneckerConstructCFF
};

cff_t* cff_kronecker(const cff_t *left, const cff_t *right)
{
    // Verify if the cffs' parameters are valid
    if (left->d != right->d)
    {
        return NULL;
    }

    // Allocate memory for the product cff (and fills matrix with 0s)
    cff_t *product_cff = cff_alloc(
        left->d, // "d" of the product cff
        left->t * right->t, // "t" of the product cff
        left->n * right->n // "n" of the product cff
    );

    // loop over right
    for (int n1 = 0; n1 < right->n; n1++)
    {
        for (int t1 = 0; t1 < right->t; t1++)
        {
            if (cff_get_value(right, t1, n1) == 1)
            {
                // loop over left
                for (int n2 = 0; n2 < left->n; n2++)
                {
                    for (int s = 0; s < left->t; s++)
                    {
                        if (cff_get_value(left, s, n2) == 1)
                        {
                            cff_set_value(
                                product_cff,
                                (t1*left->t) + s,
                                (n1*left->n) + n2,
                                1
                            );
                        }
                    }
                }
            }
        }
    }

    return product_cff;
}

// the function to add kronecker product construction parameters to the tables is in the file
// src/constructions/Optimized_Kronecker_Construction.c
// function: void applyPairConstructions(CFF_Table *table, CFF_Table *d_minus_one_table, int cff_d);