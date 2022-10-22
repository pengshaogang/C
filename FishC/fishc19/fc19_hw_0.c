#include <stdio.h>
#define MAX 1024

int main()
{
    char ch1[MAX];
    char ch2[MAX];
    int i = 0;
    int j;
    printf("请输入第一个字符串：");
    
    while ((ch1[i++] = getchar()) != '\n')
    {
        ;
    }

    i = 0;
    printf("请输入第二个字符串：");

    while ((ch2[i++] = getchar()) != '\n')
    {
        ;
    }

    printf("请输入比较的字符数：");
    scanf("%d", &j);

    

    for (i = 0; i < j; i++)
    {
        if (ch1[i] - ch2[i])
        {
            i++;
            //printf("hello");
            break;
        }
        
    }

    //printf("%d\n", ch1[i - 1]);
    //printf("%d\n", ch2[i - 1]);

    printf("比较的结果是：%d\n", ch1[i - 1] - ch2[i - 1]);
    


    
    

    

    return 0;
}