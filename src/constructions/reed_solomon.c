#include "construction_internals.h"
#include "../../include/libcfftables/libcfftables.h"

#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "../CFF_Internals.h"
#include "finite_fields_wrapper.h"


void addReedSolomonCodes(CFF_Table *table, int cff_d, int t_max, bool *prime_array)
{
    long long cff_n;
    long long q;
    short m, short_m, short_k, e;
    int cff_t;
    for (short p = 2; (unsigned) p <= ceil(sqrt(t_max)); p++) //loop over q
    {
        if (prime_array[p])
        {
            q = p;
            e = 1;
            while (q <= t_max)
            {
                for (int k = 2; k < q + 1; k++)
                {
                    m = (cff_d * (k - 1)) + 1;
                    if (m > q + 1)
                    {
                        break;
                    }
                    cff_t = m * q;
                    if (cff_t < 0) break; //this means overflow happened in above line
                    cff_n = pow(q, k);
                    updateTable(table, cff_t, cff_n, CFF_CONSTRUCTION_ID_REED_SOLOMON, p, e, k, m, 0);
                    for (int s = 1; s + 1 <= q && s < m && s < k; s++)
                    {
                        short_m = m - s;
                        short_k = k - s;
                        cff_t = short_m * q;
                        cff_n = pow(q, short_k);
                        updateTable(table, cff_t, cff_n, CFF_CONSTRUCTION_ID_SHORT_REED_SOLOMON, p, e, k, m, s);
                    }
                }
                q *= p;
                e++;
            }
        }
    }
}

cff_t* cff_reed_solomon(int p, int exp, int t, int m)
{
    // populate finite field add and mult tables
    int q = pow(p, exp);
    int multiplication_field[q][q];
    int addition_field[q][q];
    populateFiniteField(p, exp, (int*) addition_field, (int*) multiplication_field);

    // allocate cff memory and fill with zeros
    cff_t *cff = cff_alloc(
        (m - 1) / (m - (m - t + 1)), // = d
        q * m,                       // = t
        ((int) pow(q, t))            // = n
    );

    // loop over all polynomials/codewords
    int polynomialCoefficients[t];
    setToAllZeros(t, polynomialCoefficients);
    int cn = 0; //codeword number
    int ln; //letter number
    do
    {
        ln = 0;
        //set * letter
        cff_set_value(cff, polynomialCoefficients[0], cn, 1);
        ln++;
        //evaluate polynomial from polynomialCoefficients with x=i over Fq using horner's method for poly eval
        for (int i = 0; i < m - 1; i++)
        {
            int polynomialSolution = hornerPolynomialEvalOverFq(t, polynomialCoefficients, i, q, addition_field, multiplication_field);
            cff_set_value(cff, (ln * q) + polynomialSolution, cn, 1);
            ln++;
        }
        cn++;
    } while (nextLexicographicTuple(q, t, polynomialCoefficients));

   return cff;
}