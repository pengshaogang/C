#include <stdio.h>

int main()
{
    int a[3][4] = {[0][0] = 1, [1][1] = 2, [2][2] = 3};

    for (size_t i = 0; i < 3; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            printf("%d ", a[i][j]);
        }
        printf("\n");
        
    }
    

    return 0;
}