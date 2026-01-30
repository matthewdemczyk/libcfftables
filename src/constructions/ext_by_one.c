#include "construction_internals.h"

#include "../../include/libcfftables/libcfftables.h"
#include "../cff_internals.h"

#include <stdbool.h>

cff_t* cff_extend_by_one(const cff_t* cff)
{
    if (cff == NULL) return NULL;
    int matrix[1][1] = {{1}};
    cff_t *one_by_one_cff = cff_from_matrix(cff->d, 1, 1, (int *) matrix);
    cff_t *result_cff = cff_additive(cff, one_by_one_cff);
    cff_free(one_by_one_cff);
    return result_cff;
}

void cff_table_add_ext_by_one_cffs(cff_table_ctx_t *ctx, int cff_d)
{
    cff_table_t *table = ctx->tables_array[cff_d-1];
    for (int t = cff_d+1; t < table->numCFFs; t++)
    {
        update_table(table, t + 1, table->array[t].n + 1, CFF_CONSTRUCTION_ID_EXT_BY_ONE, t, 0, 0, 0, 0);
    }
}