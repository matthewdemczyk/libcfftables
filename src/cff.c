#include "../include/libcfftables/libcfftables.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "cff_internals.h"

// if true, when running cff_verify(), it will print out which rows in
// the CFF the d+1 rows of the ID matrix are found on
#define CFF_VERIFY_VERBOSE_PRINTOUT false

// allocates a d-CFF(t,n) filled with 0s
cff_t* cff_alloc(int d, int t, long long n)
{
    cff_t* c = malloc(sizeof(cff_t));
    if (c == NULL) return NULL;
    c->d = d;
    c->t = t;
    c->n = n;
    c->stride_bits = (long long) (((n + 7) / 8) * 8);
    c->matrix = calloc(((n + 7) / 8) * t, sizeof(unsigned char));
    if (c->matrix == NULL) return NULL;
    return c;
}

// free a CFF from memory
void cff_free(cff_t *cff)
{
    if (cff!=NULL) free(cff->matrix);
    free(cff);
}

int cff_get_d(const cff_t *cff)
{
    if (!cff) return -1;
    return cff->d;
}

void cff_set_d(cff_t *cff, int d)
{
    if (!cff) return;
    cff->d = d;
}

int cff_get_t(const cff_t *cff)
{
     if (!cff) return -1;
    return cff->t;
}

long long cff_get_n(const cff_t *cff)
{
     if (!cff) return -1;
    return cff->n;
}

void cff_reduce_n(cff_t *cff, long long n)
{
    if (!cff) return;
    if (n < cff->n)
    {
        cff->n = n;
    }
}

const unsigned char* cff_matrix_data(const cff_t *cff)
{
    return cff ? cff->matrix : NULL;
}

long long cff_get_row_pitch_bits(const cff_t *cff)
{
    return cff ? cff->stride_bits : 0;
}

// setter for the row "r" and column "c" for a value in the 0-1 CFF Matrix.
void cff_set_matrix_value(cff_t *cff, int r, int  c, int val)
{
    long long bitIndex = (long long) (r * cff->stride_bits + c);
    if (val)
    {
        cff->matrix[bitIndex / 8] |= (1 << (bitIndex % 8));
    } else
    {
        cff->matrix[bitIndex / 8] &= ~(1 << (bitIndex % 8));
    }
}

// getter for the row "r" and column "c" for a value in the 0-1 CFF Matrix.
int cff_get_matrix_value(const cff_t *cff, int r, int c)
{
    long long bitIndex = (long long) (r * cff->stride_bits + c);
    return (cff->matrix[bitIndex / 8] >> (bitIndex % 8)) & 1;
}

// allocates memory for a d-CFF(t,n), sets its matrix to the passed matrix, then returns a pointer to it
// make sure to free the memory using cff_free() when finished with the CFF.
cff_t* cff_from_matrix(int d, int t, long long n, const int *matrix)
{
    if (d < 1 || t < 1 || n < 1) {
        return NULL;
    }
    if (matrix == NULL)
    {
        return NULL;
    }
    // validate that the matrix is actually zeros and ones first
    for (long long i = 0; i < (long long)t * n; i++) {
        if (matrix[i] != 0 && matrix[i] != 1) {
            return NULL;
        }
    }
    cff_t *cff = cff_alloc(d, t, n);
    for(int r  = 0; r < t; r++)
    {
        for(int c  = 0; c < n; c++)
        {
            cff_set_matrix_value(cff, r, c, matrix[r *n + c]);
        }
    }
    return cff;
}

cff_t* cff_copy(const cff_t *src)
{
    cff_t *cff = cff_alloc(
        src->d,
        src->t,
        src->n
    );
    for(int r = 0; r < src->t; r++)
    {
        for(int c = 0; c < src->n; c++)
        {
            cff_set_matrix_value(cff,r,c,
                cff_get_matrix_value(src,r,c)
            );
        }
    }
    return cff;
}


// prints a CFF to console
void cff_print(const cff_t *cff)
{
    printf("%d-CFF(%d,%lld):\n", cff->d, cff->t, cff->n);
    int value;
    for(int r = 0; r < cff->t; r++)
    {
        for(int c = 0; c < cff->n; c++)
        {
            value = cff_get_matrix_value(cff, r, c);
            if (value)
            {
                printf("1 ");
            } else
            {
                printf("- ");
            }
        }
        printf("\n");
    }
}

void cff_write(const cff_t *cff, FILE *file)
{
    int value;
    for(int r = 0; r < cff->t; r++)
    {
        for(int c = 0; c < cff->n; c++)
        {
            value = cff_get_matrix_value(cff,r,c);
            if (value)
            {
                fprintf(file, "1");
            } else
            {
                fprintf(file, "0");
            }
        }
        fprintf(file, "\n");
    }
}


// returns true if the CFF is a valid cover free family.
// returns false otherwise.
bool cff_verify(const cff_t *cff)
{
    if (cff->d+1 > cff->n)
    { // return false if the parameters are invalid
        return false;
    }

    // cols will be an array of columns of size d+1 to test
    int k = cff->d + 1;
    int cols[k];
    for (int i = 0; i < k; i++)
    { // set cols to the smallest lexicographic ordering
        cols[i] = i;
    }

    do
    {
        if (CFF_VERIFY_VERBOSE_PRINTOUT)
        { //print out the current columns that are being tested
            printf("Testing cols:  ");
            for (int x = 0; x < k; x++)
            {
                printf("%u  ", cols[x]);
            }
            printf("| ID Matrix found on rows:  ");
        }

        // verify array "v". keeps track of the first i.d. row for every col in "cols"  array
        int v[k];
        for (int x = 0; x < k; x++)
        {
            v[x] = -1;
        }

        // s is sum of the current row, e will be the most recent column where a 1 was seen
        int s, e;
        // f is the number of identity rows found so far (for this particular subset of columns)
        int f = 0;
        // iterate over the rows in the cff
        for (int r = 0; r < cff->t; r++)
        {
            // iterate over the subset of columns we are currently testing
            s = 0;
            for (int c = 0; c < k; c++)
            {
                // if the current cell is 1:
                if (cff_get_matrix_value(cff, r, cols[c]) == 1)
                { // record the position of the column and increment row sum
                    e = c;
                    s++;
                }
            }
            // check if the row was the first identity row for its column with a 1
            if (s == 1 && v[e] == -1)
            {
                if (CFF_VERIFY_VERBOSE_PRINTOUT) { printf("%u  ", r); }
                // record that c's identity row exists (by saving the row index)
                v[e] = (int) r;
                f++;
                if (f == k)
                { // exit early if all identity rows are found
                    break;
                }

            }
        }

        // now return false if any of the identity rows were not there
        if (f != k)
        {
            if (CFF_VERIFY_VERBOSE_PRINTOUT) { printf(" Some ID rows not found! CFF is invalid\n"); }
            return false;
        }
        if (CFF_VERIFY_VERBOSE_PRINTOUT) { printf("\n"); }
    } while (k_subset_lex_successor(cff->n, k, cols));
    return true;
}