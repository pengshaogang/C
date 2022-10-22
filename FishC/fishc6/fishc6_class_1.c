#include <stdio.h>
#include <math.h>

int main()
{
    unsigned long int result = pow(2, 63) - 1;
    printf("result = %lu\n", result);
    return 0;
}