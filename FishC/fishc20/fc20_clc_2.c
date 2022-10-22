#include <stdio.h>

int main()
{
    int a[4][5] = {
        {80, 92, 85, 86, 99},
        {78, 65, 89, 70, 99},
        {67, 78, 76, 89, 99},
        {88, 68, 98, 90, 99}
    };

    for (size_t i = 0; i < 5; i++)
    {
        for (size_t j = 0; j < 4; j++)
        {
            printf("%d ", a[j][i]);
        }
        printf("\n");
        
    }
    

    return 0;
}