#include <stdio.h>

int main()
{
    char i;
    printf("请输入一个字符:");
    scanf("%c", &i);

    if(i >= 97 && i <= 122)
    {
        i -= 32;
        printf("%c\n", i);
    }
    else if (i >= 65 && i <= 90)
    {
        i += 32;
        printf("%c\n", i);
    }
    else
    {
        printf("%c\n", i);
    }

    
    return 0;
}