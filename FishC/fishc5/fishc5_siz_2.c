#include <stdio.h>

int main()
{
    printf("int = %ld\n", sizeof(int));
    printf("short int = %ld\n", sizeof(short));
    printf("long int = %ld\n", sizeof(long));
    printf("long long int = %ld\n", sizeof(long long));
    printf("char = %ld\n", sizeof(char));
    printf("_Bool = %ld\n", sizeof(_Bool));
    printf("float = %ld\n", sizeof(float));
    printf("double = %ld\n", sizeof(double));
    printf("long double = %ld\n", sizeof(long double));


    return 0;
}