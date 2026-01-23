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
    int stride_bits; // helper for faster memory access
    uint8_t *matrix;
};

typedef struct
{
    long long n;
    short consParams[5];
    short constructionID;
    cff_t *cff;
} CFF_Table_Row;

typedef struct
{
    bool hasBeenChanged;
    int d;
    int numCFFs;
    long long n_max; //need to store on each table because
                      //its accessed in updateTable
    CFF_Table_Row *array; //an array of length numCFFs + 1
} CFF_Table;

struct cff_table_ctx
{
    int d_max;
    int t_max;
    long long n_max;
    CFF_Table **tables_array;
};

unsigned long long choose(int n, int k);

long factorial(int n);

void setToFirstLexOrder(int k, int *buffer);

void setToAllZeros(int n, int *T);

bool kSubsetLexSuccessor(int n, int k, int *buffer);

bool nextLexicographicTuple(int n, int k, int *buffer);

int ipow(int base, int exp);

void prime_power_sieve(int n, bool prime_array[n], bool prime_power_array[n]);

// helper to search table for some row with a cff with at least n columns
int binarySearchTable(CFF_Table *table, long long n);

// called in loops to check constructions to update table with newly found CFFs
void updateTable(
    CFF_Table *table,
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