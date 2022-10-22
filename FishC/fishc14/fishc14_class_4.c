#include <stdio.h>

int main()
{

    for(int i=0, j=10; i < j; i++, j--)
    {
        printf("%d\n", i);
    }

    printf("%d, %d", i, j);
    return 0;
}