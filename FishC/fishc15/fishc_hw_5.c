#include <stdio.h>
#include <math.h>


int main()
{
    int isflow = 0;
    int cout = 0;

    for(int i = 2; i <= 10000; i++)
    {
        if(2 == i || 3 == i)
        {
            cout += 1;
        }
        for(int j = 2; j <= sqrt(i); j++)
        {
            if(i % (int)j == 0)
            {
                isflow = 1;
                break;
            }
            isflow = 2;
        }
        if(isflow == 2)
        {
            cout += 1;
        }
    }
      
    printf("10000以内共有%d个素数！\n", cout);
    
    return 0;
}