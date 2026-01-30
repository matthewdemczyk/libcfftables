#include <stdio.h>
#include <assert.h>
#include <libcfftables/libcfftables.h>

// basic usage
void test_cff_sperner_1()
{
    puts("Running test_cff_sperner_1...");
    cff_t *cff = cff_sperner(6);
    assert(cff_verify(cff));
    assert(cff_get_d(cff) == 1);
    assert(cff_get_t(cff) == 4);
    assert(cff_get_n(cff) == 6);
    cff_print(cff);
    cff_free(cff);
    puts("OK test_cff_sperner_1 passed");
}

// case when the n is not an exact value on the tables
void test_cff_sperner_2()
{
    puts("Running test_cff_sperner_2...");
    cff_t *cff = cff_sperner(5);
    assert(cff_verify(cff));
    assert(cff_get_d(cff) == 1);
    assert(cff_get_t(cff) == 4);
    assert(cff_get_n(cff) == 5);
    cff_print(cff);
    cff_free(cff);
    puts("OK test_cff_sperner_2 passed");
}

// case for invalid parameters
void test_cff_sperner_3()
{
    puts("Running test_cff_sperner_3...");
    cff_t *cff = cff_sperner(0);
    assert(cff == NULL);
    puts("OK test_cff_sperner_3 passed");
}

int main()
{
    test_cff_sperner_1();
    test_cff_sperner_2();
    test_cff_sperner_3();

    puts("ALL test_sperner passed");
}