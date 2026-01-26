#include "construction_internals.h"

#include "../../include/libcfftables/libcfftables.h"
#include "../CFF_Internals.h"

#include <stdbool.h>

cff_t* cff_extend_by_one(const cff_t* cff)
{
    return cff_additive(cff, cff_identity(cff->d,1));
}

void extendByOneConstructionFiller(CFF_Table *table, int cff_d)
{
    for (int t = cff_d+1; t < table->numCFFs; t++)
    {
        updateTable(table, t + 1, table->array[t].n + 1, CFF_CONSTRUCTION_ID_EXT_BY_ONE, t, 0, 0, 0, 0);
    }
}