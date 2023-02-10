#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int height = 0;
    while (height < 1 || height > 8)
    {
        height = get_int("Height: ");
    }


    // Row level
    for (int i = 1;  i <= height; i++)
    {
        // Make spaces equal to height - row (i)
        for (int j = 0; j < (height - i); j++)
        {
            printf(" ");
        }

        // Print # equal to row (i)
        for (int j = 0; j < i; j++)
        {
            printf("#");
        }

        // Print space in between
        printf("  ");

        // Print # equal to row (i)
        for (int j = 0; j < i; j++)
        {
            printf("#");
        }

        printf("\n");
    }
}

/*

int main(void) {
   int height = get_int("Height: ");

   // Row level
   for (int i = 0;  i < height; i++) {
        // Make spaces equal to height - row (i)
        for (int j = 0; j < (height - i); j++) {
            printf(" ");
        }

        // Print # equal to row (i)
        for (int j = 0; j < i; j++) {
            printf("#");
        }
        printf("\n");
   }
}

*/