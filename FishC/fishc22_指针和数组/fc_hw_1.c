//hw0 in 动动手：获取字符串的长度------strlen函数 \
基础要求：使用fgets函数读取用户输入的字符串(英文)，并用指针法来计算字符串的字符个数。\
https://fishc.com.cn/forum.php?mod=viewthread&tid=72205&extra=page%3D1%26filter%3Dtypeid%26typeid%3D570

#include <stdio.h>
#define MAX 512

int main()
{
    char str[MAX];
    char *ps = str;
    int count = 0;

    printf("请输入一个字符串：");

    fgets(str, MAX, stdin);

    while (*ps++ != '\0')
    {
        count++;
    }

    printf("您总共输入了 %d 个字符！\n", count-1);
    


    return 0;
}