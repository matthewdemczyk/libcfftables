#include <stdio.h>
#include <assert.h>
#include <libcfftables/libcfftables.h>

void test_fixed_constructions()
{
    for (int i = 12; i < 24; i++)
    {
        cff_t *cff = get_fixed_cff(2, i);
        cff_print(cff);
        assert(cff_verify(cff));
        cff_free(cff);
    }
}

void test_fixed_construction_not_found()
{
    cff_t *cff = get_fixed_cff(1,1); //ask for a cff that isnt a fixed cff
    assert(cff == NULL);

    cff = get_fixed_cff(2,1); //ask for a cff that isnt a fixed cff
    assert(cff == NULL);
}

int main()
{
    test_fixed_constructions();
    test_fixed_construction_not_found();
    return 0;
}