#include <stdio.h>
#include <assert.h>
#include <libcfftables/libcfftables.h>

/*
    test ensures that allocating memory works, and
    that cff_t has struct attributes set properly
*/
 void test_cff_alloc()
 {
    printf("Testing cff_alloc...\n");
    cff_t *cff = cff_alloc(2, 10, 15);
    cff_print(cff);
    assert(cff_get_d(cff) == 2);
    assert(cff_get_t(cff) == 10);
    assert(cff_get_n(cff) == 15);

    cff_set_d(cff, 3);
    assert(cff_get_d(cff) == 3);
    cff_free(cff);
    printf("✓ cff_alloc test passed\n");
}

/*
    ensure that cff_free does not try to
    access null pointers in a cff_t struct
*/
void test_cff_null_free()
{
    printf("Testing cff_null_free...\n");
    cff_t *cff = NULL;
    cff_free(cff);
    printf("✓ cff_null_free test passed\n");
}


/*
    test that cff_set_value and cff_get_value work
*/
void test_cff_set_get()
{
    printf("Testing cff_set_get...\n");
    cff_t *cff = cff_alloc(2, 10, 10);
    cff_set_value(cff, 0, 0, 1);
    cff_print(cff);
    assert(cff_get_value(cff, 0, 0) == 1);
    //check other values are still zero
    assert(cff_get_value(cff, 1, 1) == 0);
    cff_free(cff);
    printf("✓ cff_set_get test passed\n");
}

void test_cff_verify_1()
{
    printf("Testing cff_verify_1...\n");
    cff_t *cff = cff_alloc(2, 10, 10);
    cff_print(cff);
    assert(cff_verify(cff) == false);
    cff_free(cff);
    printf("✓ cff_verify_1 test passed\n");
}

void test_cff_verify_2()
{
    printf("Testing cff_verify_2...\n");
    cff_t *cff = cff_alloc(1, 4, 6);
    //set cff to sperner system for t=4
    cff_set_value(cff, 0, 0, 1);
    cff_set_value(cff, 0, 1, 1);
    cff_set_value(cff, 0, 2, 1);

    cff_set_value(cff, 1, 0, 1);
    cff_set_value(cff, 1, 3, 1);
    cff_set_value(cff, 1, 4, 1);

    cff_set_value(cff, 2, 1, 1);
    cff_set_value(cff, 2, 3, 1);
    cff_set_value(cff, 2, 5, 1);

    cff_set_value(cff, 3, 2, 1);
    cff_set_value(cff, 3, 4, 1);
    cff_set_value(cff, 3, 5, 1);
    cff_print(cff);
    assert(cff_verify(cff));
    cff_free(cff);
    printf("✓ cff_verify_2 test passed\n");
}

void test_cff_verify_3()
{
    printf("Testing cff_verify_3...\n");
    cff_t *cff = cff_alloc(2, 10, 10);
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            cff_set_value(cff, i, j, 1);
        }
    }
    cff_print(cff);
    assert(cff_verify(cff) == false);
    cff_free(cff);
    printf("✓ cff_verify_3 test passed\n");
}

// Test the invalid parameter branch in cff_verify
void test_cff_verify_4()
{
    printf("Testing cff_verify_4...\n");
    cff_t *cff = cff_alloc(10, 5, 8); // d+1=11 > n=8
    assert(cff_verify(cff) == false);
    cff_free(cff);
    printf("✓ cff_verify_invalid_params test passed\n");
}

// Test boundary: d+1 exactly equals n
void test_cff_verify_5()
{
    printf("Testing cff_verify boundary case...\n");
    cff_t *cff = cff_alloc(2, 4, 3); // d+1 = n
    // Create a minimal valid 2-CFF(4,3)
    for (int i = 0; i < 3; i++)
    {
        cff_set_value(cff, i, i, 1);
    }
    assert(cff_verify(cff) == true);
    cff_free(cff);
    printf("✓ cff_verify_boundary test passed\n");
}

void test_cff_from_matrix() {
    printf("Testing cff_from_matrix...\n");
    // this is a 3-cff(20,25) from a reed solomon code with
    // alphabet len=9, message len=2, codeword len=4
    int matrix[20][25] = {
        {1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0},
        {0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0},
        {0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0},
        {0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0},
        {1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0},
        {0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0},
        {0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
        {0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0}
    };
    cff_t *cff = cff_from_matrix(3, 20, 25, (int*) matrix);
    cff_print(cff);
    assert(cff_get_d(cff) == 3);
    assert(cff_get_t(cff) == 20);
    assert(cff_get_n(cff) == 25);
    assert(cff_verify(cff));
    cff_free(cff);
    printf("✓ cff_from_matrix test passed\n");
}

void test_cff_copy() {
    printf("Testing cff_copy...\n");
    cff_t *cff = cff_alloc(2, 6, 6);
    for (int i = 0; i < 6; i++)
    {
        cff_set_value(cff, i, i, 1);
    }
    cff_t *copy_of_cff = cff_copy(cff);
    assert(cff_verify(copy_of_cff));
    cff_free(cff);
    cff_free(copy_of_cff);
    printf("✓ cff_copy test passed\n");
}

// Test cff_write
void test_cff_write() {
    printf("Testing cff_write...\n");
    cff_t *cff = cff_alloc(1, 2, 3);
    cff_set_value(cff, 0, 0, 1);
    cff_set_value(cff, 1, 2, 1);

    FILE *f = fopen("cff_test_output.txt", "w");
    cff_write(cff, f);
    fclose(f);

    // Read back and verify
    f = fopen("cff_test_output.txt", "r");
    char buffer[10];
    fgets(buffer, 10, f);
    assert(buffer[0] == '1' && buffer[1] == '0' && buffer[2] == '0');
    fclose(f);
    remove("cff_test_output.txt");

    cff_free(cff);
    printf("✓ cff_write test passed\n");
}

int main() {
    test_cff_alloc();
    test_cff_null_free();
    test_cff_set_get();

    test_cff_verify_1();
    test_cff_verify_2();
    test_cff_verify_3();
    test_cff_verify_4();
    test_cff_verify_5();

    test_cff_from_matrix();
    test_cff_copy();
    test_cff_write();

    printf("\n✓ test_cff tests passed!\n");
    return 0;
}