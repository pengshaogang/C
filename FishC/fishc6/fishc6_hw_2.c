#include <stdio.h>
#include <math.h>

int main()
{
    int i;
    float temp;
    printf("请输入一个整数 : ");
    scanf("%d", &i);
    temp = pow(i,5);
    printf("%d的五次方是 : %14.2f\n", i, temp);
    return 0;
}