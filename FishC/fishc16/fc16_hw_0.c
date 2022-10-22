#include <stdio.h>

int main()
{
    double loan = 10000;
    double tot_xiaojiayu = loan;
    double tot_night = loan;
    int i = 0;
    const float rate_1 = 0.1;
    const float rate_2 = 0.05;

    while (++i)
    {
        tot_xiaojiayu +=  loan * rate_1;
        tot_night += tot_night * rate_2;
        if(tot_night > tot_xiaojiayu)
        {
            printf("%d年后，黑夜的投资额超过小甲鱼！\n", i);
            printf("小甲鱼的投资额是：%.2f\n", tot_xiaojiayu);
            printf("黑夜的投资额是：%.2f\n", tot_night);
            break;
        }
    }
    
    return 0;
}