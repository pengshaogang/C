#include <stdio.h>
#include "ssebtr1.h"

#define BPM 150

int main()
{
    int age;
    float rmh;
    printf("用户输入年龄:");
    scanf("%d", &age);

    rmh = 220. - age;

    return 0;
}