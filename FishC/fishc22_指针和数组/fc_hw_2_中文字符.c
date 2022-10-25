//hw0 in 动动手：进阶要求： \
你可能发现写出来的代码只能统计英文字符的个数，遇到中文字符结果就会出错。\
请自行观察你当前系统对中文字符的处理方式，并设计一个可以统计中文字符以及中英文混合字符的程序。\
https://fishc.com.cn/forum.php?mod=viewthread&tid=72205&extra=page%3D1%26filter%3Dtypeid%26typeid%3D570

#include <stdio.h>

int main()
{
    char ch[] = "爱";
    int length, i;

    length = sizeof(ch) / sizeof(ch[0]);

    printf("%d\n", length);

    for ( i = 0; i < length; i++)
    {
        printf("str[%d]: %d\n", i, ch[i]);
    }
    

    
    return 0;
}