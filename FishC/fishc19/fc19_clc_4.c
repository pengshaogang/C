#include <stdio.h>
#include "string.h"



int main()
{
    char str1[30] = "I love";
    char str2[13] = "FishC.com!";


    //strncpy(str1, str2, 3);
    //str1[3] = '\0';
    strcat(str1, " ");
    strcat(str1, str2);
    strncat(str1, str2, 5);

    printf("str1: %s\n", str1);
    return 0;
}