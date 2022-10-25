#include <stdio.h>

int main()
{
        

        int a = 110;
        int *b = &a;

        *b = *b - 10;

        printf("a = %d\n", a);
        printf("sizeof int: %lu\n", sizeof(int));
        printf("sizeof *int: %lu\n", sizeof(b));

        return 0;
}