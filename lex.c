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

// This function maps the reserved word
TokenType mapReservedWordAndIdentifier(char *str)
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

// This function maps the special symbols and alsp detects escape sequences
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

    // Returns 0 when either a space or an escape sequence is found
    if (strcmp(buff, " ") == 0 || strcmp(buff, "\n") == 0 || strcmp(buff, "\t") == 0 || strcmp(buff, "\r") == 0)
    {
        return 0;
    }

    return skipsym; // Returns when there is an invalid symbol
}

// Checks whether the buffer contains a reserved word or identifier
TokenType reservedOrIdentifier(char buffer[], int bufferLength, char *reservedWord[], char *nameTable[], int *nameTableLength, int *idenIndex)
{

    // Boolean Flag
    int found = -1;

    // Iterates over the reserved words and checks similarities with buffer
    for (int j = 0; j < norw; j++)
    {
        if (strcmp(reservedWord[j], buffer) == 0)
        {
            *idenIndex = -1;                             // If they match the index identifier remains -1 (no identifier found)
            return mapReservedWordAndIdentifier(buffer); // The token number is returned
        }
    }

    // If it is not a reserved word, this loop checks if the buffer lines up with any identifiers in the name table
    for (int i = 0; i < *nameTableLength; i++)
    {
        // If it is in the name table, break
        if (strcmp(nameTable[i], buffer) == 0)
        {
            found = i; // Found is updated to index i
            break;
        }
    }

    if (found == -1)
    {
        // if it is not in the name table then add it
        nameTable[*nameTableLength] = malloc(bufferLength + 1);

        strcpy(nameTable[*nameTableLength], buffer);
        found = *nameTableLength;
        (*nameTableLength)++;
    }

    *idenIndex = found;

    return identsym;
}

// function that checks for escape sequences

int main(int argc, char *argv[])

{
    // Array for reserved words
    char *reservedWord[] = {"null", "begin", "call", "const", "do", "else", "end", "if", "odd", "procedure", "read", "then", "var", "while", "write"};

    // Buffer used for the lexeme grouping process
    char *bufferLexeme = malloc(strmax + 1);

    // Array to store the lexemes (for printing purposes)
    char *lexemes[strmax + 1] = {""};

    // Array to store the name table
    char *nameTable[strmax + 1] = {""};

    // Array for Error Messages
    char *errorMessages[] = {"Identifier too long", "Number too long", "Invalid symbol"};

    // Array for collecting error messages
    char *errorCollect[] = {""};
    // Array to store the token list
    int tokenList[strmax + 1] = {0}; // to store all the tokens

    // Used to read each char
    char ch;

    // Only allows main to take in two arguments
    if (argc == 2)
    {

        // File pointer is initialized and file is opened in read mode
        FILE *ip = fopen(argv[1], "r");

        printf("Source Program:\n\n");

        int i = 0;               // Counter to keep track of the buffer
        int tokenCount = 0;      // Counter to keep track of the token list
        int lexLength = 0;       // Counter to keep track of the lexemes array
        int nameTableLength = 0; // Counter to keep track of the name table
        int errorMesNum = 0;     // Counter to keep track of the error messages throughout the code

        // While true
        while (1)
        {

            // First character is gathered
            ch = fgetc(ip);

            // Checks for the EOF character
            if (ch == EOF)
            {
                break;
            }

            // Char is printed
            putchar(ch);

            // Checks if char is a letter or a number
            if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= 0 && ch <= 9))
            {

                // If it is, it's added to the buffer
                bufferLexeme[i] = ch;

                /*
                 If the rest of the characters being read are also letters or numbers it can potentially
                 be a reserved word or an identifier
                */
                while ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || (ch >= 0 && ch <= 9))
                {
                    // Chars are added to the buffer until a non letter and non number character is reached
                    bufferLexeme[i] = ch;

                    // Moves along the ip pointer and gathers the character
                    ch = fgetc(ip);

                    // Char is printed
                    putchar(ch);

                    // Buffer tracker is updated
                    i++;
                }

                // Null terminator is added at the end
                bufferLexeme[i + 1] = '\0';

                // Identifier index variable is declared (it will be sent as a pointer so that it can be updated without it needing to be returned)
                int identifierIndex = -1;

                // Function is called to detect a reserved word or an identifier
                int token = reservedOrIdentifier(bufferLexeme, i, reservedWord, nameTable, &nameTableLength, &identifierIndex);

                if (token == identsym)
                {
                    if (strlen(bufferLexeme) > 11)
                    {
                        // If identifier exceeds 11 characters, skipsym is stored in the token list
                        tokenList[tokenCount] = skipsym;
                        tokenCount++;
                    }

                    else
                    {
                        // Token is added to the token list
                        tokenList[tokenCount] = skipsym;
                        tokenCount++;

                        // If identifierIndex is not -1, this means an identifier was found and added to the name table
                        if (identifierIndex != -1)
                        {
                            // Name table index is added to the token list
                            tokenList[tokenCount] = identifierIndex;
                            tokenCount++; // Token list tracker is updated
                        }
                    }
                }

                else
                {
                    // Token (reserved word) is added to the token list
                    tokenList[tokenCount] = skipsym;
                    tokenCount++;
                }

                // Memory is allocated for an element in the lexeme array that is the same length as the buffer length
                lexemes[lexLength] = malloc(strlen(bufferLexeme) + 1);

                // Buffer is copied to the lexeme array
                strcpy(lexemes[lexLength], bufferLexeme);
                lexLength++; // Lexeme array tracker is updated

                // Buffer is cleared
                bufferLexeme[0] = '\0';
                i = 0;
            }

            // If the character is strictly numbers
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

                if (strlen(bufferLexeme) > 5)
                {
                    tokenList[tokenCount] = skipsym;
                    tokenCount++;
                }

                else
                {
                    tokenList[tokenCount] = token;
                    tokenCount++;

                    tokenList[tokenCount] = atoi(bufferLexeme);
                    tokenCount++;

                    lexemes[lexLength] = malloc(strlen(bufferLexeme) + 1);
                    strcpy(lexemes[lexLength], bufferLexeme);
                    lexLength++;
                }

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

        // Print portion
        printf("\n");

        printf("Lexeme Table:\n\n");
        printf("lexeme\t\ttoken type\n");

        int token = 0; // to keep track of the tokens (it has to skip the identifier index)

        for (int i = 0; i < lexLength; i++)
        {

            if (tokenList[token] != skipsym)
            {
                printf("%s\t\t%d\n", lexemes[i], tokenList[token]);
            }

            else
            {
            }

            if (tokenList[token] == identsym || tokenList[token] == numbersym)
            {
                token += 2;
            }

            else
            {
                token++;
            }
        }

        printf("\nName Table:\n\n");
        printf("Index Name\n");
        for (int i = 0; i < nameTableLength; i++)
        {
            printf("%d\t%s\n", i, nameTable[i]);
        }

        printf("\nToken List:\n\n");

        for (int i = 0; i < tokenCount; i++)
        {
            if (tokenList[token] == identsym || tokenList[token] == numbersym)
            {
                printf("%d %d ", tokenList[i], tokenList[i + 1]);
                i++;
            }

            else
            {
                printf("%d ", tokenList[i]);
            }
        }

        printf("\n");

        fclose(ip);
    }

    else
    {
        printf("Wrong number of arguments.\n");
        printf("Try: ./00_args 123\n");
    }

    return 0;
}
