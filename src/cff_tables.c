#include "../include/libcfftables/libcfftables.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "cff_internals.h"

#include "constructions/construction_internals.h"

// this is used in cff_table_get_by_t_rec to keep track of
// all itermediate CFFs created, then the CFFs are free'd
// after the final CFF is constructed
typedef struct intermediate_cffs_list {
    int d;
    int t;
    struct intermediate_cffs_list *next;
} intermediate_cffs_list_t;

cff_t* cff_table_get_by_t_rec(cff_table_ctx_t *ctx, int d, int t, intermediate_cffs_list_t **lst)
{
    cff_t *cff;
    if (ctx->tables_array[d-1]->array[t].cff == NULL)
    {
        // setup this node in intermediate_cffs_list_t so itermediate CFFs
        // can be freed once to final CFF is constructed
        intermediate_cffs_list_t *node = malloc(sizeof(intermediate_cffs_list_t));
        if (node == NULL)
        {   // handle malloc failure
            // clean up the list we've built so far
            intermediate_cffs_list_t *curr = *lst;
            while (curr != NULL) {
                intermediate_cffs_list_t *next = curr->next;
                free(curr);
                curr = next;
            }
            return NULL;
        }
        node->d = d;
        node->t = t;
        node->next = *lst; //prepend node to linked list
        *lst = node;
        // now: construct the CFF according to which construction this
        // row of the table corresponds to
        switch (ctx->tables_array[d-1]->array[t].constructionID)
        {
        case CFF_CONSTRUCTION_ID_IDENTITY_MATRIX:
            cff = cff_identity(d, t);
            break;
        case CFF_CONSTRUCTION_ID_SPERNER:
            cff = cff_sperner((int) ctx->tables_array[d-1]->array[t].n);
            break;
        case CFF_CONSTRUCTION_ID_STS:
            cff = cff_sts(t);
            break;
        case CFF_CONSTRUCTION_ID_PORAT_ROTHSCHILD:
            cff = cff_porat_rothschild(
                (int) ctx->tables_array[d-1]->array[t].consParams[0],
                (int) ctx->tables_array[d-1]->array[t].consParams[1],
                (int) ctx->tables_array[d-1]->array[t].consParams[2],
                (int) ctx->tables_array[d-1]->array[t].consParams[3],
                (int) ctx->tables_array[d-1]->array[t].consParams[4]
            );
            break;
        case CFF_CONSTRUCTION_ID_REED_SOLOMON:
            cff = cff_reed_solomon(
                (int) ctx->tables_array[d-1]->array[t].consParams[0],
                (int) ctx->tables_array[d-1]->array[t].consParams[1],
                (int) ctx->tables_array[d-1]->array[t].consParams[2],
                (int) ctx->tables_array[d-1]->array[t].consParams[3]
            );
            break;
        case CFF_CONSTRUCTION_ID_SHORT_REED_SOLOMON:
            cff = cff_short_reed_solomon(
                (int) ctx->tables_array[d-1]->array[t].consParams[0],
                (int) ctx->tables_array[d-1]->array[t].consParams[1],
                (int) ctx->tables_array[d-1]->array[t].consParams[2],
                (int) ctx->tables_array[d-1]->array[t].consParams[3],
                (int) ctx->tables_array[d-1]->array[t].consParams[4]
            );
            break;
        case CFF_CONSTRUCTION_ID_FIXED_CFF:
            cff = cff_fixed(d, t);
            break;
        case CFF_CONSTRUCTION_ID_EXT_BY_ONE: {
            // this can be improved a lot by checking how many ext by ones it will do in advance, then
            // just doing an additive with a certain size identity matrix
            cff_t *to_extend = cff_table_get_by_t_rec(ctx, d, ctx->tables_array[d-1]->array[t].consParams[0], lst);
            cff = cff_extend_by_one(to_extend);
            break;
        }
        case CFF_CONSTRUCTION_ID_ADDITIVE: {
            cff_t *right_k = cff_table_get_by_t_rec(ctx, d, ctx->tables_array[d-1]->array[t].consParams[0], lst);
            cff_t *left_k = cff_table_get_by_t_rec(ctx, d, ctx->tables_array[d-1]->array[t].consParams[1], lst);
            cff = cff_additive(left_k, right_k);
            break;
        }
        case CFF_CONSTRUCTION_ID_DOUBLING: {
            cff_t *smallerCFF = cff_table_get_by_t_rec(ctx, 2, ctx->tables_array[1]->array[t].consParams[0], lst);
            cff = cff_doubling(smallerCFF, (int) ctx->tables_array[1]->array[t].consParams[1]);
            break;
        }
        case CFF_CONSTRUCTION_ID_KRONECKER: {
            cff_t *left = cff_table_get_by_t_rec(ctx, d, ctx->tables_array[d-1]->array[t].consParams[0], lst);
            cff_t *right = cff_table_get_by_t_rec(ctx, d, ctx->tables_array[d-1]->array[t].consParams[1], lst);
            cff = cff_kronecker(left, right);
            break;
        }
        case CFF_CONSTRUCTION_ID_OPTIMIZED_KRONECKER: {
            cff_t* inner = cff_table_get_by_t_rec(ctx, d, ctx->tables_array[d-1]->array[t].consParams[0], lst);
            cff_t* bottom = cff_table_get_by_t_rec(ctx, d, ctx->tables_array[d-1]->array[t].consParams[1], lst);
            cff_t* outer = cff_table_get_by_t_rec(ctx, d-1, ctx->tables_array[d-1]->array[t].consParams[2], lst);
            cff = cff_optimized_kronecker(outer, inner, bottom);
            break;
        }
        default:
            break;
        }
        ctx->tables_array[d-1]->array[t].cff = cff;
    } else
    {
        cff = ctx->tables_array[d-1]->array[t].cff;
    }
    return cff;
}

