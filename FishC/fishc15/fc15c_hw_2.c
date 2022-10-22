#include <stdio.h>

int main()
{
    int ch;
    int count = 0;
    int count_a = 0;
    int count_e = 0;
    int count_i = 0;
    int count_o = 0;
    int count_u = 0;
    printf("请输入一个英文句子：");

    while ((ch = getchar()) != '\n')
    {
        if('a' == ch || 'A' == ch)
        {
            count_a += 1;
            continue;
        } 
        else if ('e' == ch || 'E' == ch)
        {
            count_e += 1;
            continue;
        }
        else if ('i' == ch || 'I' == ch) 
        {
            count_i += 1;
            continue;
        }
        else if ('o' == ch || 'O' == ch)
        {
            count_o += 1;
            continue;
        }
        else if ('u' == ch || 'U' == ch) 
        {
            count_u += 1;
            continue;
        }
        else
        {
            ;
        }
        
        
        
        
    }

    printf("您输入的句子中，包含元音字母%d个！\n", count_a + count_e + count_i + count_o + count_u);
    printf("其中，a(%d)，e(%d)，i(%d)，o(%d)，u(%d)\n", count_a, count_e, count_i, count_o, count_u);
  
    
    return 0;
}