#include <stdio.h>

int main()
{
    unsigned char name[10];
    float hei, trans_hei;
    float wei, trans_wei;
    printf("请输入您的姓名 : ");
    scanf("%s", name);
    printf("请输入您的身高(cm) : ");
    scanf("%f", &hei);
    
    printf("请输入您的体重(kg) : ");
    scanf("%f", &wei);
    
    printf("========== 正在为您转换 =============\n");
    trans_hei =  hei / 2.54;
    trans_wei = 2.204623 * wei;
    printf("%s的身高是%.2f(in), 体重是%.2f(lb)。\n",name, trans_hei, trans_wei);
    return 0;
}