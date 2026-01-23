#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <libcfftables/libcfftables.h>

void test_cff_identity_1()
{
    cff_t* cff = cff_identity(15,1);
    assert(cff_verify(cff));
    cff_print(cff);
    cff_free(cff);
    printf("✓ test_cff_identity_1 construction test passed\n");
}

void test_cff_identity_2()
{
    cff_t* cff = cff_identity(10,2);
    assert(cff_verify(cff));
    cff_print(cff);
    cff_free(cff);
    printf("✓ test_cff_identity_2 construction test passed\n");
}


void test_cff_reed_solomon_1() {
    printf("Testing prime^1 reed solomon CFF construction...\n");

    // Example test
    cff_t* cff = cff_reed_solomon(5,1,2,4);
    assert(cff_verify(cff));
    cff_print(cff);
    cff_free(cff);
    printf("✓ test_cff_reed_solomon_1 construction test passed\n");
}

void test_cff_reed_solomon_2() {
    printf("Testing prime power reed solomon CFF construction...\n");

    // Example test
    cff_t* cff = cff_reed_solomon(3,2,2,4);
    assert(cff_verify(cff));
    cff_print(cff);
    cff_free(cff);
    printf("✓ test_cff_reed_solomon_2 construction test passed\n");
}

void test_cff_table()
{
    cff_table_ctx_t *ctx = cff_table_create(10,200,20000, true, true);
    cff_table_write_csv(ctx, "/mnt/e/cfftablescython/tables");
    cff_table_free(ctx);
}

int main()
{
    test_cff_identity_1();
    test_cff_reed_solomon_1();
    test_cff_reed_solomon_2();
    test_cff_table();
    printf("\n✓ test_cff_direct_constructions tests passed!\n");
    return 0;
}