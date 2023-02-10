#include <stdio.h>
#include <cs50.h>
#include <math.h>
#include <stdlib.h>

int main(void)
{

    // Get and process input
    long input = 0;
    while (input <= 0)
    {
        input = get_long("Number: ");
    }

    // Get number of digits - to be used in other areas
    int nDigits = floor(log10(labs(input))) + 1;

    // Assume false
    bool valid = false;
    if (nDigits == 13 || nDigits == 15 || nDigits  == 16)
    {
        valid = true;
    }

    // Perform checksum
    if (valid == true)
    {
        int otherDigits = 0;
        int digits = 0;
        int digit = 0;

        // Sum of digits
        for (int i = 1; i < nDigits + 2; i = i + 2)
        {
            digit = input % ((long) pow(10, i)) / (long) pow(10, i - 1);
            digits += digit;
        }

        // Sum of other digits
        for (int i = 2; i < nDigits + 2; i = i + 2)
        {
            digit = (input % ((long) pow(10, i)) / (long) pow(10, i - 1));
            digit = digit * 2;
            int digitLength = floor(log10(labs(digit))) + 1;

            // Inner for loop to add digits
            for (int j = digitLength; j > 0; j--)
            {
                int subDigit = digit % ((long) pow(10, j)) / (long) pow(10, j - 1);
                otherDigits += subDigit;
            }
        }

        // Compute checksum
        int total = otherDigits + digits;
        if (total % 10 != 0)
        {
            valid = false;
        }
    }

    if (valid == true)
    {
        // Confirm first two digits
        int firstDigit = (input % ((long) pow(10, nDigits)) / (long) pow(10, nDigits - 1));
        int secondDigit = (input % ((long) pow(10, nDigits - 1)) / (long) pow(10, nDigits - 2));

        // Assume false again again
        valid = false;

        // AMEX - 15 in length starts with 34 or 37
        if (nDigits == 15)
        {
            if (firstDigit == 3)
            {
                if (secondDigit == 4 || secondDigit == 7)
                {
                    valid = true;
                    printf("AMEX\n");
                }
            }
        }

        // MasterCard - 16 in length starts with 51, 52, 53, 54, 55
        if (nDigits == 16)
        {
            if (firstDigit == 5)
            {
                if (secondDigit == 1 || secondDigit == 2 || secondDigit == 3 || secondDigit == 4 || secondDigit == 5)
                {
                    valid = true;
                    printf("MASTERCARD\n");
                }
            }
        }

        // Visa - 13 and 16 in length starts with 4
        if (nDigits == 13 || nDigits == 16)
        {
            if (firstDigit == 4)
            {
                valid = true;
                printf("VISA\n");
            }
        }
    }

    // Prints false
    if (valid == false)
    {
        printf("INVALID\n");
    }

}