#ifndef LIBCFF_INTERNAL_CONSTRUCTIONS_HEADER
#define LIBCFF_INTERNAL_CONSTRUCTIONS_HEADER

#include "../../include/libcfftables/libcfftables.h"
#include "../CFF_Internals.h"

double poratEntropyFunction(double q, double r);

// put construction and name function structs here only if they're needed in other files
extern CFF_Construction_And_Name_Functions addConstructionFunctions;
extern CFF_Construction_And_Name_Functions kroneckerConstructionFunctions;
extern CFF_Construction_And_Name_Functions shortRsConstructionFunctions;
extern CFF_Construction_And_Name_Functions idConstructionFunctions;
extern CFF_Construction_And_Name_Functions spernerConstructionFunctions;

// these are defined in various CFF_Constructions/*.c files and are needed in src/CFF_Tables.c
void doublingConstructionFiller(CFF_Table *table_2, CFF_Table *table_1);
void addSurveyCFFs(CFF_Table* table);
void applyPairConstructions(CFF_Table *table, CFF_Table *d_minus_one_table, int cff_d);
void addPoratCodes(CFF_Table* table, int cff_d, bool* prime_power_array);
void addReedSolomonCodes(CFF_Table *table, int cff_d, bool prime_array[]);
void addSTS(CFF_Table *table);
void extendByOneConstructionFiller(CFF_Table *table, int cff_d);

#endif