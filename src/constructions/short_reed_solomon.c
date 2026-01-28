#include "construction_internals.h"
#include "../../include/libcfftables/libcfftables.h"

#include <stdbool.h>
#include <math.h>
#include <stdlib.h>

#include "../cff_internals.h"
#include "finite_fields_wrapper.h"


cff_t* cff_short_reed_solomon(int p, int exp, int k, int m, int s)
{
    if (s == 0)
    {
        return cff_reed_solomon(p, exp, k, m);
    }
    int q = (int) pow(p, exp);
    //int addition_field[q][q];
    //int multiplication_field[q][q];
    int *addition_field = malloc(q * q * sizeof(int));
    int *multiplication_field = malloc(q * q * sizeof(int));
    populate_finite_field(p, exp, (int*) addition_field, (int*) multiplication_field);

    int short_m = m - s;
    int short_k = k - s;

    int cff_d;
    // avoid division by zero:
    if (short_m - (short_m - short_k + 1) == 0)
    {
        cff_d = 1;
    } else
    {
        cff_d = (short_m - 1) / (short_m - (short_m-short_k + 1));
    }

    cff_t *cff = cff_alloc(
        cff_d,                // = d
        q * short_m,          // = t
        ((int) pow(q, short_k)) // = n
    );

    int polynomialCoefficients[k];
    int codeword[m];
    set_to_all_zeros(k, polynomialCoefficients);
    int cn = 0; //codeword number
    int numLeadingZeros;
    do
    {
        numLeadingZeros = 0;
        //set * letter
        codeword[0] = polynomialCoefficients[0];

        //evaluate polynomial from polynomialCoefficients with x=i over Fq using horner's method for poly eval
        int x;
        for (x = 0; x < s - 1; x++)
        {
            codeword[x + 1] = horner_polynomial_eval_over_fq(k, polynomialCoefficients, x, q, addition_field, multiplication_field);
        }
        for (int i = 0; i < s; i++)
        {
            if (codeword[i] == 0)
            {
                numLeadingZeros++;
            }
        }
        if (numLeadingZeros == s)
        {
            for (; x < m - 1; x++)
            {
                codeword[x+1] = horner_polynomial_eval_over_fq(k, polynomialCoefficients, x, q, addition_field, multiplication_field);
            }
            for (int i = s; i < m; i++)
            {
                cff_set_matrix_value(cff, ((i - s) * q) + codeword[i], cn, 1);
            }
            cn++;
        }
    } while (k_tuple_lex_successor(q, k, polynomialCoefficients));
    free(addition_field);
    free(multiplication_field);
    return cff;
}