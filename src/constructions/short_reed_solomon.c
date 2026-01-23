#include "construction_internals.h"
#include "../../include/libcfftables/libcfftables.h"

#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "../CFF_Internals.h"
#include "finite_fields_wrapper.h"


void shortRsShortSrcFormatter(char *strBuffer)
{
    strcpy(strBuffer, "Shortened Reed-Solomon");
}

void shortRsLongSrcFormatter(short *consParams, char *strBuffer)
{
    sprintf(strBuffer, "SRS(%hd,%hd,%hd,%hd,%hd)", consParams[0], consParams[1], consParams[2], consParams[3], consParams[4]);
}

void shortRsConstructCFF(int d, int t)
{
        global_tables_array[d-1]->array[t].cff = cff_short_reed_solomon(
        global_tables_array[d-1]->array[t].consParams[0],
        global_tables_array[d-1]->array[t].consParams[1],
        global_tables_array[d-1]->array[t].consParams[2],
        global_tables_array[d-1]->array[t].consParams[3],
        global_tables_array[d-1]->array[t].consParams[4]
    );
}

CFF_Construction_And_Name_Functions shortRsConstructionFunctions = {
    .shortSrcFormatter = shortRsShortSrcFormatter,
    .longSrcFormatter = shortRsLongSrcFormatter,
    .constructionFunction = shortRsConstructCFF
};

cff_t* cff_short_reed_solomon(int p, int exp, int k, int m, int s)
{
    if (s == 0)
    {
        return cff_reed_solomon(p, exp, k, m);
    }
    int q = (int) pow(p, exp);
    int addition_field[q][q];
    int multiplication_field[q][q];
    populateFiniteField(p, exp, (int*) addition_field, (int*) multiplication_field);

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
    setToAllZeros(k, polynomialCoefficients);
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
            codeword[x + 1] = hornerPolynomialEvalOverFq(k, polynomialCoefficients, x, q, addition_field, multiplication_field);
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
                codeword[x+1] = hornerPolynomialEvalOverFq(k, polynomialCoefficients, x, q, addition_field, multiplication_field);
            }
            for (int i = s; i < m; i++)
            {
                cff_set_value(cff, ((i - s) * q) + codeword[i], cn, 1);
            }
            cn++;
        }
    } while (nextLexicographicTuple(q, k, polynomialCoefficients));
    return cff;
}