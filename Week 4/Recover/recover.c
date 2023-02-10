#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <cs50.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{


    if (argc != 2)
    {
        printf("Usage: ./recover card.raw\n");
        return 1;
    }


    // 1. Open Memory Card
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Cannot open file");
        return 2;
    }

    // 2. loop through block
    BYTE buffer[512];
    int count = 0;
    FILE *image = NULL;
    char output[8];

    // printf("Starting loop \n");
    while (fread(&buffer, 512, 1, input) == 1)
    {
        // printf("Starting image %d\n", count);
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0)
        {
            // Close previous jpeg
            if (count != 0)
            {
                fclose(image);
            }

            // Open new image file
            sprintf(output, "%03i.jpg", count);
            image = fopen(output, "w");
            count++;
        }

        if (count != 0)
        {
            fwrite(&buffer, 512, 1, image);
        }
        // printf("Created image %d\n", count);
    }
    // Close files
    fclose(input);
    fclose(image);
    return 0;
}