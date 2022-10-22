#include <stdio.h>

int main()
{
    char str[30];
    char str1;
    int count = 0;
    printf("请输入一行英文句子 ：");
    //scanf("%[^\n]",str);

    

    
    while((str1 = getchar()) != '\n')
    {
        if(str1 <= 'Z' && str1 >= 'A')
        {
            count += 1;
        }
    

        
        
    }
    
    printf("你总共输入了%d个大写字母 !\n", count);

    
    return 0;
}