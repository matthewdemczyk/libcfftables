#include <stdio.h>
#include <assert.h>
#include <libcfftables/libcfftables.h>

// try the kronecker product of 2 STSs
void test_kronecker_1()
{
    puts("Running test_kronecker_1...");
    cff_t *cff_left = cff_sts(9);
    cff_t *cff_right = cff_sts(13);
    cff_t *cff = cff_kronecker(cff_left, cff_right);
    cff_free(cff_left);
    cff_free(cff_right);
    assert(cff_verify(cff));
    assert(cff_get_d(cff) == 2);
    assert(cff_get_t(cff) == 117);
    assert(cff_get_n(cff) == 312);
    cff_print(cff);
    cff_free(cff);
    puts("OK test_kronecker_1 passed");
}

// make sure the kronecker product of 1 or 2 NULLs returns NULL
void test_kronecker_2()
{
    puts("Running test_kronecker_2...");
    cff_t *test_cff = cff_sts(9);
    assert(cff_kronecker(NULL, NULL) == NULL);
    assert(cff_kronecker(NULL, test_cff) == NULL);
    assert(cff_kronecker(test_cff, NULL) == NULL);
    cff_free(test_cff);
    puts("OK test_kronecker_2 passed");
}

int main()
{
    test_kronecker_1();
    test_kronecker_2();

    puts("ALL test_additive passed");
}