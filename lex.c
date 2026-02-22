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










//structure for enumeration
typedef enum {
    skipsym = 1 , // Skip / ignore token
    identsym = 2, // Identifier
    numbersym = 3, // Number
    beginsym = 4, // begin
    endsym = 5, // end
    ifsym = 6, //if
    fisym = 7, // fi
    thensym = 8, // then
    whilesym = 9, // while
    dosym = 10, // do
    odsym = 11, // od
    callsym = 12, // call
    constsym = 13, // const
    varsym = 14, // var
    procsym = 15, // procedure
    writesym = 16, // write
    readsym = 17, // read
    elsesym = 18, // else
    plussym = 19, // +
    minussym = 20, // -
    multsym = 21, // *
    slashsym = 22, // /
    eqsym = 23, // =
    neqsym = 24, // <>
    lessym = 25, // <
    leqsym = 26, // <=
    gtrsym = 27, // >
    geqsym = 28, // >=
    lparentsym = 29, // (
    rparentsym = 30, // )
    commasym = 31, // ,
    semicolonsym = 32, // ;
    periodsym = 33, // .
    becomessym = 34, // :=
} TokenType;






#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define norw 15 //num of reserved words
#define imax 99999 //max num of integer value(can only be 5 digits long)
#define cmax 11 //identifier length
#define strmax 256 //str max length

/*
    Error messages:
    1- Identifier too long
    2- Number too long
    3- Invalid symbols (idk how to check this)


*/

//function that maps reserved words

//function that maps special symbols


int main(int argc, char *argv[])

{
    char *reservedWord[] = {"null", "begin", "call", "const", "do", "else", "end", "if", "odd", "procedure", "read", "then",
    "var", "while", "write"};

    
    
    //char *identifier[] = ""; //to store the identifier name (to check if length exceeds)

    char *groupLexemes[] = {""};


    char *nameTable[] = {""}; //to store the name table

    int *tokenList[] = {0}; // to store all the tokens 

    char ch = ""; //to read each char

    
    if (argc == 2)
    {
        FILE *ip = fopen(argv[1], "r");

        printf("Source Program:\n");
        int i = 0;
        int commentCheck = 0;
        
        while (ch = fgetc(ip) != EOF)
        {
            
            //ignores comments
            if (ch == '/*'){
                commentCheck = 1;
                continue;
            }

            if(commentCheck)
            {
                if(ch == '*/'){
                    commentCheck = 0;
                }

                continue;
            }


            /*For printing purposes. It prints the source code just as is. */
             if (ch == ' '){
                printf(" ");
             }
             
             if (ch == '\t'){
                printf("\t");
             }

             if (ch == '\n'){
                printf("\n");
             }

             if (ch == '\r'){
                printf("\r");
             }

             
            printf("%c", ch);

            if (ch != ' ' || ch != '\t' || ch != '\n' || ch != '\r')
            {
                groupLexemes[i] = ch;
            }

        }


        fclose(ip);



    }

    else if(argc > 2){
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
