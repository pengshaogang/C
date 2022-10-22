#include <stdio.h>

int main()
{
    int i,j;
    for(i = 1; i <= 9; i++)
    {
        for(j = i;  j <= 9; j++)
        {
            printf("%d*%d=%-4d", i, j, i*j);
        }
        putchar('\n');
    }
    return 0;
}