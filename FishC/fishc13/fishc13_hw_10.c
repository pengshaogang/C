#include <stdio.h>
#include <math.h>

int main()
{
    const int max = pow(2, sizeof(int) * 8)/2 - 1;
    const int min = pow(2, sizeof(int) * 8)/2 * (-1);

    char ch, ch1;
    int is_flow = 0;
    long long int temp;
    long long int num = 0;
    printf("请输入待转换的字符串 : ");

    ch = getchar();

    if(ch >= '0' && ch <= '9')
    {
        num = 10 * num + ch - '0';
        while ((ch1=getchar()) != '\n')
        {
            if(ch1 >= '0' && ch1 <= '9')
            {
                num = 10 * num + ch1 - '0';
                if(num > max || num < min)
                {
                    is_flow = 1;
                    break;
                }
            }
            else
            {
                break;
            }
        }
        
    }
    
    else
    {
        while ((ch1=getchar()) != '\n')
        {
            if(ch1 >= '0' && ch1 <= '9')
            {
                num = 10 * num + ch1 - '0';
            }
            else
            {
                ;
            }
        }
        
    }

    if(is_flow)
    {
        printf("数值超出范围，结果未定义！\n");
    }
    else
    {
        if(!num)
        {
            printf("并未找到任何数值！\n");
        }
        else
        {
            printf("%lld\n", num);
        }
    }

    
    

    

    printf("\n");
    
    return 0;
}