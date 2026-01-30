#include <stdio.h>
#include <assert.h>
#include <libcfftables/libcfftables.h>

// verify all the fixed cffs are returned and valid CFFs
void test_fixed_constructions()
{
    puts("Running test_fixed_constructions...");
    for (int i = 10; i < 24; i++)
    {
        cff_t *cff = cff_fixed(2, i);
        cff_print(cff);
        assert(cff_verify(cff));
        cff_free(cff);
    }
    puts("OK test_fixed_constructions passed");
}

// test that trying to get a fixed cff that isnt hardcoded returns null
void test_fixed_construction_not_found()
{
    puts("Running test_fixed_constructions...");
    cff_t *cff = cff_fixed(1,1); //ask for a cff that isnt a fixed cff
    assert(cff == NULL);

    cff = cff_fixed(2,1); //ask for a cff that isnt a fixed cff
    assert(cff == NULL);
    puts("OK test_fixed_construction_not_found passed");
}

int main()
{
    test_fixed_constructions();
    test_fixed_construction_not_found();
    puts("ALL test_fixed_cffs passed");
}