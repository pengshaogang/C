//hw0 in 动动手：进阶要求： \
你可能发现写出来的代码只能统计英文字符的个数，遇到中文字符结果就会出错。\
请自行观察你当前系统对中文字符的处理方式，并设计一个可以统计中文字符以及中英文混合字符的程序。\
https://fishc.com.cn/forum.php?mod=viewthread&tid=72205&extra=page%3D1%26filter%3Dtypeid%26typeid%3D570

#include <stdio.h>
#define MAX 1024

int main()
{
    char ch[MAX];
    char *pch = ch;
    int count = 0;
    int i;

    printf("请输入一个字符串：");

    fgets(ch, MAX, stdin);

    while (*pch++ != '\0')
    {
        printf("%c", *(pch-1));
    }

    //printf("%c\n", *(pch++));
    
    
    
    

    
    return 0;
}