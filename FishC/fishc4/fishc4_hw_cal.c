#include <stdio.h>
#define PI 3.14159
#define S(r) PI*r*r
#define C(r) 2*PI*r
#define F(n) 2*n

int main()
{
    int r = 5;
    printf("半径为%d的圆, 面积是 : %.2f, 周长是 : %.2f\n", r, S(r), C(r));
    printf("fn: %d\n", F(3+2));
    return 0;
}