#include <stdio.h>

int main()
{
    char a = 'F';
    int f = 123;
    float c = 32.;

    char *pa = &a;
    int *pb = &f;
    float *pc = &c;

    printf("a = %c\n", *pa);
    printf("f = %d\n", *pb);

    *pa = 'C';
    *pb += 1;

    printf("now, a = %c\n", *pa);
    printf("now, f = %d\n", *pb);

    printf("sizeof pa = %lu\n", sizeof(pa));
    printf("sizeof pb = %lu\n", sizeof(pb));
    printf("sizeof pc = %lu\n", sizeof(pc));

    printf("address pa = %p\n", pa);
    printf("address pb = %p\n", pb);
    printf("address pc = %p\n", pc);



    return 0;
}