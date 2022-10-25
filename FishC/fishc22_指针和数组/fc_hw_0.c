#include <stdio.h>
 

int main()
{
    char str[20] = "I love FishC.com!";


    printf("str[10]: %c\n", str[10]);
    printf("str[10]: %c\n", *(str + 10));
    printf("str[10]: %c\n", [20]str);
    //str[20] == *(str + 20) == *(20 + str) == [20]str;

    return 0;

}



