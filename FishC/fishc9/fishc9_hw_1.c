#include <stdio.h>
#include <math.h>

int main()
{
    float per_price;
    float area;
    int num_ch;
    int num_year;
    float ratio;
    printf("请输入单价 (元/平方) : ");
    scanf("%f", &per_price);
    printf("请输入面积 : ");
    scanf("%f", &area);
    printf("请输入按揭成数 : ");
    scanf("%d", &num_ch);
    printf("请输入按揭年数 : ");
    scanf("%d", &num_year);
    printf("请输入当前基准年利率 : ");
    scanf("%f", &ratio);
    printf("============ 报告结果 ============\n");
    float tot;
    tot = per_price * area;
    printf("房款总额 : %.2f元\n", tot);
    float benjin, price_fir;
    benjin = tot * num_ch / 10;
    price_fir = tot - benjin;
    printf("首期付款 : %.2f元\n", price_fir);
    printf("贷款总额 : %.2f元\n", benjin);
    float price_mon;
    float ratio_mon;
    ratio_mon = ratio / 12 / 100;
    price_mon = benjin*(ratio_mon * pow(1+ratio_mon,num_year*12))/(pow(1+ratio_mon,num_year*12)-1);
    float lixi;
    lixi = num_year * 12 * price_mon - benjin;
    float tot_huankuan;
    tot_huankuan = tot + lixi - price_fir;
    printf("还款总额 : %.2f元\n", tot_huankuan);
    printf("支付利息 : %.2f元\n", lixi);
    printf("月均还款 : %.2f元\n", price_mon);
    return 0;
}