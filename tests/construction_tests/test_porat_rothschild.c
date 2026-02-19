#include <stdio.h>
#include <assert.h>
#include <libcfftables/libcfftables.h>


void test_porat_1()
{
    puts("Running test_porat_1...");
    cff_t* cff = cff_porat_rothschild(3, 1, 2, 3, 3);
    assert(cff_verify(cff));
    assert(cff_get_d(cff) == 2);
    assert(cff_get_t(cff) == 9);
    assert(cff_get_n(cff) == 9);
    cff_print(cff);
    cff_free(cff);
    puts("OK test_porat_1 passed");
}

void test_porat_2()
{
    puts("Running test_porat_2...");
    cff_t* cff = cff_porat_rothschild(3, 1, 2, 3, 0);
    assert(cff == NULL);
    puts("OK test_porat_2 passed");
}

void test_porat_3()
{
    puts("Running test_porat_3...");
    cff_t* cff = cff_porat_rothschild(3, 1, 2, 2, 0);
    assert(cff_verify(cff));
    cff_print(cff);
    cff_free(cff);
    puts("OK test_porat_3 passed");
}

int main()
{
    test_porat_1();
    test_porat_2();
    test_porat_3();

    puts("ALL test_porat_rothschild passed");
    return 0;
}