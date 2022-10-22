#include <stdio.h>

#define STRING char *
#define IF if(
#define THEN ){
#define ELSE }else{
#define FI ;}
#define WHILE while(
#define DO ){
#define OD ;}
#define INT int
#define BEGIN {
#define END }

INT main()
BEGIN
    INT i;
    printf("请输入您的年龄: ");
    scanf("%d", &i);

    IF i < 18
    THEN
        printf("您未满18周岁，不得使用这个程序！\n");
    ELSE
        printf("您已满18周岁，欢迎使用本程序，嘿嘿嘿...\n")
    FI
        

    return 0;
END
