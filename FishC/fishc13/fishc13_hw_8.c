#include <stdio.h>

int main()
{
    char ch,ch1;
    printf("请输入一行英文句子 : ");

    while ((ch = getchar()) != '\n')
    {
        if(ch >= 97 && ch <= 122)
        {
            putchar(ch-32);
        }
        else if (ch >= 65 && ch <= 90)
        {
            putchar(ch+32);
        }
        else
        {
            putchar(ch);
        }
        

    }

    printf("\n");

    
    
    
    
    return 0;
}