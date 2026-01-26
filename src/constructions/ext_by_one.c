#include "construction_internals.h"

#include "../../include/libcfftables/libcfftables.h"
#include "../cff_internals.h"

#include <stdbool.h>

cff_t* cff_extend_by_one(const cff_t* cff)
{
    return cff_additive(cff, cff_identity(cff->d,1));
}

void cff_table_add_ext_by_one_cffs(cff_table_ctx_t *ctx, int cff_d)
{
    cff_table_t *table = ctx->tables_array[cff_d-1];
    for (int t = cff_d+1; t < table->numCFFs; t++)
    {
        update_table(table, t + 1, table->array[t].n + 1, CFF_CONSTRUCTION_ID_EXT_BY_ONE, t, 0, 0, 0, 0);
    }
}