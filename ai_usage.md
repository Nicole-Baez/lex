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

I used Claude to debug compilation error messages.

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
**Verification/Edits:\*\* I will now remove the & symbol, which I only added because the
terminal error said to do so earlier. After removing the symbol the code worked and now the buffer is visible.
I had to use DMA for both the buffer and the string.

#### Round 2

**Prompt:** I want my buffer to reset after a reserved word has been found, how could I do that?
**AI Output:** [Summarize or paste the output]
**Verification/Edits:** [How you verified accuracy or modified the output]

#### Round 3 (optional)

**Prompt:** [Exact prompt/question]
**AI Output:** [Summarize or paste the output]
**Verification/Edits:** [How you verified accuracy or modified the output]

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
