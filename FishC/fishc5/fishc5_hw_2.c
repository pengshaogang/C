#include <stdio.h>
#include <math.h>
#define WEI 25000

int main()
{
    unsigned long long int num_mai = 1;
    unsigned long long int temp;

    
    for(int i=1; i<=64; i++)
    {
        temp = pow(2,i);
        num_mai += temp;
    }
    printf("舍罕王应该给予达依尔%llu粒麦子 !\n", num_mai);
    printf("如果每%d粒麦子为1kg, 那么应该给%llukg麦子 !\n", WEI, num_mai/WEI);
    return 0;
}