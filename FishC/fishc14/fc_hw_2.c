#include <stdio.h>

int main()
{
    int stats, i;
    int sum = 0;
    do
    {
        printf("请输入合法的数字：");
        stats = scanf("%d", &i);
        sum += i;
        
    } while (stats == 1);

    printf("结果是：%d\n", sum-i);
    
    return 0;
}