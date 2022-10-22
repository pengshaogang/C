#include <stdio.h>
#include <string.h>

int main()
{
    char str[] = "I love FishC.com!";

    printf("sizeof str = %ld\n", sizeof(str));
    printf("strlen str = %lu\n", strlen(str));

    return 0;
}