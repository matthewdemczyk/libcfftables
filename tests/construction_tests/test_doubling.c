#include <stdio.h>
#include <assert.h>
#include <libcfftables/libcfftables.h>

// try doubling when s is even
void test_doubling_1()
{
    puts("Running test_doubling_1...");
    cff_t *cff_to_double = cff_sts(9);
    cff_t *cff = cff_doubling(cff_to_double, 6);
    cff_free(cff_to_double);
    assert(cff_verify(cff));
    assert(cff_get_d(cff) == 2);
    assert(cff_get_t(cff) == 17);
    assert(cff_get_n(cff) == 24);
    cff_print(cff);
    cff_free(cff);
    puts("OK test_doubling_1 passed");
}

// try doubling when s is odd
void test_doubling_2()
{
    puts("Running test_doubling_2...");
    cff_t *cff_to_double = cff_sts(13);
    cff_t *cff = cff_doubling(cff_to_double, 7);
    cff_free(cff_to_double);
    assert(cff_verify(cff));
    assert(cff_get_d(cff) == 2);
    assert(cff_get_t(cff) == 21);
    assert(cff_get_n(cff) == 52);
    cff_print(cff);
    cff_free(cff);
    puts("OK test_doubling_2 passed");
}

int main()
{
    test_doubling_1();
    test_doubling_2();

    puts("ALL test_additive passed");
}