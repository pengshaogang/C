#include <stdio.h>
#include <string.h>

int main()
{
	char ch1, ch2;
	char ch[4];	
	strcpy(ch1, " ");
	strcpy(ch2, "9");

	strcat(ch1, ch2);
	

	printf("%s\n", ch);

	return 0;
}
