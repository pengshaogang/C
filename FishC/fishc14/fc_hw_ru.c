#include <stdio.h>

int main()
{
    int i,j;
    for(i = 1; i <= 9; i++)
    {
        for(j = 1; j <= 9; j++)
        {
            if(i > j)
            {
                printf("       ");
            }
            else
            {
                printf("%d*%d=%-3d", i, j, i * j);
            }
        }
        putchar('\n');
    }
    return 0;
}