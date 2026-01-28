#include <stdio.h>
#include <assert.h>
#include <libcfftables/libcfftables.h>

void test_cff_table_get_by_t_1()
{
    cff_table_ctx_t *ctx = cff_table_create(3, 100, 2000);
    cff_t *cff = cff_table_get_by_t(ctx, 3, 20); //reed solomon
    cff_print(cff);
    assert(cff_verify(cff));
    cff_free(cff);
    cff_table_free(ctx);
    puts("passed test 1");
}

void test_cff_table_get_by_t_2()
{
    cff_table_ctx_t *ctx = cff_table_create(3, 100, 2000);
    cff_t *cff = cff_table_get_by_t(ctx, 2, 34); //doubling of a fixed cff
    cff_print(cff);
    assert(cff_verify(cff));
    cff_free(cff);
    cff_table_free(ctx);
    puts("passed test 1");
}

void test_cff_table()
{
    cff_table_ctx_t *ctx = cff_table_create(10,200,200000);
    cff_table_write_csv(ctx, "test_output");
    cff_table_free(ctx);
}

int main()
{
    test_cff_table_get_by_t_1();
    test_cff_table_get_by_t_2();

    test_cff_table();

    puts("all tests passed!\n");
}