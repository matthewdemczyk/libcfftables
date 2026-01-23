#include "construction_internals.h"
#include "../../include/libcfftables/libcfftables.h"
#include "../CFF_Internals.h"

#include <stdbool.h>
#include <string.h>

void addSurveyCFFs(CFF_Table* table)
{
    table->array[10].n = 13;
    table->array[10].consParams[0] = 10;
    table->array[10].constructionID = CFF_CONSTRUCTION_ID_FIXED_CFF;

    table->array[11].n = 17;
    table->array[11].consParams[0] = 11;
    table->array[11].constructionID = CFF_CONSTRUCTION_ID_FIXED_CFF;


    table->array[12].n = 20;
    table->array[12].consParams[0] = 12;
    table->array[12].constructionID = CFF_CONSTRUCTION_ID_FIXED_CFF;

    table->array[13].n = 26;
    table->array[13].consParams[0] = 13;
    table->array[13].constructionID = CFF_CONSTRUCTION_ID_FIXED_CFF;

    table->array[14].n = 28;
    table->array[14].consParams[0] = 14;
    table->array[14].constructionID = CFF_CONSTRUCTION_ID_FIXED_CFF;

    table->array[15].n = 42;
    table->array[15].consParams[0] = 15;
    table->array[15].constructionID = CFF_CONSTRUCTION_ID_FIXED_CFF;

    table->array[16].n = 48;
    table->array[16].consParams[0] = 16;
    table->array[16].constructionID = CFF_CONSTRUCTION_ID_FIXED_CFF;

    table->array[17].n = 68;
    table->array[17].consParams[0] = 17;
    table->array[17].constructionID = CFF_CONSTRUCTION_ID_FIXED_CFF;

    table->array[18].n = 69;
    table->array[18].consParams[0] = 18;
    table->array[18].constructionID = CFF_CONSTRUCTION_ID_FIXED_CFF;

    table->array[19].n = 76;
    table->array[19].consParams[0] = 19;
    table->array[19].constructionID = CFF_CONSTRUCTION_ID_FIXED_CFF;

    table->array[20].n = 90;
    table->array[20].consParams[0] = 20;
    table->array[20].constructionID = CFF_CONSTRUCTION_ID_FIXED_CFF;

    table->array[21].n = 120;
    table->array[21].consParams[0] = 21;
    table->array[21].constructionID = CFF_CONSTRUCTION_ID_FIXED_CFF;

    table->array[22].n = 176;
    table->array[22].consParams[0] = 22;
    table->array[22].constructionID = CFF_CONSTRUCTION_ID_FIXED_CFF;

    table->array[23].n = 253;
    table->array[23].consParams[0] = 23;
    table->array[23].constructionID = CFF_CONSTRUCTION_ID_FIXED_CFF;
}
