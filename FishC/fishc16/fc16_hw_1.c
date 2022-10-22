#include <stdio.h>

int main()
{
    const double loan = 4000000;
    const double output = 500000;
    const double rate = 0.08;
    double tot = loan;
    int i = 0;
    while (++i)
    {
        tot = tot - output;
        tot += tot * rate;
        
        if(tot < 0)
        {
            printf("%d年之后，小甲鱼败光了所有的家产，再次回到一贫如洗......", i);
            break;
        }
    }
    
    return 0;
}