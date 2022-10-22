#include <stdio.h>
#include <string.h>
#define NUM 1024

int main()
{
    int ch, max = 0;
    int j = 0;
    printf("请输入英文文本：");
    int assi[NUM] = {0};
    int input = 0;
    char count[NUM]="";

    while ((ch = getchar()) != '\n')
    {
        assi[ch]++;
        input++;
    }

    for (int i = 0; i < NUM; i++)
    {
        

        if(assi[i])
        {
            count[j++] = i;
            if (assi[i] > assi[max])
            {
                max = i;
            }
        }
        
    }


    printf("你总共输入了%d个字符，其中不同的字符个数有%lu个。\n", input, strlen(count));
    printf("它们是：%s\n", count);
    printf("出现次数最多的字符是'%c'，它总共出现了%d次。\n", max, assi[max]);
    
    
    return 0;
}