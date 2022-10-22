#include <stdio.h>

int main()
{
    int a = 3;
    int temp;
    temp = 2;
    for(; a == 3; a++)
    {
        if(a == 3)
        {
            temp = 3;
        }
    }
    printf("%d\n", temp);
    return 0;
}