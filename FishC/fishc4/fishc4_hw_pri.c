#include <stdio.h>
#define FANXIE 3.7
#define JIELAN 7
#define XIQIN 1.3
#define KONGXINCAI 8
#define YANGCONG 2.4
#define YOUCAI 9
#define HUANGGUA 6.3
#define BAILUOBO 0.5
#define WEI 0.5

int main()
{
    
    printf("小明需要支付%.2f元\n", 2. * FANXIE* WEI + 1*KONGXINCAI*WEI+1*YANGCONG*WEI);
    printf("小红需要支付%.2f元\n", 3*WEI*XIQIN + 0.5*WEI*YANGCONG + 5*WEI*HUANGGUA);
    printf("小甲鱼需要支付%.2f元\n", 10*WEI*HUANGGUA + 20*WEI*BAILUOBO);
    return 0;
}