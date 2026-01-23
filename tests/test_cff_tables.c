#include <stdio.h>
#include <assert.h>
#include <libcfftables/libcfftables.h>

void test_cff_table_get_by_t_1()
{
    cff_table_ctx_t *ctx = cff_table_create(3, 100, 2000, false, true);
    cff_t *cff = cff_table_get_by_t(ctx, 3, 20);
    cff_print(cff);
    assert(cff_verify(cff));
    cff_free(cff);
    cff_table_free(ctx);
    puts("passed test 1");
}

int main()
{
    test_cff_table_get_by_t_1();
    puts("all tests passed!\n");
}