#include <stdio.h>
#include <string.h>

int main()
{
    char ch[21];
    int i = 0;

    printf("请输入一行文本：");

    while ((ch[i++] = getchar()) != '\n')
    {
        ;
    }

    ch[20] = '\0';

    for (unsigned int j = strlen(ch); j >= 1; j--)
    {
        if (ch[j - 1] == ' ')
        {
            ch[j - 1] = '\0';
            break;
        }
        
    }
    
    

    printf("你输入的文本是：%s\n", ch);
    
    return 0;
}