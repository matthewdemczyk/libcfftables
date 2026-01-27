/**
 * @file libcfftables.h
 * @brief Public API for libcfftables - Cover-Free Family construction and manipulation
 * @mainpage libcfftables Documentation
 *
 * @section intro_sec Introduction
 *
 * libcfftables is a library for constructing and manipulating Cover-Free Families (CFFs).
 *
 * A cover-free family, denoted `d-CFF(t,n)` is a set system where the ground set has `t` elements,
 * the set system contains `n` subsets, and no subset is contained in the union
 * of any `d` other subsets.
 *
 * The tables that this library will generate are available at https://matthewdemczyk.github.io/CFFtables/
 *
 * @section usage_sec Basic Usage
 *
 * To use the library:
 * 1. Create a CFF using one of the construction functions or the table API
 * 2. Manipulate or verify the CFF using the provided operations
 * 3. Free the CFF when done
 *
 * Example:
 * @code
 * #include <libcfftables/libcfftables.h>
 *
 * int main(){
 *     // Initialize the tables by providing max d=3, t=100, n=2000 values allowed
 *     cff_table_ctx_t *ctx = cff_table_create(3, 100, 2000);
 *
 *     // Construct a CFF
 *     int d = 3;
 *     int t = 20;
 *     cff_t *cff = cff_table_get_by_t(ctx, d, t); //get a CFF with d=3, t=20, and maximum known n
 *     int n = cff_get_n(cff);
 *
 *     // ...your CFF application code here!...
 *     cff_print(cff);
 *
 *     // iterate over the rows of the CFF:
 *     for (int r = 0; r < t; r++) {
 *         // iterate over the columns of the CFF:
 *         for (int c = 0; c < n; c++) {
 *             int cell = cff_get_value(cff, r, c);
 *             // ...do something with the cell here...
 *         }
 *     }
 *
 *     // Clean up
 *     cff_free(cff);
 *     cff_table_free(ctx);
 * }
 * @endcode
 *
 * @section api_sec API Overview
 *
 * The API is organized into several groups:
 * - Core CFF Operations: Manipulation for a constructed CFF, including reading cells of its incidence matrix.
 * - Table Generation: Best-known CFF lookup tables. This will constrct a CFF that maximizes
 * `n` for a given `(d,t)` or minimizes `t` for a given `(d,n)`
 * - Construction Algorithms: Various methods to construct CFFs.
 * Only useful if you need a CFF from a specific construction for some reason. Otherwise use the tables.
 */


#ifndef LIBCFFTABLES_H
#define LIBCFFTABLES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdio.h>


/* ============================================================================
 * Core CFF Operations
 * ============================================================================ */
/**
 * @defgroup core Core CFF Operations
 * @brief Basic allocation, copying, and manipulation of CFF objects
 * @{
 */
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
 * @brief Allocates a `cff_t`, which stores a `d-CFF(t,n)`, filled with zeros.
 *
 * @note A user generally has no need for this function. The construction functions will allocate space.
 *
 * @param d The `d` of the CFF.
 * @param t The number of rows in the CFF's incidence matrix.
 * @param n The number of columns in the CFF's incidence matrix.
 *
 * @pre d < n.
 *
 * @return A pointer to a newly allocated `cff_t` filled with zeros.
 */
cff_t* cff_alloc(int d, int t, long long n);
/**
 * @brief Frees a `cff_t` from memory.
 *
 * @param cff The `cff_t` to free.
 */
void cff_free(cff_t *cff);
/**
 * @brief Initialze a `cff_t` from an int matrix of 0s and 1s.
 *
 * @param d The `d` of the CFF.
 * @param t The number of rows of the CFF's incidence matrix.
 * @param n The number of columns of the CFF's incidence matrix.
 * @param matrix A row-major `int` array of 0s and 1s, with `t` rows and `n` columns.
 *
 * @pre d < n.
 * @pre matrix's values are only 0 or 1.
 *
 * @returns A pointer to a newly allocated `d-CFF(t,n)` with cells set from `matrix`, or NULL on failure.
 *
 * @note After running the user retains ownership of `matrix`. This function makes a deep copy.
 */
cff_t* cff_from_matrix(int d, int t, long long n, const int *matrix);
/**
 * @brief Creates a deep copy of a `cff_t`.
 *
 * @param src The `cff_t` that will be copied.
 *
 * @return A pointer to a newly allocated `cff_t`, or NULL on failure.
 */
cff_t* cff_copy(const cff_t *src);
/**
 * @brief Getter for a `cff_t`'s d.
 *
 * @param cff The CFF to get `d` from.
 *
 * @return The `d` of `cff`, or `-1` if `cff` is `NULL`
 */
int cff_get_d(const cff_t *cff);
/**
 * @brief Getter for a `cff_t`'s `t`, the number of rows of its incidence matrix.
 *
 * @param cff The CFF to get `t` from.
 * @return The `t` of the `cff_t`.
 */
int cff_get_t(const cff_t *cff);
/**
 * @brief Getter for a `cff_t`'s `n`, the number of columns of its incidence matrix.
 *
 * @param cff The CFF to get `n` from.
 * @return The `n` of the `cff_t`.
 */
int cff_get_n(const cff_t *cff);
/**
 * @brief Sets a `cff_t`'s `d`.
 *
 * This function can be useful to check if a `d-CFF` is a `(d+1)-CFF`
 * by using `cff_verify()` after this function.
 *
 * @param cff A pointer to the CFF whose `d` will be changed
 * @param d The new `d` value for `cff`
 */
