#include <stdio.h>

int main()
{
    int count = 0;
    printf("请输入你的生日（如1988-05-20）:");
    int yir1, mir1, dir1;
    int yir2, mir2, dir2;
    scanf("%d-%d-%d", &yir1, &mir1, &dir1);
    printf("请输入今天的日期（如2016-03-28）:");
    scanf("%d-%d-%d", &yir2, &mir2, &dir2);
    int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    while (yir1 <= yir2)
    {
        days[1] = ((((yir1 % 4 == 0) && (yir1 % 100 !=0)) || yir1 % 400 == 0)) ? 29 : 28;

        while (mir1 <= 12)
        {
            while (dir1 <= days[mir1 -1])
            {
                if(yir1 == yir2 && mir1 == mir2 && dir1 == dir2)
                {
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

finder: printf("你在这个世界上总共生存了%d天\n", count);     
 
    
    return 0;
}