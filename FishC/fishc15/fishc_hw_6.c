#include <stdio.h>

int main()
{
    const int PAR = 3;

    int ch;

    printf("请输入明文：");

    while ((ch = getchar()) != '\n')
    {
        if((ch >= 'a' && ch <= 'w') || (ch >= 'A' && ch <= 'W'))
        {
            putchar(ch + PAR);
        }
        else if (('x' == ch || 'y' == ch || 'z' == ch))
        {
            putchar(PAR - 'z' + ch + 'a');
        }
        else if (('X' == ch || 'Y' == ch || 'Z' == ch))
        {
            putchar(PAR - 'Z' + ch + 'A');
        }
        else
        {
            putchar(ch);
        }
        
        
    }

    putchar('\n');
    
    return 0;
}