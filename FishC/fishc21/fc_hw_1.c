//hw1 in 动动手：验证尼科彻斯定理：任何一个大于2的整数的立方都可以表示成一串连续奇数的和，这些奇数一定是要连续的，\
要求：用户输入某个整数，验证该数是否符合尼科彻斯定理，并将对应的式子打印出来。\
https://fishc.com.cn/forum.php?mod=viewthread&tid=71653&extra=page%3D1%26filter%3Dtypeid%26typeid%3D570

#include <stdio.h>
#include <math.h>

int main()
{
    int num;
    int i, j;
    int sum = 0;
    int *pnum = &i;
    printf("请输入一个整数：");
    scanf("%d", &num);
    //printf("%d\n", *pnum);
    for (i = 1; i < num * num * num / 2; i+=2)
    {
        sum = 0;
        
        for (j = i; j < num * num * num / 2; j+=2)
        {
            if (sum <= num * num * num)
            {
                sum += j;
                if (sum == num * num * num)
                {
                    if (j - i > 4)
                    {
                        printf("%d = %d + %d ... + %d\n", (int)pow(num, 3), i, i+2, j);
                        goto find;
                    }
                    else {
                        printf("%d = %d + %d + %d\n", (int)pow(num, 3), i, i+2, i+4);
                    }
                    
                }
                
            } else {
                break;
            }

            
            
            
        }
        
    }

//find: printf("%d + %d + %d\n", i, j-2, j);

find:    return 0;
}