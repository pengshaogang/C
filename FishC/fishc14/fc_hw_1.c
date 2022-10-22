#include <stdio.h>

int main()
{
    printf("公斤 --- 花费(元)：\n");
    for(int i = 1; i <= 20; i++)
    {
        printf("%2d --- %3d\n", i, 14*i+9);
    }
    return 0;
}