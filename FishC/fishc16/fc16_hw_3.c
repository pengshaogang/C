#include <stdio.h>

int main()
{
    
    const int num = 2;
    int sum = 0;
    sum += num;

    int a = 1, b = 1;
    int c;

    for(int i = 3; i <= 24; i++)
    {
        c = a + b;
        b = a;
        a = c;

        
    }

    printf("两年之后可以繁殖%d对兔子！\n", c);


    

    return 0;
}