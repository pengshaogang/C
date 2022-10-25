//hw2 in 动动手：改进上一题的代码，用户输入一个表示范围的最大值count, 程序将测试3 ~ count所有的整数是否都符合尼科彻斯定理。\
附加要求：有些用户可能不希望看到打印一丢丢式子，所以是否打印应该让用户决定（提示->用二维数组先保存结果）。\
https://fishc.com.cn/forum.php?mod=viewthread&tid=71653&extra=page%3D1%26filter%3Dtypeid%26typeid%3D570

#include <stdio.h>
#include <math.h>

int main()
{
    int num;
    int i, j, k;
    int sum = 0;
    int powi;
    _Bool isflow = 0;
    int count = 0;
    char ch;


    printf("请输入一个整数：");
    scanf("%d", &num);
    getchar();

    int a[num][3];
    int b[num];

    for ( i = 3; i <= num; i++)
    {
        powi = (int)pow(i, 3);
        for ( j = 1; j < powi/2; j+=2)
        {
            sum = 0;
            isflow = 0;
            for ( k = j; k < powi/2; k+=2)
            {
                if (sum <= powi)
                {
                    sum += k;
                    if (sum == powi)
                    {
                        isflow = 1;
                        
                        a[i][0] = j;
                        a[i][1] = j + 2;
                        a[i][2] = k;
                        
                        

                        //printf("%d-%d-%d\n", a[i][0], a[i][1], a[i][2]);

                        count += 1;
                        
                        
                        break;
                    }
                    
                }
                
            }
            if (isflow)
            {
                break;
            }
            
            
        }
        
        
    }

    if (count == num - 2)
    {
        printf("经验证，3 ~ %d 之间所有的整数均符合尼科彻斯定理!\n", num);
    }

    printf("是否打印所有式子(y/n):");
    if((ch = getchar()) == 'y')
    {
        for (int s = 3; s <= num; s++)
        {
            if (a[s][2] - a[s][0] > 4)
            {
                printf("%d^3 == %d == %d + %d +... + %d\n", s, (int)pow(s,3), a[s][0], a[s][1], a[s][2]);
            }
            else {
                printf("%d^3 == %d == %d + %d + %d\n", s, (int)pow(s,3), a[s][0], a[s][1], a[s][2]);
            }
            
            
        }
        
    } else {
        ;
    }
    
    
    

    return 0;
}