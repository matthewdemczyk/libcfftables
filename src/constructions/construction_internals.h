#ifndef LIBCFF_INTERNAL_CONSTRUCTIONS_HEADER
#define LIBCFF_INTERNAL_CONSTRUCTIONS_HEADER

#include "../../include/libcfftables/libcfftables.h"
#include "../cff_internals.h"

// these are stored in a row of the table to know which
// construction a row in the table corresponds to

#define CFF_CONSTRUCTION_ID_IDENTITY_MATRIX 0
#define CFF_CONSTRUCTION_ID_SPERNER 1
#define CFF_CONSTRUCTION_ID_STS 2
#define CFF_CONSTRUCTION_ID_PORAT_ROTHSCHILD 3
#define CFF_CONSTRUCTION_ID_REED_SOLOMON 4
#define CFF_CONSTRUCTION_ID_SHORT_REED_SOLOMON 5
#define CFF_CONSTRUCTION_ID_FIXED_CFF 6
#define CFF_CONSTRUCTION_ID_EXT_BY_ONE 7
#define CFF_CONSTRUCTION_ID_ADDITIVE 8
#define CFF_CONSTRUCTION_ID_DOUBLING 9
#define CFF_CONSTRUCTION_ID_KRONECKER 10
#define CFF_CONSTRUCTION_ID_OPTIMIZED_KRONECKER 11

// these are defined in various constructions/*.c files and are needed in src/CFF_Tables.c
void doublingConstructionFiller(CFF_Table *table_2, CFF_Table *table_1);
void cff_table_add_fixed_cffs(cff_table_ctx_t *ctx);
// applyPairConstructions adds the kronecker product, additive, and optimized kronecker product to the tables
void applyPairConstructions(CFF_Table *table, CFF_Table *d_minus_one_table, int cff_d);
void addPoratCodes(CFF_Table* table, int cff_d, int t_max, bool* prime_array);
void addReedSolomonCodes(CFF_Table *table, int cff_d, int t_max, bool *prime_array);
void addSTS(CFF_Table *table, int t_max);
void extendByOneConstructionFiller(CFF_Table *table, int cff_d);

#endif