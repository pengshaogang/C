#include <stdio.h>
#define NUM 5
#include <string.h>

int main()
{
    char slogans[NUM][100] = {""};
    int i = 0, j = 0, k = 0, t = 0, s = 0, min = 0, min1 = 0, max = 0;
    _Bool is_flow = 0;
    printf("请输入1句话：");
    while ((slogans[0][i++] = getchar()) != '\n')
    {
        ;
        
    }

    printf("请输入2句话：");
    while ((slogans[1][j++] = getchar()) != '\n')
    {
        ;
    }

    printf("请输入3句话：");
    while ((slogans[2][k++] = getchar()) != '\n')
    {
        ;
    }

    printf("请输入4句话：");
    while ((slogans[3][t++] = getchar()) != '\n')
    {
        ;
    }

    printf("请输入5句话：");
    while ((slogans[4][s++] = getchar()) != '\n')
    {
        ;
    }
    
    
    printf("你输入了下边5句话：\n");
    for (size_t i = 0; i < sizeof(slogans) / sizeof(slogans[0][0]); i++)
    {
        
        printf("%c", slogans[0][i]);
    }

    for (size_t i = 0; i < 5; i++)
    {
        if (strlen(slogans[i]) > max)
        {
            is_flow = 1;
            max = strlen(slogans[i]);
        }
        
    }

    min = strlen(slogans[0]);
    for (size_t j = 0; j < 5; j++)
    {
        if (strlen(slogans[j]) < min)
        {
            min = strlen(slogans[j]);
        }
                
    }

    
    for (size_t i = 0; i < 5; i++)
    {
        if (max == strlen(slogans[i]))
        {
            printf("其中最长的是：%s", slogans[i]);
        }
        if (min == strlen(slogans[i]))
        {
            printf("其中最短的是：%s\n", slogans[i]);
        }
        
            
    }
        
    
    
    
    
    
    
    
    
    return 0;
}