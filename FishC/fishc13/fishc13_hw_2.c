#include <stdio.h>
#include <string.h>

int main()
{
        char ch[6] = "FishC";
        int i, len;

        printf("请输入你想访问的字符序号(0~5)：");
        scanf("%d", &i);
        //printf("%d\n", i);


        len  = strlen(ch);

        //printf("%ld\n", sizeof(strlen));
        
        
        if(i > strlen(ch)-1)
        {
            printf("数组越界\n");
        }
        else if (i >= 0 && i <= len)
        {
            printf("%c\n", ch[i]);
        }
        
        

        return 0;
}