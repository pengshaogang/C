#include <stdio.h>

int main()
{
    int i, j, k;
    int count = 0, sum;
    int target, invert = 0;
    int temp = 0;
    _Bool is_flow = 0;
    int tt = 0;
    int nn;

    for(i = 10000; i < 1000000; i++)
    {
        //k = i;
        target = i;
        invert = 0;
        while (target)
        {
             invert = invert * 10 + target % 10;
             target = target / 10;
        }
        
        if(invert == i)
        {
            for(j = 100; j <= 999; j++)
            {
                if((i%j==0) && (i/j >= 100) && (i/j < 1000))
                {
                    //tt += 1;
                    //goto finder;
                    temp = i;
                    nn = j;
                    //break;
                    //printf("结果是%d == %d * %d\n", i, j, i / j);
                    //tt += 1;


                    //printf("%d\n", tt);
                //(i % 10)*count + ((i / 10)%10)*(count-1) + (((i / 10)/10)%10)*(count-2)
                }
                
                

                
            }
        
            
        }

//    printf("结果是%d == %d * %d\n", temp, j, temp / j);
        
    }
    if(temp)
    {
        printf("结果是%d == %d * %d\n", temp, nn, temp / nn);
    }
    
    //printf("%d\n", tt);
    return 0;
}