#include <stdio.h>
#include <string.h>
#define MAX 1024

int main()
{
    char a[MAX];
    char b[MAX];
    int count = 1;
    //char ch[2];
    
    int i = 0;
    int j = 0;

    printf("请输入英文文本：");

    while ((a[i++] = getchar()) != '\n')
    {
        ;
    }

    a[strlen(a)-1] = '\0';

    char ch[2] = {a[0]};
    strcat(b, ch);

    unsigned int len;
    len = strlen(b);
    _Bool is_flow = 0;
    for (int i = 1; i < strlen(a); i++)
    {
        for(j = 0; j < len; j++)
        {
            if (a[i] == b[j])
            {
                is_flow = 1;
                break;
            }

            is_flow = 0;

        
            
        }
        if(!is_flow)
        {
            char ch[2] = {a[i]};
            strcat(b, ch);
            len += 1;
            count += 1;
            
            
        }


        
    }





    printf("它们是：%s\n", b);
    

    printf("你总共输入了%lu个字符，其中不同的字符个数有%d个。\n", strlen(a), count);

    return 0; 
}