cff_t* cff_table_get_by_t(cff_table_ctx_t *ctx, int d, int t)
{
    if (d < 1 || t < 1) return NULL;
    if (ctx == NULL) return NULL;
    if (d > ctx->d_max) return NULL;
    if (t >= ctx->tables_array[d-1]->numCFFs) return NULL;
    // setup linked list of intermediate CFFs needed in construction
    // so we can free them later without iterating over the entire table
    intermediate_cffs_list_t *head = NULL;

    // do the constructions
    cff_t *cff = cff_table_get_by_t_rec(ctx, d, t, &head);

    // free itermediate cffs
    intermediate_cffs_list_t *curr = head;
    while (curr != NULL)
    {
        intermediate_cffs_list_t *next = curr->next;
        // dont free the one we want constructed!
        if(!(curr->t == t && curr->d ==d))
        {
            cff_free(ctx->tables_array[curr->d-1]->array[curr->t].cff);
        }
        // always set this reference to NULL since the caller of this should
        // free the cff themselves (otherwise table has a dangling pointer after cff_free)
        ctx->tables_array[curr->d-1]->array[curr->t].cff = NULL;
        free(curr);
        curr = next;
    }
    return cff;
}

cff_t* cff_table_get_by_n(cff_table_ctx_t *ctx, int d, int n)
{
    if (d < 1 || n < 1) return NULL;
    if (ctx == NULL) return NULL;
    if (d > ctx->d_max) return NULL;
    int t = binary_search_table(ctx->tables_array[d-1], n);
    if (t == -1) return NULL;
    return cff_table_get_by_t(ctx, d, t);
}

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
)
{
    // if t fits, and the n the n is better: (it should go in the table)
    if ((t < table->numCFFs) && (n > table->array[t].n) && table->array[t].n != table->n_max)
    {
        // edge case for when the cff is bigger than n_max
        if ((n >= table->n_max))
        {
            table->array[t].n = table->n_max;
            table->numCFFs = t+1;
        }
        else
        {
            table->array[t].n = n;
        }
        table->array[t].consParams[0] = (short) consParam0;
        table->array[t].consParams[1] = (short) consParam1;
        table->array[t].consParams[2] = (short) consParam2;
        table->array[t].consParams[3] = (short) consParam3;
        table->array[t].consParams[4] = (short) consParam4;
        table->array[t].constructionID = (short) constructionID;
        table->hasBeenChanged = true;
    }
}

