#include <stdio.h>
#include <assert.h>
#include <libcfftables/libcfftables.h>

// try extending sts 9
void test_ext_by_one_1()
{
    puts("Running test_ext_by_one_1...");
    cff_t *cff_to_extend = cff_sts(9);
    cff_t *cff = cff_extend_by_one(cff_to_extend);
    cff_free(cff_to_extend);
    assert(cff_verify(cff));
    assert(cff_get_d(cff) == 2);
    assert(cff_get_t(cff) == 10);
    assert(cff_get_n(cff) == 13);
    cff_print(cff);
    cff_free(cff);
    puts("OK test_ext_by_one_1 passed");
}

// try extending a fixed cff
void test_ext_by_one_2()
{
    puts("Running test_ext_by_one_2...");
    cff_t *cff_to_extend = cff_reed_solomon(5,1,2,4);
    cff_t *cff = cff_extend_by_one(cff_to_extend);
    cff_free(cff_to_extend);
    assert(cff_verify(cff));
    assert(cff_get_d(cff) == 3);
    assert(cff_get_t(cff) == 21);
    assert(cff_get_n(cff) == 26);
    cff_print(cff);
    cff_free(cff);
    puts("OK test_ext_by_one_2 passed");
}

// try extending NULL (should return NULL)
void test_ext_by_one_3()
{
    puts("Running test_ext_by_one_3...");
    cff_t *cff = cff_extend_by_one(NULL);
    assert(cff == NULL);
    puts("OK test_ext_by_one_3 passed");
}

int main()
{
    test_ext_by_one_1();
    test_ext_by_one_2();
    test_ext_by_one_3();

    puts("ALL test_ext_by_one passed");
}