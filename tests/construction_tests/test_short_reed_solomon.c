#include <stdio.h>
#include <assert.h>
#include <libcfftables/libcfftables.h>

// try shortening a RS code by 1
void test_cff_short_reed_solomon_1()
{
    puts("Running test_cff_short_reed_solomon_1...");
    cff_t* cff = cff_short_reed_solomon(5,1,2,4,1);
    assert(cff_verify(cff));
    assert(cff_get_d(cff) == 1);
    assert(cff_get_t(cff) == 15);
    assert(cff_get_n(cff) == 5);
    cff_print(cff);
    cff_free(cff);
    puts("OK test_cff_short_reed_solomon_1 passed");
}

// try shortening a RS code by 2
void test_cff_short_reed_solomon_2()
{
    puts("Running test_cff_short_reed_solomon_2...");
    cff_t* cff = cff_short_reed_solomon(11,1,3,7,2);
    assert(cff_verify(cff));
    assert(cff_get_d(cff) == 1);
    assert(cff_get_t(cff) == 55);
    assert(cff_get_n(cff) == 11);
    cff_print(cff);
    cff_free(cff);
    puts("OK test_cff_short_reed_solomon_2 passed");
}

// note: these are testing bad CFFs that have n < t, these wouldnt appear in the tables
// (short RS never appears in the tables)
int main()
{
    test_cff_short_reed_solomon_1();
    test_cff_short_reed_solomon_2();

    puts("ALL test_short_reed_solomon passed");
    return 0;
}