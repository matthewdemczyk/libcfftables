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
 * of any `d` other subsets. d-cover-free families are also known as d-disjunct matrices. For more information
 * see https://en.wikipedia.org/wiki/Disjunct_matrix .
 *
 * The main feature of this library is the ability to provide a `(d,n)` and construct a CFF that minimizes
 * `t`, or provide a `(d,t)` and construct a CFF that maximizes `n` from our selection of CFF constructions.
 *
 * The tables that this library will generate are available at https://matthewdemczyk.github.io/CFFtables/
 * for values up to n=100 trillion and d=25. This library can generate larger values though.
 *
 * @section usage_sec Basic Usage
 *
 * To use the library:
 * 1. Initialize the CFF tables up to some maximum values, and construct a CFF from these tables
 * 2. Read the cells of the CFF's incidence matrix, for use in your application
 * 3. Free the CFF and tables when done
 *
 * Example:
 * @code
 * #include <libcfftables/libcfftables.h>
 *
 * int main() {
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
 *     // Iterate over the rows of the CFF's incidence matrix:
 *     for (int row = 0; row < t; row++) {
 *         // Iterate over the columns of the CFF's incidence matrix:
 *         for (int col = 0; col < n; col++) {
 *             int cell = cff_get_matrix_value(cff, row, col);
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
 * `n` for a given `(d,t)` or minimizes `t` for a given `(d,n)` from our selection of construcitons.
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
 * The functions `cff_get_d()`, `cff_get_t()`, `cff_get_n()`, can be
 * used to read the `d`,`t`, and `n` of the CFF. The `d` can be changed with
 * `cff_set_d()`.
 *
 * Cells of the incidence matrix can be read with `cff_get_matrix_value()`
 * or written to with `cff_set_matrix_value()`, where `value = 0, 1`.
 *
 * A `cff_t` must be initialized with functions such as
 * `cff_alloc()`, `cff_from_matrix()`, one of the construction routines,
 * or with `cff_table_get_by_t()`/`cff_table_get_by_n()` after initializing
 * a `cff_table_ctx_t` with `cff_table_create()`.
 *
 * A `cff_t` must be released with `cff_free()`, regardless of method
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
 * @param cff The `cff_t` to free. May be NULL (no-op).
 *
 * @post The memory pointed to by `cff` is freed and the pointer
 * becomes invalid. Caller should not use it afterward.
 */
void cff_free(cff_t *cff);
/**
 * @brief Initialize a `cff_t` from an int matrix of 0s and 1s.
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
 * @note After running, the user retains ownership of `matrix`. This function makes a deep copy.
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
 * @return The `d` of `cff`, or `-1` if `cff` is `NULL`.
 */
int cff_get_d(const cff_t *cff);
/**
 * @brief Getter for a `cff_t`'s `t`, the number of rows of its incidence matrix.
 *
 * @param cff The CFF to get `t` from.
 * @return The `t` of the `cff_t`, or `-1` if `cff` is `NULL`
 */
int cff_get_t(const cff_t *cff);
/**
 * @brief Getter for a `cff_t`'s `n`, the number of columns of its incidence matrix.
 *
 * @param cff The CFF to get `n` from.
 * @return The `n` of the `cff_t`, or `-1` if `cff` is `NULL`.
 */
long long cff_get_n(const cff_t *cff);
/**
 * @brief Helper function that can be used to reduce the n of a CFF
 *
 * This function modifies the stored value for n of a CFF. This does not change the
 * memory for the CFF's bitfield (incidence matrix), so the extra columns are still stored
 * in memory, they just can't be accessed after a CFF has its n reduced.
 *
 * This function can be useful if using cff_table_get_by_n, since the function will sometimes return
 * a CFF with larger n than requested. By reducing the n after getting the CFF, this can
 * make working with the CFF more simple.
 *
 * @param cff A pointer to the CFF to modify `n` for
 * @param n The new `n` value
 */
void cff_reduce_n(cff_t *cff, long long n);
/**
 * @brief Sets a `cff_t`'s `d`.
 *
 * This function can be useful to check if a `d-CFF` is a `(d+1)-CFF`
 * by using `cff_verify()` after this function.
 *
 * @param cff A pointer to the CFF whose `d` will be changed.
 * @param d The new `d` value for `cff`.
 *
 * @post If `cff` is not NULL, its `d` is updated to the passed value. If `cff` is NULL, nothing will happen.
 */
void cff_set_d(cff_t *cff, int d);
/**
 * @brief Getter for one cell of a `cff_t`'s incidence matrix.
 *
 * @warning The user must ensure that `cff` is not `NULL` before calling this. There is no check
 * inside of the function that does this. The user must also ensure:
 * `0 <= c < cff_get_n(cff)` and `0 <= r < cff_get_t(cff)` to avoid reading out of bounds.
 *
 * @param cff The CFF to get a cell's value from.
 * @param r The row of `cff`'s incidence matrix to get.
 * @param c The column of `cff`'s incidence matrix to get.
 *
 * @pre `cff` is not NULL.
 * @pre `0 <= c < cff_get_n(cff)`.
 * @pre `0 <= r < cff_get_t(cff)`.
 */
int cff_get_matrix_value(const cff_t *cff, int r, int c);
/**
 * @brief Sets one of the cells in a `cff_t`'s incidence matrix to zero or one.
 *
 * @warning The user must ensure that `cff` is not `NULL` before calling this. There is no check
 * inside of the function that does this. The user must also ensure:
 * `0 <= c < cff_get_n(cff)` and `0 <= r < cff_get_t(cff)` to avoid reading out of bounds.
 *
 * @param cff A pointer to the CFF to set a value in.
 * @param r The row of `cff`'s incidence matrix to set.
 * @param c The column of `cff`'s incidence matrix to set.
 * @param val The value, either `0` or `1`, to set the cell of the incidence matrix to.
 *
 * @pre `cff` is not NULL.
 * @pre `value` = `0` or `1`.
 * @pre `0 <= c < cff_get_n(cff)`.
 * @pre `0 <= r < cff_get_t(cff)`.
 */
void cff_set_matrix_value(cff_t *cff, int r, int  c, int val);
/**
 * @brief Prints a `cff_t` to stdout.
 *
 * The will first print `d-CFF(t,n)` where `d`,`t`,`n` are read from the `cff_t`.
 * The zeros of the CFF are printed as a `-` instead of `0`.
 *
 * @par Example:
 * @code
 * cff_t *cff = cff_sperner(6);
 * cff_print(cff);
 * // Output:
 * // 1-CFF(4,6):
 * // 1 1 1 - - -
 * // 1 - - 1 1 -
 * // - 1 - 1 - 1
 * // - - 1 - 1 1
 * @endcode
 * @param cff A pointer to the CFF that will be printed.
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
 * @brief Verify if a `cff_t` is a valid CFF.
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
 * @brief Best-known (from our selection of constructions) CFF lookup tables
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
 * A `cff_table_ctx_t` is initialized with the function `cff_table_create()`.
 *
 * The purpose of initializing this is to be able to use the functions
 * `cff_table_get_by_t()` and `cff_table_get_by_n()`, which will construct a CFF.
 *
 * @warning Access to a `cff_table_ctx_t` is not thread-safe. In multi-threaded applications,
 * each thread must initialize and maintain its own `cff_table_ctx_t` instance.
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
 *
 * @post The memory pointed to by `ctx` is freed and the pointer
 * becomes invalid. Caller should not use it afterward.
 */
void cff_table_free(cff_table_ctx_t *ctx);
/**
 * @brief Construct a CFF by supplying the desired `d` and `t`.
 *
 * This function will construct a best-known CFF by consulting the CFF tables
 * to know which construction to use. Best-known means maximizing `n` for the
 * given `(d,t)`
 *
 * @param ctx The CFF tables. Must be initialized with `cff_table_create()`.
 * @param d The `d` of the CFF.
 * @param t The number of rows of the desired CFF.
 *
 * @pre The values `d` and `t` appear in the tables.
 *
 * @return A pointer to a `cff_t` constructed from the tables with the given `d` and `t` values or NULL on failure.
 *
 * @note A user has ownership of the returned CFF and must free it themselves with `cff_free()`.
 */
cff_t* cff_table_get_by_t(cff_table_ctx_t *ctx, int d, int t);

/**
 * @brief Construct a CFF by supplying the desired `d` and `n`.
 *
 * @attention This function may return a CFF pointer with a larger `n` than the requested `n`. However,
 * you can ignore the extra columns of the CFF, because it's possible to remove/ignore any `x` columns from
 * a `d-CFF(t,n)` to get a valid `d-CFF(t,n-x)`.
 *
 * This function will construct a best-known CFF by consulting the CFF tables
 * to know which construction to use. Best-known means minimizing `t` for the
 * given `(d,n)`.
 *
 * @param ctx The CFF tables. Must be initialized with `cff_table_create()`.
 * @param d The `d` of the CFF.
 * @param n The number of columns of the desired CFF.
 *
 * @pre The values `d` and `n` appear in the tables.
 *
 * @return A pointer to a `cff_t` constructed from the tables with the given `d` and `n` values or NULL on failure.
 *
 * @note A user has ownership of the returned CFF and must free it themselves with `cff_free()`.
 */
cff_t* cff_table_get_by_n(cff_table_ctx_t *ctx, int d, int n);

/**
 * @brief Write the contents of the tables to CSV files.
 *
 * This function will make a file called d_1.csv, d_2.csv, up to the largest d in the tables,
 * Where each row of the CSV has the n of the resulting CFF, the construction name used,
 * and the parameters to the construction necessary to obtain this CFF. The `t` of the CFF is
 * the row number, so this number is not directly written to the CSV.
 *
 * @warning The user must ensure that `ctx` is not NULL before calling this function, and the
 * user must ensure that `folder_path` is a valid folder on the file system.
 *
 * @param ctx The `cff_table_ctx_t` to write to CSVs.
 * @param folder_path The path to write the various CSVs to. Can be a relative path.
 *
 * @pre `ctx` is not NULL.
 * @pre `folder_path` is an existing folder in the file system.
 */
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
 * @pre `d<n`.
 * @return Pointer to a `d-CFF(n,n)`, or NULL on failure.
 */
cff_t* cff_identity(int d, int n);
/**
 * @brief Constructs a 1-CFF from a Sperner system.
 *
 * @param n Number of subsets in the sperner system/CFF.
 * @return Pointer to newly allocated `1-CFF(t = min{s : choose(s, s/2) >= n }, n)`, or `NULL` on failure.
 */
cff_t* cff_sperner(int n);
/**
 * @brief Constructs a 2-CFF from a Steiner Triple System.
 *
 * @param v Order of the STS.
 *
 * @pre v is congruent to 1,3 mod 6.
 *
 * @returns Pointer to a `2-CFF(v, (v * (v - 1)) / 6)`, or `NULL` on failure.
 */
cff_t* cff_sts(int v);
/**
 * @brief Gets one of the hardcoded fixed CFFs.
 *
 * The hardcoded CFFs are only for `d = 2` and `t = 10 ... 23`.
 *
 * @param d The `d` of the fixed CFF.
 * @param t The `t` of the fixed CFF.
 *
 * @pre d = 2.
 * @pre 10 >= t >= 23.
 *
 * @return A pointer to a newly copied fixed CFF, or NULL on failure. The user gains ownership of
 * this and must free it after use with `cff_free()`.
 */
cff_t* cff_fixed(int d, int t);
/**
 * @brief Constructs a CFF from a Reed-Solomon error correcting code.
 *
 * The alphabet for the Reed-Solomon code will be size `q=p^exp`, message vector length
 * will be t, and codeword size will be `m`.
 *
 * @param p The prime of the alphabet.
 * @param exp The power of the alphabet.
 * @param t Message length of the Reed-Solomon code.
 * @param m Codeword size of the Reed-Solomon code.
 *
 * @pre `p` is prime.
 * @pre `exp` > 0.
 * @pre m <= (p^exp)+1.
 * @pre q >= t - 1 >= 0.
 *
 * @warning The caller must ensure the preconditions are true before calling this function, or there may
 * be undefined behaviour.
 *
 * @return Pointer to a `((m - 1) / (t - 1))-CFF(m*(p^exp), (p^exp)^t)`, or NULL on failure.
 */
cff_t* cff_reed_solomon(int p, int exp, int t, int m);
/**
 * @brief Constructs a CFF from a shortened Reed-Solomon error correcting code.
 *
 * The alphabet for the Reed-Solomon code will be size `q=p^exp`, message vector length
 * will be t, and codeword size will be `m`, and the code is shortened `s` times.
 *
 * @param p The prime of the alphabet.
 * @param exp The power of the alphabet.
 * @param t Message length of the Reed-Solomon code.
 * @param m Codeword size of the Reed-Solomon code.
 * @param s Number of times to shorten the code.
 *
 * @pre `p` is prime.
 * @pre `exp` > 0.
 * @pre m <= (p^exp)+1
 * @pre q >= t - 1 >= 0.
 * @pre 0 < s < m.
 *
 * @warning The caller must ensure the preconditions are true before calling this function, or there may
 * be undefined behaviour.
 *
 * @return Pointer to a `(((m-s) - 1) / ((t-s) - 1))-CFF((m-s)*(p^exp), (p^exp)^(t-s))`, or NULL on failure.
 */
cff_t* cff_short_reed_solomon(int p, int exp, int t, int m, int s);
/**
 * @brief Constructs a CFF using Porat and Rothschild's probabilistic linear error correcting code construction.
 *
 * See https://arxiv.org/abs/0712.3876 for more details.
 *
 * @note The CFFs from Reed-Solomon codes and others are better than this construction until `n`
 * grows extremely large (around `n = 2^370`), so this construction is not really used.
 *
 * @param p The prime of the code's alphabet `q = p^a`.
 * @param a The power of the prime's alphabet `q = p^a`.
 * @param k The message length of the code.
 * @param r The `d` of the desired CFF + 1 (so if you want a 3-CFF, r=4).
 * @param m The codeword length of the code. Set to zero to calculate the best possible allowed by the theorem.
 *
 * @pre `p` is prime.
 * @pre `a` > 0.
 * @pre m > k.
 * @pre `2r <= p^a < 4r.
 *
 * @return A pointer to a `cff_t`, a `(r-1)-CFF(m*(p^a),(p^a)^k)`, or NULL on failure.
 * @note Sometimes when reducing `m` to values smaller than the theorem allows, this still produces valid CFFs.
 * If this is done, it may become better than other constructions sooner than `n = 2^370`.
 */
cff_t* cff_porat_rothschild(int p, int a, int k, int r, int m);
/**
 * @brief A recursive construction that returns a new CFF with `t` and `n` increased by one.
 *
 * @param cff A pointer to a `cff_t`. Must be not NULL.
 * @return A pointer to a newly allocated `cff_t` with 1 larger `t` and `n` than the passed CFF.
 * @note The user retains ownership of the passed CFF and can free it without affecting the returned CFF.
 */
cff_t* cff_extend_by_one(const cff_t *cff);
/**
 * @brief A recursive construction that will make a new CFF with their `t` values added and their `n` values added together.
 *
 * For a given `d-CFF(t1,n1)` and `d-CFF(t2,n2)`, this function will return a `d-CFF(t1+t2,n1+n2)`.
 *
 * @param left The first `cff_t` to add.
 * @param right The other `cff_t` to add.
 *
 * @pre The `d` of the two CFFs is the same.
 *
 * @returns A pointer to the newly allocated constructed CFF.
 *
 * @note The user retains ownership of the passed CFFs and can free them without affecting the result CFF.
 */
cff_t* cff_additive(const cff_t *left, const cff_t *right);
/**
 * @brief Recursive construction for `d=2` that doubles the number of columns in a CFF.
 *
 * @param cff The `cff_t` to double.
 * @param s An integer satisfying s = min{s : choose(s, s/2) > cff_get_n(cff)}.
 *
 * @pre `s = min{s : choose(s, s/2) > cff_get_n(cff)}`.
 * @pre `cff_get_d(cff) = 2`
 * @warning The user must ensure the precondition on `s` is satisfied, there is no check inside the function.
 * @return A 2-CFF with double the columns as the passed 2-CFF, or NULL on failure.
 * @note The user retains ownership of the passed CFFs and can free them without affecting the result CFF.
 */
cff_t* cff_doubling(const cff_t *cff, int s);
/**
 * @brief A recursive construction that is the Kronecker product of two CFFs.
 *
 * For a given `d-CFF(t1,n1)` and `d-CFF(t2,n2)`, this function will return a `d-CFF(t1*t2,n1*n2)`.
 * See https://en.wikipedia.org/wiki/Kronecker_product for info.
 *
 * @param left The `cff_t` that is the left operand to a Kronecker product.
 * @param right The `cff_t` that is the right operand to a Kronecker product.
 *
 * @pre The `d` of the two CFFs is the same.
 *
 * @return A pointer to a `cff_t` that is the Kronecker product of the two CFFs, or NULL on failure.
 * @note The user retains ownership of the passed CFFs and can free them without affecting the result CFF.
 */
cff_t* cff_kronecker(const cff_t *left, const cff_t *right);
/**
 * @brief Constructs the optimized Kronecker product for three given CFFs.
 *
 * For a given `(d-1)-CFF(s, n2)`, `d-CFF(t1, n1)`, and a `d-CFF(t2, n2)`, this will construct a
 * `d-CFF(s*t1, n1*n2)`.
 *
 * @param kronecker_outer A pointer to a `cff_t` (d-1)-CFF(s,  n2).
 * @param kronecker_inner A pointer to a `cff_t` d-CFF(t1, n1).
 * @param bottom_cff A pointer to a `cff_t` d-CFF(t2, n2).
 *
 * @return A pointer to a `cff_t` d-CFF(s*t1, n1*n2), or NULL on failure.
 * @note The user retains ownership of the passed CFFs and can free them without affecting the result CFF.
 */
cff_t* cff_optimized_kronecker
(
    const cff_t *kronecker_outer, // (d-1)-CFF(s,  n2)
    const cff_t *kronecker_inner,  //     d-CFF(t1, n1)
    const cff_t *bottom_cff        //     d-CFF(t2, n2)
);
/** @} */ // end of construction group

/**
 * @defgroup internal Advanced / low-level accessors
 * @warning These expose the internal representation.
 *          They are intended for high-performance bindings.
 * @{
 */

 /**
  * @brief Returns a pointer to the bitfield for the CFF's matrix
  *
  * @warning This function is intended for high performance bindings. A user should typically use
  * `cff_get_matrix_value()` instead.
  *
  * The matrix is stored in row-major order with t rows. Each row occupies cff_get_row_pitch_bits() bits.
  * However only the first `n` bits of the row are used. The extra bits (less than or equal to 7 bits) are
  * used to ensure byte alignment.
  *
  * @note The returned pointer is owned by the CFF and remains valid until cff_free() is called.
  *
  * @param cff The CFF whose bitfield to access.
  * @return A pointer to the CFF's bitfield, or NULL on failure.
  */
const unsigned char* cff_matrix_data(const cff_t *cff);

/**
 * @brief Get the row pitch, in bits, of the CFF’s incidence matrix.
 *
 * Returns the number of bits between the start of consecutive rows in the incidence matrix.
 * This value is a multiple of 8 to ensure byte alignment.
 *
 * @param cff The CFF to access.
 * @return The the row pitch, in bits, of the CFF’s incidence matrix.
 */
long long cff_get_row_pitch_bits(const cff_t *cff);

/** @} */

#ifdef __cplusplus
}
#endif

#endif