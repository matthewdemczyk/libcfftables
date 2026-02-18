#ifndef LIBCFFTABLES_INTERNAL_UTILS_HEADER
#define LIBCFFTABLES_INTERNAL_UTILS_HEADER

#include <stdbool.h>
#include "../include/libcfftables/libcfftables.h"

// a d-CFF(t, n):
// there are t subsets of the set n, stored as incidence matrix
struct cff
{
    int d;
    int t;
    long long n;
    long long stride_bits; // helper for faster memory access
    unsigned char *matrix;
};

typedef struct
{
    long long n;
    short consParams[5];
    short constructionID;
    cff_t *cff;
} cff_table_row_t;

typedef struct
{
    bool hasBeenChanged;
    int d;
    int numCFFs;
    int num_loops_when_creating;
    long long n_max; //need to store on each table because
                      //its accessed in updateTable
    cff_table_row_t *array; //an array of length numCFFs + 1
} cff_table_t;

struct cff_table_ctx
{
    int d_max;
    int t_max;
    long long n_max;
    cff_table_t **tables_array;
};

long long choose(int n, int k);

long factorial(int n);

void set_to_all_zeros(int n, int *T);

bool k_subset_lex_successor(int n, int k, int *buffer);

bool k_tuple_lex_successor(int n, int k, int *buffer);

int ipow(int base, int exp);

void prime_sieve(int n, bool *prime_array);

// helper to search table for some row with a cff with at least n columns
int binary_search_table(cff_table_t *table, long long n);

// called in loops to check constructions to update table with newly found CFFs
void update_table(
    cff_table_t *table,
    int t,
    long long n,
    int constructionID,
    int consParam0,
    int consParam1,
    int consParam2,
    int consParam3,
    int consParam4
);

#endif