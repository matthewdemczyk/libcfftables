#include "construction_internals.h"
#include "../../include/libcfftables/libcfftables.h"

#include <stdbool.h>
#include <string.h>

void idShortSrcFormatter(char *strBuffer)
{
    strcpy(strBuffer, "ID");
}

void idLongSrcFormatter(short *consParams, char *strBuffer)
{
    sprintf(strBuffer, "ID(%hd)", consParams[0]);
}

void idConstructCFF(int d, int t)
{
    global_tables_array[d-1]->array[t].cff = cff_identity(t, d);
}

CFF_Construction_And_Name_Functions idConstructionFunctions = {
    .shortSrcFormatter = idShortSrcFormatter,
    .longSrcFormatter = idLongSrcFormatter,
    .constructionFunction = idConstructCFF
};

cff_t* cff_identity(int n, int d)
{
    cff_t *result = cff_alloc(d, n, n);
    for (int i = 0; i < n; i++)
    {
        cff_set_value(result, i, i, 1);
    }
    return result;
}