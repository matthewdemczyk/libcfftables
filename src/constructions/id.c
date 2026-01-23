#include "../../include/libcfftables/libcfftables.h"


cff_t* cff_identity(int n, int d)
{
    cff_t *result = cff_alloc(d, n, n);
    for (int i = 0; i < n; i++)
    {
        cff_set_value(result, i, i, 1);
    }
    return result;
}