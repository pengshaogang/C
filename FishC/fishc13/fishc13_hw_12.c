#include <stdio.h>
#include <math.h>

int main()
{
    char ch;
    int is_flow = 0;
    long int temp;
    long int num = 0;
    const int max = pow(2, sizeof(int) * 8)/2 - 1;
    const int min = pow(2, sizeof(int) * 8)/2 * (-1);

    printf("请输入待转换的字符串 : ");
    do
    {
        ch = getchar();
        if(ch >= '0' && ch <= '9')
        {
            num = 10 * num + ch - '0';
            if(num > max || num < min)
            {
                is_flow = 1;
                break;
            }
            else
            {
                temp = num;
            }
        }
        else
        {
            if(num)
            {
                break;
            }
        }


    } while (ch != '\n');


    if(is_flow)
    {
        printf("数值超出范围，结果未定义 ！\n");
        
    }
    else
    {
        if(!num)
        {
            printf("并未找到任何数值 ！\n");
        }
        else
        {
            printf("%ld\n", num);
        }
    }


    
    

    

    



    
    return 0;
}