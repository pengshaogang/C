#include <stdio.h>

int main()
{
        int i;

        printf("请输入一个整数：");
        scanf("%d", &i);

        switch (i)
        {
                case 1 + 1: printf("1 + 1\n"); break;
                case 16 / 4: printf("2 / 1\n"); break;
                case 1 + 2: printf("1 + 2\n"); break;
                case 1 - 2: printf("1 - 2\n"); break;
                case 1 - 3: printf("1 - 3\n"); break;
        }

        return 0;
}