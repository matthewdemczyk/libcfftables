#include "construction_internals.h"
#include "../../include/libcfftables/libcfftables.h"
#include "../cff_internals.h"


cff_t* cff_sperner(int n)
{
    // first, determine t = min{s : choose(s, s/2) >= n }
    int s = 0;
    while (((long long) choose(s, s / 2)) < n) // cast choose(s,s/2) to suppress compiler warnings
    {
        s++;
    }
    int t = s;

    // allocate memory for the CFF and set its matrix to all 0s
    cff_t *cff = cff_alloc(1, t, n);

    // create the buffer for the current t/2-subset of {1 ... t}
    // and populate it with the first (lexicographic) set
    int subset[t / 2];
    for (int i = 0; i < t / 2; i++)
    {
        subset[i] = i;
    }

    // iterate over all of the t/2-subsets and assign them to a column in the cff
    int col = 0;
    do
    {
        // iterate over the subset and assign its points to the current CFF row
        for (int i = 0; i < t / 2; i++)
        {
            if (subset[i] < t && col < n)
            {
                cff_set_value(cff, subset[i], col, 1);
            }
        }
        col++;
    } while (kSubsetLexSuccessor(t, t / 2, subset));
    return cff;
}