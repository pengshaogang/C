#include <stdio.h>
#include <math.h>

int main()
{
    long temp;
    const long PAR = 100;
    for(long i = 1; i < sqrt((double)PAR); i++)
    {
        if(!(PAR % i))
        {
            temp = i;
            printf("%ld\n", i);
        }
        
    }

    printf("其最大质数因子是%ld\n", temp);
    return 0;
}