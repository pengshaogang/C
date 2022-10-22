#include <stdio.h>
#include <math.h>

int main()
{
    float fiv_rate,tot_5,thr_rate,tot_3_2,tot_3;
    float one_rate,tot_1,tot_1_5,tw_rate;
    fiv_rate = 2.75 / 100;
    tot_5 = 10000 * (1 + fiv_rate * 5);
    printf("一次性定期存5年,存得的本息和为 %.2f\n", tot_5);


    thr_rate = 2.75 / 100;
    tw_rate = 2.25 / 100;
    tot_3 = 10000 * (1 + thr_rate * 3);
    tot_3_2 = tot_3 * (1 + tw_rate * 2);
    printf("先存3年定期，到期后本息再存2年定期，得到的本息和为 %.2f\n", tot_3_2);

    one_rate = 1.75 / 100;
    tot_1 = 10000 * (1 + one_rate);
    tot_1_5 = tot_1 * pow((1 + one_rate),4);

    printf("存1年定期，到期后本息再存1年定期，连续存5次，得到的本息和为 %.2f\n", tot_1_5);
    
    return 0;
}