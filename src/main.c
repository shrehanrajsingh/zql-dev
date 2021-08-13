#include <zql_imports.h>

int main(int argc, char const *argv[])
{
    ZQL_InitializeHeap();
    ZQL_RegisterDatabase("tests/test.db");

    zql_t *get_name = ZQL_Get("name");
    ZQL_PrintQueryKey(get_name[0]);

    zql_t *get_everything = ZQL_ReadAll();
    ZQL_PrintQuerySet(get_everything);

    free(get_name);
    free(get_everything);
    
    ZQL_DestroyHeap();
    return 0;
}