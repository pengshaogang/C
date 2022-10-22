#include <stdio.h>

int main()
{
    double i;
    int cout=0;

    for (i = 10; i > 0; i /= 2)
    {
        printf("%.2f\n", i);
        cout += 1;
        
    }
    printf("%d\n", cout);

    return 0;
}