#include "construction_internals.h"
#include "../../include/libcfftables/libcfftables.h"

#include <stdbool.h>
#include <stdlib.h>


void cff_table_add_sts_cffs(cff_table_ctx_t *ctx, int t_max)
{
    cff_table_t *table = ctx->tables_array[1]; //get 2-cff table
    long long n;
    for (int v = 2; v <= t_max; v++)
    {
        if (v % 6 == 3 || v % 6 == 1)
        {
            n = ((v * (v - 1)) / 6);
            update_table(table, v, n, CFF_CONSTRUCTION_ID_STS, v, 0, 0, 0, 0);
        }
    }
}

typedef struct SteinerTripleSystem
{
    int order;
    int numBlocks;
    int blocks[][3];
} SteinerTripleSystem;

int symmetricIdempotentQuasiGroupFunction
(
    int n, // order of the quasi group
    int x, // left operand
    int y  // right operand
)
{
    return (((n + 1) / 2) * (x + y)) % n;
}

int halfIdempotentQuasiGroupFunction
(
    int n, // order of the quasi group
    int x, // left operand
    int y  // right operand
)
{
    int t = (x + y) % n;
    if (t % 2 == 0)
    {
        return t / 2;
    } else
    {
        return (t + n - 1) / 2;
    }
}

SteinerTripleSystem* boseConstruction
(
    int v,                     // order of the STS (must be 3 mod 6)
    int (*ball)(int, int, int) // quasi group function
)
{
    int b = (v * (v - 1)) / 6; // b is the number of blocks of the STS
    SteinerTripleSystem *sts = malloc(sizeof(SteinerTripleSystem) + sizeof(int[b][3]));
    sts->numBlocks = b;
    sts->order = v;

    int n = (v - 3) / 6;
    int Q = (2 * n) + 1; // Q is the order of the quasi group

    // populate quasi group
    int quasiGroup[Q][Q];
    for (int x = 0; x < Q; x++)
    {
        for (int y = 0; y < Q; y++)
        {
            quasiGroup[x][y] = ball(Q, x, y);
        }
    }

    // create the blocks
    int blockNum = 0; //index of the current block in the array in the SteinerTripleSystem struct
    for (int x = 0; x <= 2 * n; x++)
    {
        for (int y = 0; y <= 2 * n; y++)
        {
            if (x == y)
            {   // Type 1 triple case
                sts->blocks[blockNum][0] = x + (Q * 0) + 1;
                sts->blocks[blockNum][1] = x + (Q * 1) + 1;
                sts->blocks[blockNum][2] = x + (Q * 2) + 1;
                blockNum++;
            } else if (x < y)
            {   // Type 2 triple case
                for (int i = 0; i < 3; i++)
                {
                    sts->blocks[blockNum][0] = x + (Q * i) + 1;
                    sts->blocks[blockNum][1] = y + (Q * i) + 1;
                    sts->blocks[blockNum][2] = quasiGroup[x][y] + (Q * ((i + 1) % 3)) + 1;
                    blockNum++;
                }
            }
        }
    }
    return sts;
}

SteinerTripleSystem* skolemConstruction
(
    int v,                     // order of the STS (must be 1 mod 6)
    int (*ball)(int, int, int) // quasi group function
)
{
    int b = (v * (v - 1)) / 6; // b is the number of blocks of the STS
    SteinerTripleSystem *sts = malloc(sizeof(SteinerTripleSystem) + sizeof(int[b][3]));
    sts->numBlocks = b;
    sts->order = v;

    int n = (v - 1) / 6;
    int Q = 2 * n; // Q is the order of the quasi group

    // populate quasi group
    int quasiGroup[Q][Q];
    for (int x = 0; x < Q; x++)
    {
        for (int y = 0; y < Q; y++)
        {
            quasiGroup[x][y] = ball(Q, x, y);
        }
    }

    // create the blocks
    int blockNum = 0; // index of the current block in the array in the SteinerTripleSystem struct
    int inf = v;
    for (int x = 0; x <= n-1; x++)
    {
        // Type 1 triple case
        sts->blocks[blockNum][0] = x + (Q * 0) + 1;
        sts->blocks[blockNum][1] = x + (Q * 1) + 1;
        sts->blocks[blockNum][2] = x + (Q * 2) + 1;
        blockNum++;

        // Type 2 triple case
        for (int i = 0; i < 3; i++)
        {
            sts->blocks[blockNum][0] = inf;
            sts->blocks[blockNum][1] = n + x + (Q * i) + 1;
            sts->blocks[blockNum][2] = x + (Q * ((i + 1) % 3)) + 1;
            blockNum++;
        }
    }
    for (int x = 0; x <= (2 * n) - 1; x++)
    {
        for (int y = 0; y <= (2 * n) - 1; y++)
        {
            if (x < y)
            {   // Type 3 triple case
                for (int i = 0; i < 3; i++)
                {
                    sts->blocks[blockNum][0] = x + (Q * i) + 1;
                    sts->blocks[blockNum][1] = y + (Q * i) + 1;
                    sts->blocks[blockNum][2] = quasiGroup[x][y] + (Q * ((i + 1) % 3)) + 1;
                    blockNum++;
                }
            }
        }
    }
    return sts;
}

cff_t* cff_sts(int v)
{
    SteinerTripleSystem *sts;
    if (v % 6 == 1)
    {
        sts = skolemConstruction(v, halfIdempotentQuasiGroupFunction);
    } else if (v % 6 == 3)
    {
        sts = boseConstruction(v, symmetricIdempotentQuasiGroupFunction);
    } else
    {
        return NULL;
    }
    cff_t *cff = cff_alloc(2, sts->order, sts->numBlocks);
    if (cff == NULL) return NULL;
    for (int i = 0; i < sts->numBlocks; i++)
    {
        for (int n = 0; n < 3; n++)
        {
            cff_set_matrix_value(cff, sts->blocks[i][n] - 1, i, 1);
        }
    }
    free(sts);
    return cff;
}
