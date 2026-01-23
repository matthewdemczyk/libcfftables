#ifndef LIBCFFTABLES_H
#define LIBCFFTABLES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>


/* ============================================================================
 * Types
 * ============================================================================ */

typedef struct cff cff_t;
typedef struct cff_table_ctx cff_table_ctx_t;

/* ============================================================================
 * Core CFF Operations
 * ============================================================================ */

// allocates a CFF filled with zeros (must be freed with cff_free)
cff_t* cff_alloc(int d, int t, int n);
// frees a CoverFreeFamily from memory
void cff_free(cff_t *cff);

// allocates a d-CFF(t,n) and sets its values from a 0-1 int matrix (must be freed with cff_free)
cff_t* cff_from_matrix(int d, int t, int n, const int *matrix);

// allocates and makes a copy of a cff (must be freed with cff_free)
cff_t* cff_copy(const cff_t *src);

// cff struct getters/setters
int cff_get_d(const cff_t *cff);
int cff_get_t(const cff_t *cff);
int cff_get_n(const cff_t *cff);
void cff_set_d(cff_t *cff, int d);

// getters/setters for cells in a CFF's matrix
void cff_set_value(cff_t *cff, int r, int  c, int val);
int cff_get_value(const cff_t *cff, int r, int c);

// print/write cff
void cff_print(const cff_t *cff);
void cff_write(const cff_t *cff, FILE *file);

// verify if a CFF is valid
bool cff_verify(const cff_t *cff);

/* ============================================================================
 * Individual Construction Algorithms
 * ============================================================================ */

// direct constructions
cff_t* cff_identity(int n, int d);
cff_t* cff_sperner(int n);
cff_t* cff_sts(int v);
cff_t* cff_porat_rothschild(int p, int a, int k, int r, int m);
cff_t* cff_reed_solomon(int p, int exp, int t, int m);
cff_t* cff_short_reed_solomon(int p, int exp, int k, int m, int s);

// recursive constructions
cff_t* cff_additive(const cff_t *left, const cff_t *right);
cff_t* cff_doubling(const cff_t *cff, int s);
cff_t* cff_kronecker(const cff_t *left, const cff_t *right);
cff_t* cff_extend_by_one(const cff_t *cff);
cff_t* cff_optimized_kronecker
(
    const cff_t *kronecker_outer, // (d-1)-CFF(s,  n2)
    const cff_t *kronecker_inner,  //     d-CFF(t1, n1)
    const cff_t *bottom_cff        //     d-CFF(t2, n2)
);


/* ============================================================================
 * Table Generation
 * ============================================================================ */

// initializes global_tables_array (must be freed with freeGlobalTableArray)
void cff_table_create(unsigned d_maximum, unsigned t_maximum, unsigned long long n_maximum, bool printLoops, bool useBinConstWeightCodes);

// frees global_tables_array's memory
void cff_table_free();

// main recursive function to get a CFF after making the tables
cff_t* cff_table_get_by_t(int d, int t);

// uses getByT to get a cff
cff_t* cff_table_get_by_n(int d, int n);

// makes csv files in ./tables/ from global_tables_array
void cff_table_write_csv();


#ifdef __cplusplus
}
#endif

#endif