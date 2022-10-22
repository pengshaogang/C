#include <stdio.h>

int main()
{
    int a = 1, b = 1;
    int c = 0;
    int sum = 0;

    do
    {
        c = a + b;
        b = a;
        a = c;
        if(c > 4000000)
        {
            break;
        }
        if(!(c % 2))
        {
            sum += c;
            
        }
        printf("%d\n", c);
    } while (c <= 4000000);

    printf("偶数项的和为%d\n", sum);
    
    
    return 0;
}