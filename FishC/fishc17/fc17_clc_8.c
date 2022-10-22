#include <stdio.h>

int main()
{
    int count = 0;
    int count1 = 0;
    printf("请输入你的生日（如1988-05-20）:");
    int yir1, mir1, dir1;
    int yir2, mir2, dir2;
    int yir3, mir3, dir3;
    scanf("%d-%d-%d", &yir1, &mir1, &dir1);
    printf("请输入今天的日期（如2016-03-28）:");
    scanf("%d-%d-%d", &yir2, &mir2, &dir2);
    
    int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    yir3 = yir1 + 80;
    mir3 = mir1;
    dir3 = dir1;

    while (1)
    {
        days[1] = ((((yir1 % 4 == 0) && (yir1 % 100 !=0)) || yir1 % 400 == 0)) ? 29 : 28;

        while (mir1 <= 12)
        {
            while (dir1 <= days[mir1 -1])
            {
                if(yir1 == yir2 && mir1 == mir2 && dir1 == dir2)
                {
                    count1 = count;
                    printf("你在这个世界上总共生存了%d天\n", count);
                }

                if(yir1 == yir3 && mir1 == mir3 && dir1 == dir3)
                {
                    printf("如果能活到80岁，你还剩下%d天\n", count - count1);
                    printf("你已经使用了%.2f%%的生命，请好好珍惜剩下的时间！\n", (double)count1 * 100 /count);
                    goto finder;
                }


                dir1 += 1;
                count += 1;
                
            }
            dir1 = 1;
            mir1 += 1;
            
            
        }

        yir1 += 1;
        mir1 = 1;
        
        

        
        
        
    }

 
    
finder: return 0;
}