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
#define CFF_CONSTRUCTION_ID_DOUBLING 8
#define CFF_CONSTRUCTION_ID_ADDITIVE 9
#define CFF_CONSTRUCTION_ID_KRONECKER 10
#define CFF_CONSTRUCTION_ID_OPTIMIZED_KRONECKER 11

// these are defined in various constructions/*.c files and are needed in src/cff_table_ts.c

void cff_table_add_sts_cffs(cff_table_ctx_t *ctx, int t_max);
void cff_table_add_porat_rothschild_cffs(cff_table_ctx_t *ctx, int cff_d, int t_max, bool *prime_array);
void cff_table_add_reed_solomon_cffs(cff_table_ctx_t *ctx, int cff_d, int t_max, bool *prime_array);
void cff_table_add_fixed_cffs(cff_table_ctx_t *ctx);
void cff_table_add_ext_by_one_cffs(cff_table_ctx_t *ctx, int cff_d);
void cff_table_add_doubling_cffs(cff_table_ctx_t *ctx); //only for d=2
void cff_table_add_pair_constructed_cffs(cff_table_ctx_t *ctx, int cff_d);

#endif