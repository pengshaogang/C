#include <stdio.h>
#include <math.h>

int main()
{
    const double f1 = 1;
    
    double i = 0, j = 0;
    double temp = 0;
    double tt, t;
    const int pot = 1e7;
    const double pot1 = 1e-8;
    
    while (++i)
    {
        //printf("%d\n", (int)i % 2);
        //temp +=  
        if((t = (2*i-1 > 0) ? 1/(2*i-1) : 1/(1-2*i)) < pot1)
        {
            printf("%.7f\n", tt);
            break;
        }
        
        temp +=  ((int)i % 2) ? 1/(2*i-1) : 1/(1-2*i);
        
        tt = temp * 4;
        

        

        
        

    }

    return 0;
}
