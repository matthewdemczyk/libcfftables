#include <stdio.h>
#include <assert.h>
#include <libcfftables/libcfftables.h>

// test bose construction
void test_cff_sts_1()
{
    puts("Running test_cff_sts_1...");
    cff_t *cff = cff_sts(9);
    assert(cff_verify(cff));
    assert(cff_get_d(cff) == 2);
    assert(cff_get_t(cff) == 9);
    assert(cff_get_n(cff) == 12);
    cff_print(cff);
    cff_free(cff);
    puts("OK test_cff_sts_1 passed");
}

// test skolem construction
void test_cff_sts_2()
{
    puts("Running test_cff_sts_2...");
    cff_t *cff = cff_sts(13);
    assert(cff_verify(cff));
    assert(cff_get_d(cff) == 2);
    assert(cff_get_t(cff) == 13);
    assert(cff_get_n(cff) == 26);
    cff_print(cff);
    cff_free(cff);
    puts("OK test_cff_sts_2 passed");
}

// test with v not congruent to 1,3 mod 6
void test_cff_sts_3()
{
    puts("Running test_cff_sts_3...");
    cff_t *cff = cff_sts(14);
    assert(cff == NULL);
    puts("OK test_cff_sts_3 passed");
}


int main()
{
    test_cff_sts_1();
    test_cff_sts_2();
    test_cff_sts_3();
    puts("ALL test_sts passed");
}