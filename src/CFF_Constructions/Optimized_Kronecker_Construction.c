#include "Construction_Function_Internals.h"
#include "../../include/libcfftables/libcfftables.h"
#include "../CFF_Internals.h"

#include <stddef.h>
#include <string.h>

void optKroneckerShortSrcFormatter(char *strBuffer)
{
    strcpy(strBuffer, "Optimized Kronecker");
}

void optKroneckerLongSrcFormatter(short *consParams, char *strBuffer)
{
    sprintf(strBuffer, "OKr(%hd,%hd,%hd)", consParams[0], consParams[1], consParams[2]);
}

void optKroneckerConstructCFF(int d, int t)
{
    cff_t* inner = cff_table_get_by_t(d, global_tables_array[d-1]->array[t].consParams[0]);
    cff_t* bottom = cff_table_get_by_t(d, global_tables_array[d-1]->array[t].consParams[1]);
    cff_t* outter = cff_table_get_by_t(d-1, global_tables_array[d-1]->array[t].consParams[2]);
    global_tables_array[d-1]->array[t].cff = cff_optimized_kronecker(outter, inner, bottom);
}

CFF_Construction_And_Name_Functions optKroneckerConstructionFunctions = {
    .shortSrcFormatter = optKroneckerShortSrcFormatter,
    .longSrcFormatter = optKroneckerLongSrcFormatter,
    .constructionFunction = optKroneckerConstructCFF
};

cff_t* cff_optimized_kronecker
(
    const cff_t* kronecker_outer, // (d-1)-CFF(s,  n2)
    const cff_t* kronecker_inner,  //     d-CFF(t1, n1)
    const cff_t* bottom_cff        //     d-CFF(t2, n2)
)
{
    // Verify that the parameters are valid
    if (kronecker_inner->d != bottom_cff->d)
    {
        return NULL;
    }
    if (kronecker_outer->d + 1 != kronecker_inner->d)
    {
        return NULL;
    }

    // Allocate memory for the product CFF, initialize its matrix to all 0s, and set its parameters (d,t,n)
    cff_t* product_cff = cff_alloc(
        kronecker_inner->d, // d
        (kronecker_outer->t * kronecker_inner->t) + bottom_cff->t, // t
        bottom_cff->n * kronecker_inner->n // n
    );

    // Construct the kronecker product of the first 2 CFFs
    for (int n1 = 0; n1 < bottom_cff->n; n1++)
    {
        for (int t1 = 0; t1 < kronecker_outer->t; t1++)
        {
            if (cff_get_value(kronecker_outer, t1, n1) == 1)
            {
                for (int n2 = 0; n2 < kronecker_inner->n; n2++) // first inner for loop
                {
                    for (int s = 0; s < kronecker_inner->t; s++)
                    {
                        if (cff_get_value(kronecker_inner, s, n2) == 1)
                        {
                            cff_set_value(
                                product_cff,
                                (t1 * kronecker_inner->t) + s,
                                (n1 * kronecker_inner->n) + n2,
                                1
                            );
                        }
                    }
                }
            }
        }
    }

    // Finally add in the bottom part
    int rows_above = kronecker_inner->t * kronecker_outer->t;
    for (int r = 0; r < bottom_cff->t; r++)
    {
        for (int c = 0; c < bottom_cff->n; c++)
        {
            if (cff_get_value(bottom_cff, r, c) == 1)
            {
                for (int col_repeat = 0; col_repeat < kronecker_inner->n; col_repeat++)
                {
                    cff_set_value(
                        product_cff,
                        r + rows_above,
                        (c*kronecker_inner->n) + col_repeat,
                        1
                    );
                }
            }
        }
    }
    return product_cff;
}

void applyPairConstructions(CFF_Table *table, CFF_Table *d_minus_one_table, int cff_d)
{
    int t, s;
    CFF_n_type n;
    for (int t1 = cff_d; t1 < table->numCFFs - 1; t1++)
    {
        for (int t2 = t1; t2 < table->numCFFs; t2++)
        {
            // addative construction
            t = t1 + t2;
            if (t > table->numCFFs)
            {
                break;
            }
            n = table->array[t1].n + table->array[t2].n;
            updateTable(table, t, n, &addConstructionFunctions, t1, t2, 0, 0, 0);

            // kronecker product
            t = t1 * t2;
            n = table->array[t1].n * table->array[t2].n;
            updateTable(table, t, n, &kroneckerConstructionFunctions, t1, t2, 0, 0, 0);

            // try optimized kronecker in both combinations (t1,t2) & (t2,t1)
            s = binarySearchTable(d_minus_one_table, table->array[t2].n);
            if (s != -1)
            {
                t = (s * t1) + t2;
                n = table->array[t1].n * table->array[t2].n;
                updateTable(table, t, n, &optKroneckerConstructionFunctions, t1, t2, s, 0, 0);
            }
            // try the other way for opt kronecker
            s = binarySearchTable(d_minus_one_table, table->array[t1].n);
            if (s != -1)
            {
                t = (s * t2) + t1;
                n = table->array[t1].n * table->array[t2].n;
                updateTable(table, t, n, &optKroneckerConstructionFunctions, t2, t1, s, 0, 0);
            }
        }
    }
}


