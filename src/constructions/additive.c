#include "construction_internals.h"

#include "../../include/libcfftables/libcfftables.h"
#include "../CFF_Internals.h"


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
// src/constructions/Optimized_Kronecker_Construction.c
// function: void applyPairConstructions(CFF_Table *table, CFF_Table *d_minus_one_table, int cff_d);