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

        int i, j, k, row;

        for (i = 0; i < M; i++)
        {
                for (j = 0; j < N; j++)
                {
                        for (k = 0; k < P; k++)
                        {
                                c[i][j] += a[i][k] * b[k][j];
                        }
                }
        }

        // row 取行数最大值
        row = M > P ? M : P;

        for (i = 0; i < row; i++)
        {
            if (i < row - 1)
            {
                printf("|");
            }
               
            
            
            
            //打印A
            for (size_t j = 0; j < P; j++)
            {
                
                if (i < P - 1)
                {
                        
                    printf(" %d ", a[i][j]);
                        
                        
                        
                }
                else {
                    printf("\b\b\b\b\b       ");
                }
                    
                    
                    
            }

            if (i < row - 1)
            {
                printf("|");
            }

            
            if (i == P - 2)
            {
                printf(" *  ");
            }
            else
            {
                printf("    ");
            }
            if (i < row )
            {
                
                printf("|");
            }

            
            //打印B
            for (size_t k = 0; k < N; k++)
            {
                printf(" ");

                
                if (i < P)
                {
                    
                    printf("%d ", b[i][k]);
                    //printf("|");
                    
                    
                }   
                    //printf("|");

                    
                
                
            }

            
            
            
            printf("|");

            //打印 =
            if (i == P - 2)
            {
                printf(" =  ");
            }
            else
            {
                printf("    ");
            }

            if (i < row - 1 )
            {
                
                printf("|");
            }


            for (size_t k = 0; k < N; k++)
            {
                printf(" ");

                
                if (i < N)
                {
                    
                    printf("%d ", c[i][k]);
                    //printf("|");
                    
                    
                }   
                    //printf("|");

                    
               
                
            }

            if (i < row - 1)
            {
                printf("|");
            }

            


            printf("\n");
                
        }
       

        return 0;
}