#include <zql_imports.h>

int main(int argc, char const *argv[])
{
    ZQL_InitializeHeap();
    ZQL_RegisterDatabase("tests/test.db");


    ZQL_DestroyHeap();
    return 0;
}