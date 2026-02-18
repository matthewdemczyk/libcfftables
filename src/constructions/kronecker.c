#include "construction_internals.h"
#include "../../include/libcfftables/libcfftables.h"
#include "../cff_internals.h"


cff_t* cff_kronecker(const cff_t *left, const cff_t *right)
{
    if (left == NULL || right == NULL) return NULL;
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

    if (product_cff == NULL) return NULL;

    // loop over right
    for (int n1 = 0; n1 < right->n; n1++)
    {
        for (int t1 = 0; t1 < right->t; t1++)
        {
            if (cff_get_matrix_value(right, t1, n1) == 1)
            {
                // loop over left
                for (int n2 = 0; n2 < left->n; n2++)
                {
                    for (int s = 0; s < left->t; s++)
                    {
                        if (cff_get_matrix_value(left, s, n2) == 1)
                        {
                            cff_set_matrix_value(
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
// function: void applyPairConstructions(cff_table_t *table, cff_table_t *d_minus_one_table, int cff_d);