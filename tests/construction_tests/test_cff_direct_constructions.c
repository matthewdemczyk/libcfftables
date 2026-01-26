#include <stdio.h>
#include <assert.h>
#include <libcfftables/libcfftables.h>

void test_cff_identity_1()
{
    printf("Running test_cff_identity_1 construction...\n");
    cff_t* cff = cff_identity(15,1);
    assert(cff_verify(cff));
    cff_print(cff);
    cff_free(cff);
    printf("✓ test_cff_identity_1 construction test passed\n");
}

void test_cff_identity_2()
{
    printf("Running test_cff_identity_2 construction...\n");
    cff_t* cff = cff_identity(10,2);
    assert(cff_verify(cff));
    cff_print(cff);
    cff_free(cff);
    printf("✓ test_cff_identity_2 construction test passed\n");
}


void test_cff_reed_solomon_1() {
    printf("Running test_cff_reed_solomon_1 construction...\n");

    // Example test
    cff_t* cff = cff_reed_solomon(5,1,2,4);
    assert(cff_verify(cff));
    cff_print(cff);
    cff_free(cff);
    printf("✓ test_cff_reed_solomon_1 construction test passed\n");
}

void test_cff_reed_solomon_2() {
    printf("Running test_cff_reed_solomon_2 test...\n");

    // Example test
    cff_t* cff = cff_reed_solomon(3,2,2,4);
    assert(cff_verify(cff));
    cff_print(cff);
    cff_free(cff);
    printf("✓ test_cff_reed_solomon_2 test passed\n");
}



int main()
{
    test_cff_identity_1();
    test_cff_identity_2();
    test_cff_reed_solomon_1();
    test_cff_reed_solomon_2();

    printf("\n✓ test_cff_direct_constructions tests passed!\n");
    return 0;
}