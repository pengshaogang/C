#include <stdio.h>
#include <string.h>

int main()
{
        char str[10];

        printf("sizeof: %lu\n", sizeof(str));

        strncat(str, "I love FishC.com!", sizeof(str)-1);

        printf("str: %s\n", str);

        return 0;
}