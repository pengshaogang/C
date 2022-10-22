#include <stdio.h>

int main()
{
    int a[][4] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    printf("%lu\n", sizeof(a)/sizeof(int));
    
    
    return 0;
}