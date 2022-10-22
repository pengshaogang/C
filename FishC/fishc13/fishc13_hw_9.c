#include <stdio.h>

int main()
{
    char ch, ch1;
    printf("请输入待转换的字符串 : ");

    ch = getchar();

    if(ch >= '1' && ch <= '9')
    {
        putchar(ch);
        while ((ch1=getchar()) != '\n')
        {
            if(ch1 >= '1' && ch1 <= '9')
            {
                putchar(ch1);
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
            if(ch1 >= '1' && ch1 <= '9')
            {
                putchar(ch1);
            }
            else
            {
                ;
            }
        }
        
    }
    

    

    printf("\n");
    
    return 0;
}