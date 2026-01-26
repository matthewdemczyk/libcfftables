#ifndef LIBCFFTABLES_H
#define LIBCFFTABLES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdio.h>


/* ============================================================================
 * Types
 * ============================================================================ */

 /**
 * @brief Opaque handle representing a Cover-Free Family (CFF).
 *
 * A cover-free family is a set system where the ground set has `t` elements,
 * the set system contains `n` subsets, and no subset is contained in the union
 * of any `d` other subsets.
 *
 * A cover-free family is stored as an incidenc matrix with `t` rows and `n` columns.
 *
 * The functions `cff_get_d(cff)`, `cff_get_t(cff)`, `cff_get_n(cff)`, can be
 * used to read the `d`,`t`, and `n` of the CFF. The `d` can be changed with
 * `cff_set_d(cff, d)`.
 *
 * Cells of the incidence matrix can be read with `cff_get_value(cff, row, col)`
 * or written to with `cff_set_value(cff, row, call, value)`, where `value = 0, 1`.
 *
 * A `cff_t` must be initialized functions such as
 * `cff_alloc`, `cff_from_matrix`, one of the construction routines,
 * or with `cff_table_get_by_t`/`cff_table_get_by_n` after initializing
 * a `cff_table_ctx_t` with `cff_table_create`.
 *
 * A `cff_t` must be released with `cff_free(cff)`, regardless of method
 * of initialization.
 *
 * All access to a `cff_t` is performed through the provided API
 * functions.
 */
typedef struct cff cff_t;

/**
 * @brief Opaque handle representing the CFF tables
 *
 * This is a table of best-known cover-free families from our selection of constructions.
 *
 * Best-known cover-free familiy means that for a given
 * `(d,n)`, `t` is minimized, or for a given `(d,t)`, `n` is maximized.
 *
 * The purpose of initializing this is to be able to use the functions
 * `cff_table_get_by_t` and `cff_table_get_by_n`, which will construct a CFF.
 */
typedef struct cff_table_ctx cff_table_ctx_t;

/* ============================================================================
 * Core CFF Operations
 * ============================================================================ */

/**
 * @brief Allocates a `cff_t`, which stores a `d-CFF(t,n)`, filled with zeros.
 *
 * @param d The `d` of the CFF.
 * @param t The number of rows in the CFF's incidence matrix.
 * @param n The number of columns in the CFF's incidence matrix.
 *
 * @return A newly allocated cff_t filled with zeros.
 */
cff_t* cff_alloc(int d, int t, long long n);
/**
 * @brief Frees a `cff_t` from memory.
 *
 * @param cff The `cff_t` to free.
 */
void cff_free(cff_t *cff);

/**
 * @brief Initialze a `cff_t` from an int matrix of 0s and 1s
 *
 * @param d The `d` of the CFF
 * @param t The number of rows of the CFF's incidence matrix
 * @param n The number of columns of the CFF's incidence matrix
 * @param matrix A row-major `int` array of 0s and 1s, with `t` rows and `n` columns
 * @returns A pointer to a newly allocated `d-CFF(t,n)` with cells set from `matrix`, or NULL on failure
 */
cff_t* cff_from_matrix(int d, int t, long long n, const int *matrix);

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

/**
 * @brief Constructs a CFF from an n x n identity matrix.
 *
 * @param d The `d` of the resulting CFF
 * @param n Number of rows and cols in the resulting CFF, must be greater than `d`
 * @return Pointer to a `d-CFF(n,n)`, or NULL on failure
 */
cff_t* cff_identity(int d, int n);
/**
 * @brief Constructs a CFF from a Sperner system.
 *
 * @param n Number of subsets in the sperner system/CFF
 * @return Pointer to newly allocated `1-CFF(t = min{s : choose(s, s/2) >= n }, n)`, or `NULL` on failure
 */
cff_t* cff_sperner(int n);
/**
 * @brief Constructs a 2-CFF from a Steiner Triple System.
 *
 * @param v Order of the STS
 * @returns Pointer to a `2-CFF(v, (v * (v - 1)) / 6)`, or `NULL` on failure
 */
cff_t* cff_sts(int v);
/**
 * TODO
 */
cff_t* cff_porat_rothschild(int p, int a, int k, int r, int m);
/**
 * @brief Constructs a CFF from a Reed-Solomon error correcting code
 */
cff_t* cff_reed_solomon(int p, int exp, int t, int m);
cff_t* cff_short_reed_solomon(int p, int exp, int k, int m, int s);
cff_t* get_fixed_cff(int d, int t);

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
cff_table_ctx_t* cff_table_create(int d_maximum, int t_maximum, long long n_maximum);

// frees global_tables_array's memory
void cff_table_free(cff_table_ctx_t *ctx);

// main recursive function to get a CFF after making the tables
cff_t* cff_table_get_by_t(cff_table_ctx_t *ctx, int d, int t);

// uses getByT to get a cff
cff_t* cff_table_get_by_n(cff_table_ctx_t *ctx, int d, int n);

// makes csv files in ./tables/ from global_tables_array
void cff_table_write_csv(cff_table_ctx_t *ctx, const char *folder_path);


#ifdef __cplusplus
}
#endif

#endif