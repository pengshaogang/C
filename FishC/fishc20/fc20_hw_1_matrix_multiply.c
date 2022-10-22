#include <stdio.h>
#define M 2
#define N 2
#define P 3

int main()
{
    int a[M][P] = {
        {1, 2, 3},
        {4, 5, 6}};

    int b[P][N] = {
        {1, 4},
        {2, 5},
        {3, 6}};

    int c[M][N] = {0};

    for (size_t i = 0; i < M; i++)
    {
        for (size_t j = 0; j < N; j++)
        {
            for (size_t k = 0; k < P; k++)
            {
                c[i][j] += a[i][k] * b[k][j];
            }
            
        }
        
        
    }

    


    int row;
    row = (M > P) ? M : P;
    //printf("%d\n", row);


    for (size_t i = 0; i < row; i++)
    {
        // 打印A
        printf("|   ");
        for (size_t j = 0; j < P; j++)
        {
            if(i < M)
            {
                printf("\b%d ", a[i][j]);
                printf("|");
            }
            else
            {
                printf("\b\b\b     ");
            }
        }
        
        
        
        
    }
    
    
    
    return 0;
}