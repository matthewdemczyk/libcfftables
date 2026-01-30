#include <stdio.h>
#include <assert.h>
#include <libcfftables/libcfftables.h>


void test_cff_reed_solomon_1()
{
    puts("Running test_cff_reed_solomon_1...");
    cff_t* cff = cff_reed_solomon(5,1,2,4);
    assert(cff_verify(cff));
    assert(cff_get_d(cff) == 3);
    assert(cff_get_t(cff) == 20);
    assert(cff_get_n(cff) == 25);
    cff_print(cff);
    cff_free(cff);
    puts("OK test_cff_reed_solomon_1 passed");
}

void test_cff_reed_solomon_2()
{
    puts("Running test_cff_reed_solomon_2...");
    cff_t* cff = cff_reed_solomon(2,3,2,5);
    assert(cff_verify(cff));
    assert(cff_get_d(cff) == 4);
    assert(cff_get_t(cff) == 40);
    assert(cff_get_n(cff) == 64);
    cff_print(cff);
    cff_free(cff);
    puts("OK test_cff_reed_solomon_2 passed");
}

int main()
{
    test_cff_reed_solomon_1();
    test_cff_reed_solomon_2();

    puts("ALL test_reed_solomon passed");
    return 0;
}