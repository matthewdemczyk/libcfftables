#include <stdio.h>
#include <assert.h>
#include <libcfftables/libcfftables.h>

// test id construction with d=1, n=15.
// note this is never used in practice, because
// an optimal 1-CFF is from Sperner systems
void test_cff_identity_1()
{
    puts("Running test_cff_identity_1...");
    cff_t* cff = cff_identity(1,15);
    assert(cff_verify(cff));
    cff_print(cff);
    cff_free(cff);
    puts("OK test_cff_identity_1 passed");
}

// test id construction with t=2, n=7. This appears in the tables
void test_cff_identity_2()
{
    puts("Running test_cff_identity_2...");
    cff_t* cff = cff_identity(2,7);
    assert(cff_verify(cff));
    cff_print(cff);
    cff_free(cff);
    puts("OK test_cff_identity_2 passed");
}

// ensure that if d == n id returns NULL (invalid parameters)
void test_cff_identity_3()
{
    puts("Running test_cff_identity_2...");
    cff_t* cff = cff_identity(10,10);
    assert(cff == NULL);
    puts("OK test_cff_identity_3 passed");
}

// ensure that if d > n, id returns NULL (invalid parameters)
void test_cff_identity_4()
{
    puts("Running test_cff_identity_2...");
    cff_t* cff = cff_identity(20,10);
    assert(cff == NULL);
    puts("OK test_cff_identity_4 passed");
}

int main()
{
    test_cff_identity_1();
    test_cff_identity_2();
    test_cff_identity_3();
    test_cff_identity_4();

    puts("ALL test_identity tests passed");
}