#include <stdio.h>

int main()
{
    int yir;
    int mir[12];

    printf("请输入一个年份：");
    scanf("%d", &yir);

    if ((((yir % 4 == 0) && (yir % 100 !=0)) || yir % 400 == 0))
    {
        for (int i = 0; i < 12; i++)
        {
            if (i == 0 || i == 2 || i == 4 || i == 6 || i == 7 || i == 9 || i == 11)
            {
                mir[i] = 31;
            }
            else if (i == 1)
            {
                mir[i] = 29;
            }
            else
            {
                mir[i] = 30;
            }
            
            
        }
        
        
    }
    else
    {
        for (int j = 0; j < 12; j++)
        {
            if (j == 0 || j == 2 || j == 4 || j == 6 || j == 7 || j == 9 || j == 11)
            {
                mir[j] = 31;
            }
            else if (j == 1)
            {
                mir[j] = 28;
            }
            else
            {
                mir[j] = 30;
            }
        }
        
    }

    for (int k = 0; k < 12; k++)
    {
        printf("%2i月份：%d天\n", k + 1, mir[k]);
    }
    
    
    

    return 0;
}