void cff_set_d(cff_t *cff, int d);
/**
 * @brief Sets one of the cells in a `cff_t`'s incidence matrix to zero or one
 *
 * @warning The user must ensure that `cff` is not `NULL` before calling this. There is no check
 * inside of the function that does this. The user must also ensure:
 * `0 <= c < cff_get_n(cff)` and `0 <= r < cff_get_t(t)` to avoid reading out of bounds.
 *
 * @param cff A pointer to the CFF to set a value in
 * @param r The row of `cff`'s incidence matrix to set
 * @param c The column of `cff`'s incidence matrix to set
 * @param val The value, either `0` or `1`, to set the cell of the incidence matrix to
 *
 * @pre `value` = `0` or `1`
 * @pre `0 <= c < cff_get_n(cff)`
 * @pre `0 <= r < cff_get_t(t)`
 */
void cff_set_value(cff_t *cff, int r, int  c, int val);
/**
 * @brief Getter for one cell of a `cff_t`'s incidence matrix
 *
 * @warning The user must ensure that `cff` is not `NULL` before calling this. There is no check
 * inside of the function that does this. The user must also ensure:
 * `0 <= c < cff_get_n(cff)` and `0 <= r < cff_get_t(t)` to avoid reading out of bounds.
 *
 * @param cff The CFF to get a cell's value from
 * @param r The row of `cff`'s incidence matrix to get
 * @param c The column of `cff`'s incidence matrix to get
 */
int cff_get_value(const cff_t *cff, int r, int c);
/**
 * @brief Prints a `cff_t` to stdin.
 *
 * The fill first print `d-CFF(t,n)` where `d`,`t`,`n` are read from the `cff_t`.
 * The zeros of the CFF are printed as a `-` instead of `0`.
 *
 * @param cff A pointer to the cff that will be printed
 */
void cff_print(const cff_t *cff);
/**
 * @brief Writes a `cff_t`'s incidence matrix to a file.
 *
 * @param cff A pointer to the cff that will be written to a file.
 * @param[out] file A pointer to a file. If the file exists it is overwritten.
 */
void cff_write(const cff_t *cff, FILE *file);
/**
 * @brief Verify if a `cff_t` is a valid CFF
 *
 * This function iterates of all n choose d+1 subset of columns an ensures that
 * each 1-weight d+1 size tuple is present in each subset of columns.
 *
 * @param cff The CFF to verify.
 *
 * @return true if the CFF is valid, false otherwise.
 */
bool cff_verify(const cff_t *cff);
/** @} */ // end of core group

/* ============================================================================
 * Table Generation
 * ============================================================================ */
/**
 * @defgroup tables Table Generation
 * @brief Best-known CFF lookup tables
 * @{
 */
/**
 * @brief Opaque handle representing the CFF tables.
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
/**
 * @brief Initializes the tables of best-known CFFs from our selection of constructions.
 *
 * The function will check all possible CFFs that can be constructed from our selection of
 * construction. There will be `d_maximum` different tables. All of the created tables are stored in
 * the returned `cff_table_ctx_t`. There will be a row for each `t` up to the `t_maximum` parameter,
 * that stores information about how to construct the CFF. This function does not construct
 * all of the CFFs. It makes a table that knows how to construct them.
 *
 * After creating the tables, CFFs can be constructed from the table by using the functions
 * `cff_table_get_by_n()` and `cff_table_get_by_t()`.
 *
 * The tables should be freed once they are not needed anymore with `cff_table_free()`.
 *
 * @param d_maximum The maximum `d` that will appear in the tables.
 * @param t_maximum The maximum `t` that will appear in the tables.
 * @param n_maximum The maximum `n` that will appear in the tables.
 *
 * @return A `cff_table_ctx_t` that stores the created tables.
 */
cff_table_ctx_t* cff_table_create(int d_maximum, int t_maximum, long long n_maximum);
/**
 * @brief Frees a `cff_table_ctx_t` from memory
 *
 * This frees the tables and all associated data after they've been
 * initialized with `cff_table_create()`.
 *
 * @param ctx The `cff_table_ctx_t` to free
 */
void cff_table_free(cff_table_ctx_t *ctx);
/**
 * @brief Construct a CFF by supplying the desired `d` and `t`.
 *
 * This function will construct a best-known CFF by consulting the CFF tables
 * to know which construction to use.
 *
 * @param ctx The CFF tables. Must be initialized with `cff_table_create()`.
 * @param d The `d` of the CFF.
 * @param t The number of rows of the desired CFF.
 */
cff_t* cff_table_get_by_t(cff_table_ctx_t *ctx, int d, int t);

// uses getByT to get a cff
cff_t* cff_table_get_by_n(cff_table_ctx_t *ctx, int d, int n);

// makes csv files in ./tables/ from global_tables_array
void cff_table_write_csv(cff_table_ctx_t *ctx, const char *folder_path);
/** @} */ // end of tables group

/* ============================================================================
 * Individual Construction Algorithms
 * ============================================================================ */
/**
 * @defgroup construction Construction Algorithms
 * @brief Various methods to construct cover-free families
 * @{
 */
/**
 * @brief Constructs a CFF from an n x n identity matrix.
 *
 * @param d The `d` of the CFF. Must be positive and less than `n`.
 * @param n Number of rows and cols in the resulting CFF, must be positive and greater than `d`.
 * @return Pointer to a `d-CFF(n,n)`, or NULL on failure.
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
 *
 * @pre v is congruent to 1,3 mod 6.
 *
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
cff_t* cff_fixed(int d, int t);

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
/** @} */ // end of construction group

#ifdef __cplusplus
}
#endif

#endif