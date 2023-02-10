#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

string substitute(string word, string key);
char getNewLetter(char c, string key);
bool checkValidCharacters(string key);
bool checkDuplicateCharacters(string key);
bool validateKey(string key);


int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Missing Key\n");
        return 1;
    }
    else if (validateKey(argv[1]) == false)
    {
        return 1;
    }

    string input = get_string("plaintext: ");
    printf("ciphertext: %s\n", substitute(input, argv[1]));
}

bool validateKey(string key)
{
    if (checkValidCharacters(key) == false)
    {
        printf("Contains invalid characters in key\n");
    }
    else if (checkDuplicateCharacters(key) == false)
    {
        printf("Key contains duplicate characters\n");
    }
    else if (strlen(key) != 26)
    {

        printf("Key must contain 26 characters.\n");
    }
    else
    {
        return true;
    }
    return false;
}

bool checkValidCharacters(string key)
{
    for (int i = 0; i < strlen(key); i++)
    {
        if (isalpha(key[i]) == 0)
        {
            return false;

        }
    }
    return true;
}

bool checkDuplicateCharacters(string key)
{
    int letters[255] = {};
    for (int i = 0; i < strlen(key); i++)
    {
        if (letters[(int)key[i]] == '\0')
        {
            letters[(int)key[i]] = 1;

        }
        else
        {
            return false;

        }
    }
    return true;
}

string substitute(string word, string key)
{
    for (int i = 0; i < strlen(word); i++)
    {
        word[i] = getNewLetter(word[i], key);
    }
    return word;
}

char getNewLetter(char c, string key)
{
    if (isalpha(c) != 0)
    {
        if (islower(c) == 512)
        {
            c = tolower((key[tolower(c) - 97]));
        }
        else
        {
            c = toupper(key[tolower(c) - 97]);
        }
    }
    return c;
}