int binary_search_table(cff_table_t *table, long long n) {
    int low = 0;
    int high = table->numCFFs - 1;

    if (table->array[high].n < n) {
        return -1; // n is not present in the range
    }

    while (low <= high) {
        // continue until low == high
        int mid = low + (high - low) / 2; // avoid overflow

        if (table->array[mid].n == n)
        {
            return mid;
        }

        if (table->array[mid].n > n) {
            high = mid - 1; // move high down
        } else {
            low = mid + 1; // move low up
        }
    }

    return low;
}

// helper used in makeTables()
cff_table_t* initializeTable(int numCFFs, int cff_d, long long n_max)
{
    cff_table_t *table = malloc(sizeof(cff_table_t));
    if (table == NULL)
    {
        printf("malloc fail'd table in initializeTable\n");
        exit(1);
    }
    table->hasBeenChanged = false;
    table->numCFFs = numCFFs;
    table->d = cff_d;
    table->n_max = n_max;
    // allocate space for the t=0 cff because this array should be indexed starting at 1
    table->array = malloc(sizeof(cff_table_row_t)*(table->numCFFs+1));
    if (table->array == NULL)
    {
        printf("malloc fail'd table->array in initializeTable\n");
        exit(1);
    }
    // fill table with ID matices
    for (int t = 0; t < numCFFs + 1; t++)
    {
        table->array[t].n = t;
        table->array[t].consParams[0] = t;
        table->array[t].consParams[1] = 0;
        table->array[t].consParams[2] = 0;
        table->array[t].consParams[3] = 0;
        table->array[t].consParams[4] = 0;
        table->array[t].constructionID = (short) CFF_CONSTRUCTION_ID_IDENTITY_MATRIX;
        table->array[t].cff = NULL;
    }
    return table;
}

// sperners are always optimal 1-CFFs so this is a special case (no need to start with ID matrices)
cff_table_t* makeSpernerTable()
{
    cff_table_t *table = initializeTable(67, 1, 0);
    for (int t = 4; t <= table->numCFFs; t++)
    {
        table->array[t].n = choose(t, t/2);
        table->array[t].consParams[0] = t;
        table->array[t].constructionID = (short) CFF_CONSTRUCTION_ID_SPERNER;
    }
    table->num_loops_when_creating = 1;
    return table;
}

