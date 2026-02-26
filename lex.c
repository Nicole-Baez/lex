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

    Falta:
    - el name table
    - printear el lexeme, el name table, y el token list


*/

// Check for reserved word or identifier
char *reservedOrIdentifier(char buffer[], char *reservedWord[], char *nameTable[], int nameTableLength)
{

    int length;
    int found = 0;

    for (int j = 0; j < norw; j++)
    {
        if (strcmp(reservedWord[j], buffer) == 0)
        {

            return buffer;
        }
    }

    return "identifier"; // "" -> not a reserved word or an identifier
}

// function that maps reserved words
TokenType mapReservedWordAndIdentifier(char *str)
{

    if (strcmp(str, "identifier") == 0)
    {

        return identsym;
    }

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
TokenType mapSpecialSym(char *buff)
{
    if (strcmp(buff, "+") == 0)
    {

        return plussym;
    }

    if (strcmp(buff, "-") == 0)
    {
        return minussym;
    }

    if (strcmp(buff, "/") == 0)
    {
        return slashsym;
    }

    if (strcmp(buff, "*") == 0)
    {
        return multsym;
    }

    if (strcmp(buff, "(") == 0)
    {
        return lparentsym;
    }

    if (strcmp(buff, ")") == 0)
    {
        return rparentsym;
    }

    if (strcmp(buff, "=") == 0)
    {
        return eqsym;
    }

    if (strcmp(buff, ",") == 0)
    {
        return commasym;
    }

    if (strcmp(buff, ";") == 0)
    {
        return semicolonsym;
    }

    if (strcmp(buff, ".") == 0)
    {
        return periodsym;
    }

    if (strcmp(buff, "<") == 0)
    {
        return lessym;
    }

    if (strcmp(buff, ">") == 0)
    {
        return gtrsym;
    }

    if (strcmp(buff, "<=") == 0)
    {
        return leqsym;
    }

    if (strcmp(buff, ">=") == 0)
    {
        return geqsym;
    }

    if (strcmp(buff, "<>") == 0)
    {
        return neqsym;
    }

    if (strcmp(buff, ":=") == 0)
    {
        return becomessym;
    }

    // escape sequences
    if (strcmp(buff, " ") == 0 || strcmp(buff, "\n") == 0 || strcmp(buff, "\t") == 0 || strcmp(buff, "\r") == 0)
    {
        return 0;
    }

    return skipsym; // invalid symbol
}

// function that checks for escape sequences

int main(int argc, char *argv[])

{
    char *reservedWord[] = {"null", "begin", "call", "const", "do", "else", "end", "if", "odd", "procedure", "read", "then", "var", "while", "write"};

    char *bufferLexeme = malloc(strmax + 1);

    // declare another buffer to determine: reserved word, special symbol, letter, digit, and identifier
    //(do lookup to check)
    char *lexemes[strmax + 1] = {""};
    char *nameTable[strmax + 1] = {""}; // to store the name table

    int tokenList[strmax + 1] = {0}; // to store all the tokens

    char ch; // to read each char

    if (argc == 2)
    {
        FILE *ip = fopen(argv[1], "r");

        printf("Source Program:\n\n");

        int i = 0;
        int tokenCount = 0;
        int lexLength = 0;
        int nameTableLength = 1;

        // use buffer to check little by little if it is a reserved word, special character, or an identifier
        while (1)
        {

            ch = fgetc(ip);

            if (ch == EOF)
            {
                break;
            }

            putchar(ch);

            // check if ch is a letter
            // problem: if an identifier has numbers it won't get in here
            // put a while loop that checcks for letter and digit, THEN send it to the function
            if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= 0 && ch <= 9))
            {

                bufferLexeme[i] = ch;
                // bufferLexeme[i + 1] = '\0';

                while ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= 0 && ch <= 9))
                {
                    bufferLexeme[i] = ch;

                    ch = fgetc(ip);
                    putchar(ch);
                    i++;
                }

                bufferLexeme[i + 1] = '\0';

                char *word = reservedOrIdentifier(bufferLexeme, reservedWord, nameTable, nameTableLength);

                // pass the string to check for the token number, if string is not empty
                if (strcmp(word, "identifier") == 0 || (strcmp(word, " ") != 0)) //|| strcmp(word, "identifier") == 0
                {

                    // call the function to identify the token number
                    int token = mapReservedWordAndIdentifier(word);

                    tokenList[tokenCount] = token;
                    tokenCount++;

                    lexemes[lexLength] = malloc(strlen(word) + 1);
                    strcpy(lexemes[lexLength], word);
                    lexLength++;

                    // clear the buffer
                    bufferLexeme[0] = '\0';
                    i = 0;
                }
            }

            if (ch >= '0' && ch <= '9')
            {
                i = 0;
                bufferLexeme[i] = ch;

                while (ch >= '0' && ch <= '9')
                {
                    bufferLexeme[i] = ch;
                    ch = fgetc(ip);
                    putchar(ch);
                    i++;
                }

                bufferLexeme[i + 1] = '\0';

                int token = numbersym;
                tokenList[tokenCount] = token;
                tokenCount++;

                lexemes[lexLength] = malloc(strlen(bufferLexeme) + 1);
                strcpy(lexemes[lexLength], bufferLexeme);
                lexLength++;

                bufferLexeme[0] = '\0';
                i = 0;
            }

            if (!((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= 0 && ch <= 9)))
            {

                bufferLexeme[i] = ch;

                if (ch == '<' || ch == '>' || ch == ':')
                {

                    ch = fgetc(ip);
                    if (!((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= 0 && ch <= 9)))
                    {

                        bufferLexeme[i + 1] = ch;
                    }

                    bufferLexeme[i + 2] = '\0';

                    int token = mapSpecialSym(bufferLexeme);

                    if (token != 0)
                    {
                        tokenList[tokenCount] = token;
                        tokenCount++;

                        lexemes[lexLength] = malloc(strlen(bufferLexeme) + 1);
                        strcpy(lexemes[lexLength], bufferLexeme);
                        lexLength++;

                        // clear the buffer
                        bufferLexeme[0] = '\0';
                        i = 0;
                        continue;
                    }
                }

                if (ch == '/')
                {
                    ch = fgetc(ip);
                    putchar(ch);

                    if (ch == '*')
                    {
                        // ignorar comentario

                        while ((ch = fgetc(ip)) != '/')
                        {
                            continue;
                        }
                    }

                    // llamar
                    int token = mapSpecialSym(bufferLexeme);

                    if (token != 0)
                    {

                        tokenList[tokenCount] = token;
                        tokenCount++;

                        lexemes[lexLength] = malloc(strlen(bufferLexeme) + 1);
                        strcpy(lexemes[lexLength], bufferLexeme);
                        lexLength++;

                        // clear the buffer
                        bufferLexeme[0] = '\0';
                        i = 0;
                        continue;
                    }
                }

                else
                {

                    bufferLexeme[i + 1] = '\0';
                    // lama mapsymbol

                    int token = mapSpecialSym(bufferLexeme);

                    if (token != 0)
                    {
                        tokenList[tokenCount] = token;
                        tokenCount++;

                        lexemes[lexLength] = malloc(strlen(bufferLexeme) + 1);
                        strcpy(lexemes[lexLength], bufferLexeme);
                        lexLength++;

                        // clear the buffer
                        bufferLexeme[0] = '\0';
                        i = 0;
                        continue;
                    }
                }
            }
        }

        printf("\n");

        printf("Lexeme Table:\n\n");
        printf("lexeme\t\ttoken type\n");

        for (int i = 0; i < lexLength; i++)
        {

            printf("%s\t%d\n", lexemes[i], tokenList[i]);
        }

        printf("\nToken List:\n\n");

        for (int i = 0; i < tokenCount; i++)
        {

            printf("%d ", tokenList[i]);
        }

        printf("\n");

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
