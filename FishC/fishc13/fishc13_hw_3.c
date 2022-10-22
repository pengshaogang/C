#include <stdio.h>

int main()
{
    float num1, num2;
    char ch;
    printf("请输入式子 : ");
    scanf("%f %c %f", &num1, &ch, &num2);

    switch (ch)
    {
    case '+':
        printf("结果是 ：%.2f\n", num1 + num2);
        break;
    case '-':
        printf("结果是 ：%.2f\n", num1 - num2);
        break;
    case '*':
        printf("结果是 ：%.2f\n", num1 * num2);
        break;
    case '/':
        if(0 == num2)
        {
            printf("很遗憾，除数不能为零 !\n");
            break;
        }
        printf("结果是 ：%.2f\n", num1 / num2);
        break;
    default:
        break;
    }
    return 0;
}