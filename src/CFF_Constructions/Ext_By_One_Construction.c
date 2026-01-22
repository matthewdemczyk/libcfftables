#include "Construction_Function_Internals.h"

#include "../../include/libcfftables/libcfftables.h"
#include "../CFF_Internals.h"

#include <string.h>
#include <stdbool.h>


void extShortSrcFormatter(char *strBuffer)
{
    strcpy(strBuffer, "Extension by one");
}

void extLongSrcFormatter(short *consParams, char *strBuffer)
{
    sprintf(strBuffer, "Extension by one of %hd", consParams[0]);
}

void extConstructCFF(int d, int t)
{
    cff_t *toExtend = cff_table_get_by_t(d, global_tables_array[d-1]->array[t].consParams[0]);
    global_tables_array[d-1]->array[t].cff = cff_additive(toExtend, cff_table_get_by_t(d, 1));
}

cff_t* cff_extend_by_one(const cff_t* cff)
{
    return cff_additive(cff, cff_identity(cff->d,1));
}

CFF_Construction_And_Name_Functions extConstructionFunctions = {
    .shortSrcFormatter = extShortSrcFormatter,
    .longSrcFormatter = extLongSrcFormatter,
    .constructionFunction = extConstructCFF
};

void extendByOneConstructionFiller(CFF_Table *table, int cff_d)
{
    for (int t = cff_d+1; t < table->numCFFs; t++)
    {
        updateTable(table, t + 1, table->array[t].n + 1, &extConstructionFunctions, t, 0, 0, 0, 0);
    }
}