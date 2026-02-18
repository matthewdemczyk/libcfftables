#include "../../include/libcfftables/libcfftables.h"


cff_t* cff_identity(int d, int n)
{
    if (d >= n) return NULL;
    cff_t *result = cff_alloc(d, n, n);
    if (result == NULL) return NULL;
    for (int i = 0; i < n; i++)
    {
        cff_set_matrix_value(result, i, i, 1);
    }
    return result;
}