cff_table_ctx_t* cff_table_create(int d_maximum, int t_maximum, long long n_maximum)
{
    // save memory:
    if (t_maximum > n_maximum)
    {
        t_maximum = n_maximum;
    }

    // setup tables ctx
    // the tables ctx stores an array of pointers to each table, and
    // also stores the max d, t, and n allowed in the tables
    cff_table_ctx_t *ctx = malloc(sizeof(cff_table_ctx_t));
    ctx->d_max = d_maximum;
    ctx->t_max = t_maximum;
    ctx->n_max = n_maximum;
    ctx->tables_array = malloc(sizeof(cff_table_t*) * d_maximum);

    // create an array of booleans to determine if numbers are prime
    bool *prime_array = malloc(sizeof(bool)*t_maximum + 1);
    prime_sieve(t_maximum, prime_array);

    // each table is stored in this array of pointers to structs, where each struct is one table

    // best 1-CFFs are sperner systems, make these seperately
    ctx->tables_array[0] = makeSpernerTable();

    // 2-CFFs have more constructions, so handle it seperately
    int c = 0;
    if (d_maximum > 1)
    {
        ctx->tables_array[1] = initializeTable(t_maximum+1, 2, n_maximum);
        cff_table_add_fixed_cffs(ctx);
        cff_table_add_sts_cffs(ctx, t_maximum);
        cff_table_add_reed_solomon_cffs(ctx, 2, t_maximum, prime_array);
        cff_table_add_porat_rothschild_cffs(ctx, 2, t_maximum, prime_array);
        ctx->tables_array[1]->hasBeenChanged = true;
        while(ctx->tables_array[1]->hasBeenChanged)
        {
            c++;
            ctx->tables_array[1]->hasBeenChanged = false;
            cff_table_add_doubling_cffs(ctx);
            cff_table_add_ext_by_one_cffs(ctx, 2);
            cff_table_add_pair_constructed_cffs(ctx, 2);
        }
        ctx->tables_array[1]->num_loops_when_creating = c;
    }

    //tables for d=3 ... d_max
    for (int cff_d = 3; cff_d < d_maximum+1; cff_d++)
    {
        ctx->tables_array[cff_d-1] = initializeTable(t_maximum+1, cff_d, n_maximum);
        cff_table_add_reed_solomon_cffs(ctx, cff_d, t_maximum, prime_array);
        cff_table_add_porat_rothschild_cffs(ctx, cff_d, t_maximum, prime_array);
        ctx->tables_array[cff_d-1]->hasBeenChanged = true;
        c = 0;
        while(ctx->tables_array[cff_d-1]->hasBeenChanged)
        {
            c++;
            ctx->tables_array[cff_d-1]->hasBeenChanged = false;
            cff_table_add_ext_by_one_cffs(ctx, cff_d);
            cff_table_add_pair_constructed_cffs(ctx, cff_d);
        }
        ctx->tables_array[cff_d-1]->num_loops_when_creating = c;
    }
    free(prime_array);
    return ctx;
}

void cff_table_free(cff_table_ctx_t *ctx)
{
    for (int d = 0; d < ctx->d_max; d++)
    {
        for (int i = 0; i < ctx->tables_array[d]->numCFFs; i++)
        {
            cff_free(ctx->tables_array[d]->array[i].cff);
        }
        free(ctx->tables_array[d]->array);
        free(ctx->tables_array[d]);
    }
    free(ctx->tables_array);
    free(ctx);
}

void cff_table_short_name(cff_table_ctx_t *ctx, int d, int t, char *str_buffer)
{
    switch (ctx->tables_array[d]->array[t].constructionID)
    {
    case CFF_CONSTRUCTION_ID_IDENTITY_MATRIX:
        strcpy(str_buffer, "ID");
        break;
    case CFF_CONSTRUCTION_ID_SPERNER:
        strcpy(str_buffer, "Sperner");
        break;
    case CFF_CONSTRUCTION_ID_STS:
        strcpy(str_buffer, "STS");
        break;
    case CFF_CONSTRUCTION_ID_PORAT_ROTHSCHILD:
        strcpy(str_buffer, "Porat and Rothschild");
        break;
    case CFF_CONSTRUCTION_ID_REED_SOLOMON:
        strcpy(str_buffer, "Reed-Solomon");
        break;
    case CFF_CONSTRUCTION_ID_SHORT_REED_SOLOMON:
        strcpy(str_buffer, "Shortened Reed-Solomon");
        break;
    case CFF_CONSTRUCTION_ID_FIXED_CFF:
        strcpy(str_buffer, "Constant-weight binary code");
        break;
    case CFF_CONSTRUCTION_ID_EXT_BY_ONE:
        strcpy(str_buffer, "Extension by one");
        break;
    case CFF_CONSTRUCTION_ID_ADDITIVE:
        strcpy(str_buffer, "Additive");
        break;
    case CFF_CONSTRUCTION_ID_DOUBLING:
        strcpy(str_buffer, "Doubling");
        break;
    case CFF_CONSTRUCTION_ID_KRONECKER:
        strcpy(str_buffer, "Kronecker");
        break;
    case CFF_CONSTRUCTION_ID_OPTIMIZED_KRONECKER:
        strcpy(str_buffer, "Optimized Kronecker");
        break;
    default:
        strcpy(str_buffer, "Unknown construction!!!");
        break;
    }
}

