# AI Usage Disclosure Details

**Student Name:** Nicole Baez Espinosa
**Student ID:** 5544379
**Assignment:** Homework 2

---

## Instructions

Complete this template with detailed information about your AI usage. Submit this
file along with your signed PDF declaration form. Include the conversation log in
order, with each round listing the prompt, AI output, and verification/edits.

---

## AI Tool #1

### Tool Name

ChatGPT

### Version/Model

5.2

### Date(s) Used

February 24th, 2026

### Specific Parts of Assignment

I used AI to help me debug the buffer I used for grouping Lexemes. I asked ChatGPT
to help me with C syntax.

### Conversation Log

[Use one block per round. Add more rounds as needed.]

#### Round 1

**Prompt:**
char *reservedOrIdentifier(char *buffer, int bufferLength, char *reservedWord[])
{
char *str;
int length;

    for (length = 0; length < bufferLength; length++)
    {
        str[length] = buffer[length];
    }

    for (int j = 0; j < norw; j++)
    {
        if (strcmp(reservedWord[j], str) == 0)
        {
            printf("RESERVED WORD WOW");
            return str;
        }
    }

    return "";

} (This function is part of an assignment where I'm building a Lexical Analyzer. I am using a buffer to check little by little if the characters in the buffer are either a reserved word or an identifier. Before I call this function, I add characters to the buffer and try to print it like this bufferLexeme[i] = ch; printf("%c", bufferLexeme[i]); Why is nothing showing up?)
**AI Output:** GPT said the issues could be due to not initializing the str pointer by
not allocating dynamic memory. It also said to include null terminators for the buffer
and for the str.
**Verification/Edits:** I made those changes but still nothing was printing. So I decided to look at
the if statement that calls the function and thought that maybe there was something wrong
with the condition itself.

#### Round 2

**Prompt:** I want my buffer to reset after a reserved word has been found, how could I do that? I rather not use memset.
**AI Output:**
You can just set the first character to the null terminator:
cbuffer[0] = '\0';
bufferIndex = 0;
This effectively "clears" the buffer since all string functions in C treat the string as ending at the first \0. The old data is still technically in memory but nothing will read past that first character.
**Verification/Edits:** I added this once my code checks that a lexeme is not a reserved word or an identifier.

#### Round 3 (optional)

**Prompt:** lex.c:210:30: error: incompatible integer to pointer conversion initializing 'char *' with an expression of type 'int' [-Wint-conversion] 210 | char *specialSymbol[] = {'+', '-', '*', '/', '(', ')', '=', ',', '.', '#', '<', '>', '$', '%', ';'}; What does this error mean
**AI Output:** ChatGPT said the error was due to declaring the arrat as an array of pointers. It suggested I either
change every element to be string literals or get rid of the * in the array.
**Verification/Edits:** I removed the * from the array declaration and I still got other errors. I will ask
about those errors. 

#### Round 4 (optional)

**Prompt:** if (strcmp(specialSymbol[j], str[j]) == 0) { // if not in name table printf("Identifier!!"); identifier = 1; // store in name table return "Success"; } 
(This condition is giving me trouble, specialSymbol is declared as an array of characters)
**AI Output:** ChatGPT said I was using strcmp incorrectly since I'm evaluating two characters instead of two strings. 
**Verification/Edits:** I removed the strcmp and instead did the comparison using ==. It worked and the compilation
errors went away. 

### Learning & Reflection

[Reflect on what you learned and how the AI assistance contributed to your
understanding]
[e.g., I learned about proper error handling techniques and improved my debugging
skills]
[e.g., Understanding of cryptographic concepts was enhanced through AI
explanations]

---

## AI Tool #2 (if applicable)

### Tool Name

Claude

### Version/Model

Sonnet 4.6

### Date(s) Used

February 24th, 2026

### Specific Parts of Assignment

I used Claude to debug compilation error messages. I also used Claude for help on storing and printing
identifier indexes from the name table.

### Conversation Log

[Use one block per round. Add more rounds as needed.]

#### Round 1

**Prompt:**
lex.c:98:20: warning: incompatible pointer types passing 'char \*_' to parameter of type 'const char _'; remove & [-Wincompatible-pointer-types]

98 | if (strcmp(&reservedWord[j], str) == 0)
(What does this error mean? It's part of a function that is meant to tell a reserved word apart from an identifier in a lexical analyzer in C.)
**AI Output:**
The issue is that reservedWord is likely already an array of strings (i.e., char _reservedWord[] or char reservedWord[][...]), so each element reservedWord[j] is already a char _. By writing &reservedWord[j], you're taking the address of that pointer, giving you a char ** — one level of indirection too many.
strcmp expects two const char _ arguments, so just remove the &:
cif (strcmp(reservedWord[j], str) == 0)
That gives strcmp the actual string (a char _) rather than a pointer to it.
**Verification/Edits:** I will now remove the & symbol, which I only added because the
terminal error said to do so earlier. After removing the symbol the code worked and now the buffer is visible.
I had to use DMA for both the buffer and the string.

#### Round 2

**Prompt:** 

This condition in my code is not correctly adding the lexeme into my lexeme array. 

if (strcmp(word, "identifier") == 0 || (strcmp(word, " ") != 0)) //|| strcmp(word, "identifier") == 0
                {

                    // call the function to identify the token number
                    int token = mapReservedWordAndIdentifier(word);

                    tokenList[tokenCount] = token;
                    tokenCount++;

                    lexemes[lexLength] = word;
                    lexLength++;

                    // clear the buffer
                    bufferLexeme[0] = '\0';
                    i = 0;
                }
**AI Output:** 

The condition on your if statement is the problem. Let's break it down:
cstrcmp(word, "identifier") == 0 || (strcmp(word, " ") != 0)
The second part of the OR — strcmp(word, " ") != 0 — is true for almost every string that isn't a single space. This means the entire condition is nearly always true, so it runs even when it shouldn't.
It looks like your intent is to add the lexeme only when the word is an identifier or when it's not whitespace/empty. The fix depends on your exact logic, but most likely you want AND instead of OR for part of the condition, or you want to check for an empty/whitespace string differently:
c// Option 1: Only add if word is not a space and not empty
if (strcmp(word, " ") != 0 && strcmp(word, "") != 0)

// Option 2: Only add if word is "identifier" OR word is a non-space token
if (strcmp(word, "identifier") == 0 || strcmp(word, " ") != 0 && strlen(word) > 0)

// Option 3: Most common fix — check it's not whitespace/empty at all
if (strlen(word) > 0 && strcmp(word, " ") != 0)
A few other things worth checking in the broader code:
String storage issue — This is likely a bigger bug. If word is a pointer to a buffer (like bufferLexeme), then doing lexemes[lexLength] = word just copies the pointer, not the string. Since you clear the buffer afterward with bufferLexeme[0] = '\0', all your lexeme entries will point to the same (now-empty) memory. Fix it with strdup:
clexemes[lexLength] = strdup(word);
Or if lexemes is a 2D char array:
cstrcpy(lexemes[lexLength], word);
Index reset — You reset i = 0 but if i is your buffer index, make sure that's consistent with how you're building bufferLexeme elsewhere.


**Verification/Edits:** I tried doing strcpy but it was giving me an error. Then I followed up with Claude and
I realized that not allocating memory for each pointer in the lexeme char array could be one of the issues. 
Once I allocated memory the lexemes started appearing in the terminal. 

#### Round 3 (optional)

**Prompt:** 
    int length;
    int found = 0;

    for (int j = 0; j < norw; j++)
    {
        if (strcmp(reservedWord[j], buffer) == 0)
        {

            return buffer;
        }
    }

    for (int i = 0; i < nameTableLength; i++)
    {
        // if it is in the name table, break
        if (strcmp(nameTable[nameTableLength], buffer) == 0)
        {
            found = 1;
            break;
        }
    }

    if (!found)
    {
        // if it is not in the name table then add it
        nameTable[nameTableLength] = malloc(bufferLength + 1);
        nameTable[nameTableLength] = buffer;

        strcpy(nameTable[nameTableLength], buffer);
        nameTableLength++;
    }

    return buffer; 

For this function, I add the identifier to the name table and I return the buffer, when I receive it in main this is how I have it set up 

if ((strcmp(word, " ") != 0)) //|| strcmp(word, "identifier") == 0
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

I need to distinguish the identifier from the reserved word and print the specific index of the identifier that's stored in the name table, how can I modify main to make this distinction possible?
**AI Output:** [Summarize or paste the output]
**Verification/Edits:** [How you verified accuracy or modified the output]


#### Round 4 (optional)

**Prompt:**
int length;
    int found = 0;

    for (int j = 0; j < norw; j++)
    {
        if (strcmp(reservedWord[j], buffer) == 0)
        {

            return mapReservedWordAndIdentifier(buffer);
        }
    }

    for (int i = 0; i < nameTableLength; i++)
    {
        // if it is in the name table, break
        if (strcmp(nameTable[nameTableLength], buffer) == 0)
        {
            found = i;
            break;
        }
    }

    if (!found)
    {
        // if it is not in the name table then add it
        nameTable[nameTableLength] = malloc(bufferLength + 1);
        nameTable[nameTableLength] = buffer;

        strcpy(nameTable[nameTableLength], buffer);
        nameTableLength++;
    }

    idenIndex = &found;

    return identsym;
This is how I modified the function. 

 int identifierIndex = 0;

                int token = reservedOrIdentifier(bufferLexeme, i, reservedWord, nameTable, nameTableLength, &identifierIndex);

                tokenList[tokenCount] = token;
                tokenList[tokenCount++] = identifierIndex;

                tokenCount++;

                lexemes[lexLength] = malloc(strlen(bufferLexeme) + 1);
                strcpy(lexemes[lexLength], bufferLexeme);
                lexLength++;

                // clear the buffer
                bufferLexeme[0] = '\0';
                i = 0;
This is how it's received in main.

for (int i = 0; i < lexLength; i++)
        {

            printf("%s\t%d\n", lexemes[i], tokenList[i]);
        }

        printf("\nToken List:\n\n");

        for (int i = 0; i < tokenCount; i++)
        {

            printf("%d ", tokenList[i]);
        }
This is how I'm printing. lexeme          token type
var     0
x       0
,       0
y       0
;       31
begin   0
y       0
:=      32
3       0
;       0
x       0
:=      0
y       34
-       3
56      32
;       0
end     0
.       34 
(And this is the output I'm getting, where are the zeros coming from and how do I fix it?)
**AI Output:** 
10:11 AMYou have several bugs compounding each other. Let me walk through them:
Bug 1: Recursive call instead of token lookup
c// Wrong - this calls itself recursively forever
return mapReservedWordAndIdentifier(buffer);

// Should return the token number for that reserved word
return reservedWordTokens[j]; // or however you map words to token numbers
Bug 2: Overwriting tokenList slot and double-incrementing tokenCount
ctokenList[tokenCount] = token;
tokenList[tokenCount++] = identifierIndex; // overwrites the token you just stored!
tokenCount++;                               // double increment
This should be:
ctokenList[tokenCount] = token;
identifierIndices[tokenCount] = identifierIndex;
tokenCount++;
Bug 3: Same index bug from before in your loop
cif (strcmp(nameTable[nameTableLength], buffer) == 0) // always checks out-of-bounds slot
// should be
if (strcmp(nameTable[i], buffer) == 0)
Bug 4: found initialized to 0, so index 0 looks like "not found"
cint found = 0;   // 0 is a valid name table index!
// ...
if (!found)      // triggers even when found at index 0
Change it to:
cint found = -1;
// ...
if (found == -1)
Bug 5: idenIndex = &found stores address of a local variable
found dies when the function returns, so that pointer becomes dangling. Use the pointer parameter instead:
c*identifierIndex = found; // write through the pointer passed in from main
With all fixes applied your function should look like:
cint found = -1;

for (int j = 0; j < norw; j++)
{
    if (strcmp(reservedWord[j], buffer) == 0)
    {
        *identifierIndex = -1;
        return reservedWordTokens[j];
    }
}

for (int i = 0; i < nameTableLength; i++)
{
    if (strcmp(nameTable[i], buffer) == 0)
    {
        found = i;
        break;
    }
}

if (found == -1)
{
    nameTable[nameTableLength] = malloc(bufferLength + 1);
    strcpy(nameTable[nameTableLength], buffer);
    found = nameTableLength;
    nameTableLength++;
}

*identifierIndex = found;
return identsym;
And in main:
cint identifierIndex = -1;
int token = reservedOrIdentifier(bufferLexeme, i, reservedWord, nameTable, nameTableLength, &identifierIndex);

tokenList[tokenCount] = token;
identifierIndices[tokenCount] = identifierIndex;
tokenCount++;i want to identifier index to the token list though10:16 AMThen just append it as a second entry right after the token:
cint identifierIndex = -1;
int token = reservedOrIdentifier(bufferLexeme, i, reservedWord, nameTable, nameTableLength, &identifierIndex);

tokenList[tokenCount++] = token;

if (identifierIndex != -1)
{
    tokenList[tokenCount++] = identifierIndex;
}
This way reserved words only add one entry to the token list, and identifiers add two (the token code followed by its name table index). Then when printing you need to account for that:
cfor (int i = 0; i < tokenCount; i++)
{
    if (tokenList[i] == identsym)
    {
        printf("%d %d ", tokenList[i], tokenList[i + 1]);
        i++; // skip the index entry since we already printed it
    }
    else
    {
        printf("%d ", tokenList[i]);
    }
}
**Verification/Edits:** I fixed the way I was adding the identifier to the tokenList. Instead of using an 
array to store the identifier index, I decided to store it in the token list since I will have to print the 
indexes in the order they're added to the tokenList. 

#### Round 5 (optional)

**Prompt:** I forgot to update found to nameTable, now I'm getting this output. It seems that the way I'm printing the token type is truncating my special symbols. Source Program:
var x , y ;
begin
y : 3;
x : y - 56;
end .
Lexeme Table:
lexeme          token type
var             14
x               0
,               2
y               2
;               31
begin           2
y               2
:=              32
3               4
;               0
x               2
:=              2
y               34
-               3
56              32
;               2
end             2
.               34
Token List:
14 0 2 2 31 2 2 32 4 0 2 2 34 3 32 2 2 34 2 2 20 3 32 5 0 33
**AI Output:** The token type column is showing the identifier index instead of the actual token code for identifiers. Your lexeme table print loop uses tokenList[i] for every entry, but since identifiers now store two entries (token code + index), the index is what lines up with some lexemes.
You need to track a separate index for walking the token list while printing the lexeme table:
cint t = 0; // separate token list walker
for (int i = 0; i < lexLength; i++)
{
    printf("%s\t\t%d\n", lexemes[i], tokenList[t]);
    
    if (tokenList[t] == identsym)
    {
        t += 2; // skip both the token code and the name table index
    }
    else
    {
        t++;
    }
}
And your token list print should already be correct since it just dumps every entry. But if you want the token list to look cleaner and show the index only for identifiers, it should be doing the same thing:
cfor (int i = 0; i < tokenCount; i++)
{
    if (tokenList[i] == identsym)
    {
        printf("%d %d ", tokenList[i], tokenList[i + 1]);
        i++;
    }
    else
    {
        printf("%d ", tokenList[i]);
    }
}
The root issue is that your lexeme array and token list are no longer 1-to-1 once identifiers take two slots, so you can't use the same index i for both.
**Verification/Edits:** I implemented an additional tracker for the tokenList to match with the lexeme array. This
approach worked though I was still having issues with the printing due to logical errors in my reservedOrIdentifier
function. 



### Learning & Reflection

## [What you learned from using this tool]

## AI Tool #3 (if applicable)

### Tool Name

[Tool name if you used a third AI tool]

### Version/Model

[Version/model information]

### Date(s) Used

[Dates when this tool was used]

### Specific Parts of Assignment

[Which parts of the assignment involved this tool]

### Conversation Log

[Use one block per round. Add more rounds as needed.]

#### Round 1

**Prompt:** [Exact prompt/question]
**AI Output:** [Summarize or paste the output]
**Verification/Edits:** [How you verified accuracy or modified the output]

#### Round 2

**Prompt:** [Exact prompt/question]
**AI Output:** [Summarize or paste the output]
**Verification/Edits:** [How you verified accuracy or modified the output]

#### Round 3 (optional)

**Prompt:** [Exact prompt/question]
**AI Output:** [Summarize or paste the output]
**Verification/Edits:** [How you verified accuracy or modified the output]

### Learning & Reflection

## [What you learned from using this tool]

## Overall Reflection

[Provide an overall reflection on your AI usage for this assignment]
[Consider: How did AI tools help your learning? What did you understand better? How
did you ensure the work remained your own?]

---

## Notes

- Be as specific and detailed as possible in your responses
- Keep the conversation rounds in order
- Include exact prompts and AI outputs when possible
- Explain how you verified and modified AI-generated content
- Reflect on what you learned through the AI interaction
