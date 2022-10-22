#include <stdio.h>

int main()
{
    char ch[100];
    int count = 0;

    printf("请输入一行英文字符:");
    
    while (getchar() != '\n')
    {
        count += 1;
        
    }

    printf("你总共输入了%d个字符 ! \n", count);
    
    return 0;
}