void cff_table_long_name(cff_table_ctx_t *ctx, int d, int t, char *str_buffer)
{
    short *consParams = ctx->tables_array[d]->array[t].consParams;
    switch (ctx->tables_array[d]->array[t].constructionID)
    {
    case CFF_CONSTRUCTION_ID_IDENTITY_MATRIX:
        sprintf(str_buffer, "ID(%hd)", consParams[0]);
        break;
    case CFF_CONSTRUCTION_ID_SPERNER:
        sprintf(str_buffer, "Sp(%hd)", consParams[0]);
        break;
    case CFF_CONSTRUCTION_ID_STS:
        sprintf(str_buffer, "STS(%hd)", consParams[0]);
        break;
    case CFF_CONSTRUCTION_ID_PORAT_ROTHSCHILD:
        sprintf(str_buffer, "PR(%hd;%hd;%hd;%hd)", consParams[0], consParams[1], consParams[2], consParams[3]);
        break;
    case CFF_CONSTRUCTION_ID_REED_SOLOMON:
        sprintf(str_buffer, "RS(%hd^%hd;%hd;%hd)", consParams[0], consParams[1], consParams[2], consParams[3]);
        break;
    case CFF_CONSTRUCTION_ID_SHORT_REED_SOLOMON:
        sprintf(str_buffer, "SRS(%hd;%hd;%hd;%hd;%hd)", consParams[0], consParams[1], consParams[2], consParams[3], consParams[4]);
        break;
    case CFF_CONSTRUCTION_ID_FIXED_CFF:
        sprintf(str_buffer, "Survey CFF %hd", consParams[0]);
        break;
    case CFF_CONSTRUCTION_ID_EXT_BY_ONE:
        sprintf(str_buffer, "Extension by one of %hd", consParams[0]);
        break;
    case CFF_CONSTRUCTION_ID_ADDITIVE:
        sprintf(str_buffer, "Add(%hd;%hd)", consParams[0], consParams[1]);
        break;
    case CFF_CONSTRUCTION_ID_DOUBLING:
        sprintf(str_buffer, "Dbl(%hd;%hd)", consParams[0], consParams[1]);
        break;
    case CFF_CONSTRUCTION_ID_KRONECKER:
        sprintf(str_buffer, "Kr(%hd;%hd)", consParams[0], consParams[1]);
        break;
    case CFF_CONSTRUCTION_ID_OPTIMIZED_KRONECKER:
        sprintf(str_buffer, "OKr(%hd;%hd;%hd)", consParams[0], consParams[1], consParams[2]);
        break;
    default:
        strcpy(str_buffer, "Unknown construction!!!");
        break;
    }
}

// writes the tables to csvs in folder_path
// each table is called d_1.csv, d_2.csv, etc
// the function will replace files if they were there before
void cff_table_write_csv(cff_table_ctx_t *ctx, const char *folder_path)
{
    for (int i = 0; i < ctx->d_max; i++)
    {
        char filename[500];
        char shortSrc[100];
        char longSrc[100];
        FILE *fptr = NULL;
        sprintf(filename, "%s/d_%d.csv", folder_path, ctx->tables_array[i]->d);
        fptr = fopen(filename, "w");
        fprintf(fptr, "t,n,short source,long source");
        for (int t = 0; t < ctx->tables_array[i]->numCFFs; t++)
        {
            cff_table_short_name(ctx, i, t, shortSrc);
            cff_table_long_name(ctx, i, t, longSrc);
            fprintf(
                fptr,
                "\n%d, %lld, %s, %s",
                t,
                ctx->tables_array[i]->array[t].n,
                shortSrc,
                longSrc
            );
        }
        fclose(fptr);
    }
}

int cff_table_get_num_loops_done_to_create(cff_table_ctx_t *ctx, int d)
{
    return ctx->tables_array[d-1]->num_loops_when_creating;
}