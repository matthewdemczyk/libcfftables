#include <stdio.h>
#include <assert.h>
#include <libcfftables/libcfftables.h>

void test_optimized_kronecker_1()
{
    puts("Running test_optimized_kronecker_1...");
    cff_t *cff_outer = cff_sperner(7);
    cff_t *cff_inner = cff_sts(9);
    cff_t *cff_bottom = cff_sts(7);
    cff_t *cff = cff_optimized_kronecker(cff_outer, cff_inner, cff_bottom);
    cff_free(cff_outer);
    cff_free(cff_inner);
    cff_free(cff_bottom);
    assert(cff_verify(cff));
    cff_print(cff);
    cff_free(cff);
    puts("OK test_optimized_kronecker_1 passed");
}

void test_optimized_kronecker_2()
{
    puts("Running test_optimized_kronecker_2...");
    cff_t *test_cff = cff_sts(9);
    assert(cff_optimized_kronecker(NULL, NULL, NULL) == NULL);

    assert(cff_optimized_kronecker(test_cff, NULL, NULL) == NULL);
    assert(cff_optimized_kronecker(NULL, test_cff, NULL) == NULL);
    assert(cff_optimized_kronecker(NULL, NULL, test_cff) == NULL);

    assert(cff_optimized_kronecker(test_cff, test_cff, NULL) == NULL);
    assert(cff_optimized_kronecker(NULL, test_cff, test_cff) == NULL);
    assert(cff_optimized_kronecker(test_cff, NULL, test_cff) == NULL);

    assert(cff_optimized_kronecker(test_cff, test_cff, test_cff) == NULL);

    cff_free(test_cff);
    puts("OK test_optimized_kronecker_2 passed");
}

int main()
{
    test_optimized_kronecker_1();
    test_optimized_kronecker_2();

    puts("ALL test_optimized_kronecker passed");
}