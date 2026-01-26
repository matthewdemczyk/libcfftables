#include "construction_internals.h"
#include "../../include/libcfftables/libcfftables.h"
#include "../cff_internals.h"

#include <stddef.h>

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

void cff_table_add_pair_constructed_cffs(cff_table_ctx_t *ctx, int cff_d)
{
    int t, s;
    long long n;
    cff_table_t *table = ctx->tables_array[cff_d-1];
    cff_table_t *d_minus_one_table = ctx->tables_array[cff_d-2];
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
            update_table(table, t, n, CFF_CONSTRUCTION_ID_ADDITIVE, t1, t2, 0, 0, 0);

            // kronecker product
            t = t1 * t2;
            n = table->array[t1].n * table->array[t2].n;
            update_table(table, t, n, CFF_CONSTRUCTION_ID_KRONECKER, t1, t2, 0, 0, 0);

            // try optimized kronecker in both combinations (t1,t2) & (t2,t1)
            s = binary_search_table(d_minus_one_table, table->array[t2].n);
            if (s != -1)
            {
                t = (s * t1) + t2;
                n = table->array[t1].n * table->array[t2].n;
                update_table(table, t, n, CFF_CONSTRUCTION_ID_OPTIMIZED_KRONECKER, t1, t2, s, 0, 0);
            }
            // try the other way for opt kronecker
            s = binary_search_table(d_minus_one_table, table->array[t1].n);
            if (s != -1)
            {
                t = (s * t2) + t1;
                n = table->array[t1].n * table->array[t2].n;
                update_table(table, t, n, CFF_CONSTRUCTION_ID_OPTIMIZED_KRONECKER, t2, t1, s, 0, 0);
            }
        }
    }
}


