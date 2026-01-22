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
    int n;
    int stride_bits; // helper for faster memory access
    uint8_t *matrix;
};

extern unsigned d_max;
extern unsigned t_max;
extern CFF_n_type n_max;

typedef void (*TableSourceShortSrcFormatter)(char*);
typedef void (*TableSourceLongSrcFormatter)(short*, char*);
typedef void (*ConstructionFunction)(int, int);

typedef struct
{
    TableSourceShortSrcFormatter shortSrcFormatter;
    TableSourceLongSrcFormatter longSrcFormatter;
    ConstructionFunction constructionFunction;
} CFF_Construction_And_Name_Functions;

typedef struct
{
    CFF_n_type n;
    short consParams[5];
    CFF_Construction_And_Name_Functions *functions;
    cff_t *cff;
} CFF_Table_Row;

typedef struct
{
    bool hasBeenChanged;
    int d;
    int numCFFs;
    CFF_Table_Row *array; //an array of length numCFFs + 1
} CFF_Table;


// an array of pointers to tables
extern CFF_Table **global_tables_array;

unsigned long long choose(int n, int k);

long factorial(int n);

void setToFirstLexOrder(int k, int *buffer);

void setToAllZeros(int n, int *T);

bool kSubsetLexSuccessor(int n, int k, int *buffer);

bool nextLexicographicTuple(int n, int k, int *buffer);

int ipow(int base, int exp);

void prime_power_sieve(int n, bool prime_array[n], bool prime_power_array[n]);

// helper to search table for some row with a cff with at least n columns
int binarySearchTable(CFF_Table *table, CFF_n_type n);

// called in loops to check constructions to update table with newly found CFFs
void updateTable(
    CFF_Table *table,
    int t,
    CFF_n_type n,
    CFF_Construction_And_Name_Functions *cffFunctions,
    int consParam0,
    int consParam1,
    int consParam2,
    int consParam3,
    int consParam4
);

#endif