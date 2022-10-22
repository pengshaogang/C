#include <stdio.h>

int main()
{
    int ch;
    int num_a = 0;
    int num_e = 0;
    int num_i = 0;
    int num_o = 0;
    int num_u = 0;

    printf("请输入一个英文句子：");

    while ((ch = getchar()) != '\n')
    {
        switch (ch)
        {
        case 'a':
        case 'A': 
            num_a += 1;
            continue;

        case 'e': 
        case 'E': 
            num_e += 1;
            continue;

        case 'i':
        case 'I': 
            num_i += 1;
            continue;

        case 'o': 
        case 'O': 
            num_o += 1;
            continue;

        case 'u': 
        case 'U': 
            num_u += 1;
            continue;
        
        
        default:
            continue;
        }
    }

    printf("您输入的句子中，包含元音字母%d个！\n", num_a + num_e + num_i + num_o + num_u);
    printf("其中：a(%d)，e(%d)，i(%d)，o(%d)，u(%d)\n", num_a, num_e, num_i, num_o, num_u);
    
    return 0;
}