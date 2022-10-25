//hw0 in 动动手：请按要求恢复下边被无情抹掉的代码 https://fishc.com.cn/forum.php?mod=viewthread&tid=71653&extra=page%3D1%26filter%3Dtypeid%26typeid%3D570
#include <stdio.h>

int main(void)
{
    int a, b, c, t;
    int *pa, *pb, *pc;


    printf("请输入三个数：");
    scanf("%d%d%d", &a, &b, &c);

    pa = &a;
    pb = &b;
    pc = &c;

    if (a > b)
    {
        t = a;
        a = b;
        b = t;
    }

    if (a > c)
    {
        t = a;
        a = c;
        c = t;
        
    }

    if (b > c)
    {
        t = b;
        b = c;
        c = t;
    }
    

    printf("%d <= %d <= %d\n", *pa, *pb, *pc);

    return 0;
    
}



