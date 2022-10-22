#include <stdio.h>

int main()
{
        int i = 0;

        while (i++)
        {
                if (i > 10)
                {
                        goto Label;
                }
                putchar('B');
        }

Label:  putchar('\n');

        return 0;
}