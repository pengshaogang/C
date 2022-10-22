#include <stdio.h>

int main()
{
        int i, j, k;

        for (i = 1; i <= 9; i++)
        {
                for (k = 1; k <= 9-i; k++)
                {
                        printf("        ");
                }

                for (j = 1; j <= i; j++)
                {
                        printf("%d*%d=%-2d  ", i, j, i * j);
                }
                putchar('\n');
        }

        return 0;
}