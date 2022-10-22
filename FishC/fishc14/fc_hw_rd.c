#include <stdio.h>

int main()
{
    int i,j,k;
    for(i = 1; i <= 9; i++)
    {
        for(j = 1; j <= 9-i; j++)
        {
            printf("       ");
        }

        for(k = 1; k <= i; k++)
        {
            printf("%d*%d=%-3d", i, k, i * k);
        }

        putchar('\n');
    }
    return 0;
}