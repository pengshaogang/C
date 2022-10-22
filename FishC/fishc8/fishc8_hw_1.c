#include <stdio.h>

int main()
{
    float huashidu, sheshidu;
    printf("请输入华氏度 : ");
    scanf("%f", &huashidu);
    sheshidu = (huashidu-32)*5/9;
    printf("转换为摄氏度是 : %.2f\n", sheshidu);
    return 0;
}