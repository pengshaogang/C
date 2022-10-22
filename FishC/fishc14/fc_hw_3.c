#include <stdio.h>

int main()
{
    char ch;
    float i = 0;
    float sum = 0.0;
    int stats, stats1;

    do
    {
        printf("请输入合法的数字：");
        do
        {
            sum += i;
            stats = scanf("%f", &i);
            
        } while (stats == 1 && (ch = getchar()) != '\n');
        

        //stats1 = scanf("%f", &i);
        
        
    } while (stats == 1);
    
    printf("结果是：%.2f\n", sum);
    
    
    return 0;
}