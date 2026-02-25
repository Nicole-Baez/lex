/*
Assignment:
lex - Lexical Analyzer for PL/0
Author: Nicole Baez Espinosa, Lianet Martin
Language: C(only)
To Compile:
gcc -O2 -std=c11 -o lex lex.c
To Execute (on Eustis):
./lex <input file>
where:
<input file> is the path to the PL/0 source program
Notes:
- Implement a lexical analyser for the PL/0 language.
- The program must detect errors such as
- numbers longer than five digits
- identifiers longer than eleven characters
- invalid characters.
- The output format must exactly match the specification.
- Tested on Eustis.
Class: COP 3402 - System Software - Spring 2026
Instructor: Dr. Jie Lin
Due Date: Monday, March 3, 2026
*/

// Structure for enumeration
typedef enum
{
    skipsym = 1,       // Skip / ignore token
    identsym = 2,      // Identifier
    numbersym = 3,     // Number
    beginsym = 4,      // begin
    endsym = 5,        // end
    ifsym = 6,         // if
    fisym = 7,         // fi
    thensym = 8,       // then
    whilesym = 9,      // while
    dosym = 10,        // do
    odsym = 11,        // od
    callsym = 12,      // call
    constsym = 13,     // const
    varsym = 14,       // var
    procsym = 15,      // procedure
    writesym = 16,     // write
    readsym = 17,      // read
    elsesym = 18,      // else
    plussym = 19,      // +
    minussym = 20,     // -
    multsym = 21,      // *
    slashsym = 22,     // /
    eqsym = 23,        // =
    neqsym = 24,       // <>
    lessym = 25,       // <
    leqsym = 26,       // <=
    gtrsym = 27,       // >
    geqsym = 28,       // >=
    lparentsym = 29,   // (
    rparentsym = 30,   // )
    commasym = 31,     // ,
    semicolonsym = 32, // ;
    periodsym = 33,    // .
    becomessym = 34,   // :=
} TokenType;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define norw 15    // num of reserved words
#define imax 99999 // max num of integer value(can only be 5 digits long)
#define idenmax 11 // identifier length
#define strmax 256 // str max length

/*
    Error messages:
    1- Identifier too long
    2- Number too long
    3- Invalid symbols (idk how to check this)


*/

// Check for reserved word or identifier
char *reservedOrIdentifier(char buffer[], int bufferLength, char *reservedWord[], char specialSymbol[], char *nameTable[])
{
    char *str = malloc(bufferLength + 1);
    int length;

    for (length = 0; length < bufferLength; length++)
    {
        str[length] = buffer[length];
    }

    str[length] = '\0';

    for (int j = 0; j < norw; j++)
    {
        if (strcmp(reservedWord[j], str) == 0)
        {
            printf("RESERVED WORD WOW");
            return str;
        }

        if (specialSymbol[j] == str[j])
        {
            // if not in name table
            printf("Identifier!!");
            // store in name table without the special symbol

            return "Success";
        }
    }

    return ""; // "" -> not a reserved word or an identifier
}

// function that maps reserved words
TokenType mapReservedWord(char *str)
{

    if (strcmp(str, "begin") == 0)
    {
        return beginsym;
    }

    if (strcmp(str, "end") == 0)
    {
        return endsym;
    }

    if (strcmp(str, "if") == 0)
    {
        return ifsym;
    }

    if (strcmp(str, "fi") == 0)
    {
        return fisym;
    }

    if (strcmp(str, "then") == 0)
    {
        return thensym;
    }

    if (strcmp(str, "while") == 0)
    {
        return whilesym;
    }

    if (strcmp(str, "do") == 0)
    {
        return dosym;
    }

    if (strcmp(str, "od") == 0)
    {
        return odsym;
    }

    if (strcmp(str, "call") == 0)
    {
        return callsym;
    }

    if (strcmp(str, "const") == 0)
    {
        return constsym;
    }

    if (strcmp(str, "var") == 0)
    {
        return varsym;
    }

    if (strcmp(str, "procedure") == 0)
    {
        return procsym;
    }

    if (strcmp(str, "write") == 0)
    {
        return writesym;
    }

    if (strcmp(str, "read") == 0)
    {
        return readsym;
    }

    if (strcmp(str, "else") == 0)
    {
        return elsesym;
    }

    return 0; // it is not a reserved word
}

// function that maps special symbols

// function that checks for escape sequences

int main(int argc, char *argv[])

{
    char *reservedWord[] = {"null", "begin", "call", "const", "do", "else", "end", "if", "odd", "procedure", "read", "then", "var", "while", "write"};

    char specialSymbol[] = {'+', '-', '*', '/', '(', ')', '=', ',', '.', '#', '<', '>', '$', '%', ';'};

    // char *identifier[] = ""; //to store the identifier name (to check if length exceeds)

    char *bufferLexeme = malloc(strmax + 1);

    // declare another buffer to determine: reserved word, special symbol, letter, digit, and identifier
    //(do lookup to check)

    char *nameTable[] = {""}; // to store the name table

    int *tokenList[] = {0}; // to store all the tokens

    char ch; // to read each char

    if (argc == 2)
    {
        FILE *ip = fopen(argv[1], "r");

        printf("Source Program:\n");

        int i = 0;

        // use buffer to check little by little if it is a reserved word, special character, or an identifier
        while (1)
        {

            ch = fgetc(ip);

            if (ch == EOF)
            {
                break;
            }

            // check if ch is a letter
            if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))
            {

                bufferLexeme[i] = ch;
                bufferLexeme[i + 1] = '\0';

                char *word = reservedOrIdentifier(bufferLexeme, i, reservedWord, specialSymbol, nameTable);
                printf("Building the BUFFER: %s\n", bufferLexeme);
                printf("HERE IS THE WORD: %s ITERATION: %d\n", word, i);

                // pass the string to check for the token number, if string is not empty
                if (strcmp(word, "") != 0)
                {
                    bufferLexeme[0] = '\0';
                    i = 0;
                    // call the function to identify the token number
                    int token = mapReservedWord(word);
                    printf("This is the token: %d\n", token);

                    continue;
                }

                if (strcmp(word, "Success") == 0)
                {
                    // returns identifier (already added to the name table), clear the buffer
                    bufferLexeme[0] = '\0';
                    i = 0;
                }

                else

                {
                    // could be a number, special character, escape sequence, or identifier
                }
            }

            // putchar(ch);
            i++;
        }

        fclose(ip);
    }

    else if (argc > 2)
    {
        printf("Extra arguments were provided.\n");
        printf("Try: ./00_args 123\n");
    }
    else
    {
        printf("No extra arguments provided.\n");
        printf("Try: ./00_args 123\n");
    }
    return 0;
}
