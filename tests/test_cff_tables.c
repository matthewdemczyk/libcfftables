#include <stdio.h>
#include <assert.h>
#include <libcfftables/libcfftables.h>

// test a direct construction
void test_cff_table_get_by_t_1()
{
    puts("Running test_cff_table_get_by_t_1...");
    cff_table_ctx_t *ctx = cff_table_create(3, 100, 2000);
    cff_t *cff = cff_table_get_by_t(ctx, 3, 20); //reed solomon
    cff_print(cff);
    assert(cff_verify(cff));
    cff_free(cff);
    cff_table_free(ctx);
    puts("OK test_cff_table_get_by_t_1 passed");
}

// test a recursive construction
void test_cff_table_get_by_t_2()
{
    puts("Running test_cff_table_get_by_t_2...");
    cff_table_ctx_t *ctx = cff_table_create(3, 100, 2000);
    cff_t *cff = cff_table_get_by_t(ctx, 2, 34); //doubling of a fixed cff
    cff_print(cff);
    assert(cff_verify(cff));
    cff_free(cff);
    cff_table_free(ctx);
    puts("OK test_cff_table_get_by_t_2 passed");
}

// test edge case: cff is exactly d max and t max
void test_cff_table_get_by_t_3()
{
    puts("Running test_cff_table_get_by_t_3...");
    cff_table_ctx_t *ctx = cff_table_create(3, 20, 2000);
    //edge case: requested cff has exactly t max and d max
    cff_t *cff = cff_table_get_by_t(ctx, 3, 20);
    cff_print(cff);
    assert(cff_verify(cff));
    cff_free(cff);
    cff_table_free(ctx);
    puts("OK test_cff_table_get_by_t_3 passed");
}

// test asking for a cff over bounds
void test_cff_table_get_by_t_4()
{
    puts("Running test_cff_table_get_by_t_4...");
    cff_table_ctx_t *ctx = cff_table_create(3, 20, 2000);
    //case: requested cff is outside of table bounds
    cff_t *cff = cff_table_get_by_t(ctx, 4, 30);
    assert(cff == NULL);
    cff_table_free(ctx);
    puts("OK test_cff_table_get_by_t_4 passed");
}

// test a direct construction
void test_cff_table_get_by_n_1()
{
    puts("Running test_cff_table_get_by_n_1...");
    cff_table_ctx_t *ctx = cff_table_create(3, 100, 2000);
    cff_t *cff = cff_table_get_by_n(ctx, 2, 20); //fixed cff
    cff_print(cff);
    assert(cff_verify(cff));
    cff_free(cff);
    cff_table_free(ctx);
    puts("OK test_cff_table_get_by_n_1 passed");
}

// test a recursive construction
void test_cff_table_get_by_n_2()
{
    puts("Running test_cff_table_get_by_n_2...");
    cff_table_ctx_t *ctx = cff_table_create(3, 100, 2000);
    cff_t *cff = cff_table_get_by_n(ctx, 3, 26); //ext by one of rs code
    cff_print(cff);
    assert(cff_verify(cff));
    cff_free(cff);
    cff_table_free(ctx);
    puts("OK test_cff_table_get_by_n_2 passed");
}

// test edge case: cff is exactly d and n max
void test_cff_table_get_by_n_3()
{
    puts("Running test_cff_table_get_by_n_3...");
    cff_table_ctx_t *ctx = cff_table_create(3, 100, 25);
    cff_t *cff = cff_table_get_by_n(ctx, 3, 25);
    cff_print(cff);
    assert(cff_verify(cff));
    cff_free(cff);
    cff_table_free(ctx);
    puts("OK test_cff_table_get_by_n_3 passed");
}

// test asking for a cff over bounds
void test_cff_table_get_by_n_4()
{
    puts("Running test_cff_table_get_by_n_4...");
    cff_table_ctx_t *ctx = cff_table_create(3, 20, 20);
    //case: requested cff is outside of table bounds
    cff_t *cff = cff_table_get_by_n(ctx, 4, 30);
    assert(cff == NULL);
    cff_table_free(ctx);
    puts("OK test_cff_table_get_by_n_4 passed");
}

void test_cff_table_write()
{
    puts("Running test_cff_table...");
    cff_table_ctx_t *ctx = cff_table_create(10,200,200000);
    cff_table_write_csv(ctx, "test_output");
    cff_table_free(ctx);
    puts("OK test_cff_table passed");
}

int main()
{
    test_cff_table_get_by_t_1();
    test_cff_table_get_by_t_2();
    test_cff_table_get_by_t_3();
    test_cff_table_get_by_t_4();

    test_cff_table_get_by_n_1();
    test_cff_table_get_by_n_2();
    test_cff_table_get_by_n_3();
    test_cff_table_get_by_n_4();

    test_cff_table_write();

    puts("ALL test_cff_tables passed");
}