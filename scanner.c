#include <stdio.h>
#include <stdlib.h>  /* for exit, malloc, realloc */
#include <ctype.h>   /* for isspace */
#include <string.h>  /* for strdup */
#include "scanner.h"

static void *allocateMsg(size_t size,char *where);
static void *reallocateMsg(void *s,size_t size,char *where);

/* VERSION 1.3
 *
 * scanner.c - a collection of input routines for C
 *           - written by John C. Lusth
 *           - in general, the functions return what is implied by their names
 *
 *    readInt(FILE *)
 *      - wrapper for fscanf(fp,"%d")
 *      - returns 0 if end of file, but feof will subsequently return true
 *      - usage example: int x = readInt(stdin);
 *    readReal(FILE *)
 *      - wrapper for fscanf(fp,"%lf")
 *      - returns a double
 *      - returns 0.0 if read fails due to end of file,
 *        but feof will subsequently return true
 *      - usage example: double x = readReal(stdin);
 *    readChar(FILE *)
 *      - wrapper for fscanf(fp," %c")
 *      - returns a non-whitespace character
 *      - returns EOF if end of file; feof will subsequently return true
 *      - usage example: char x = readChar(stdin);
 *    readRawChar(FILE *fp)
 *      - wrapper for fscanf(fp,"%c")
 *      - returns a character, whether whitespace or non-whitespace
 *      - returns EOF if end of file; feof will subsequently return true
 *      - usage example: char x = readRawChar(stdin);
 *    readToken(FILE *fp)
 *      - safe version of fscanf(fp,"%s")
 *      - returns a malloc'd string
 *        the caller should free the returned string
 *      - returns 0 if end of file; feof will subsequently return true
 *      - usage example: char *x = readToken(stdin);
 *    readString(FILE *fp)
 *      - reads in a double quoted string
 *      - returns a malloc'd string; the quotation marks are not included
 *        the caller should free the returned string
 *      - returns 0 if end of file; feof will subsequently return true
 *      - usage example: char *x = readString(stdin);
 *    readLine(FILE *fp)
 *      - reads in a line or remainder of a line
 *      - returns a malloc'd string; the newline is not included
 *        the caller should free the returned string
 *      - returns 0 if end of file; feof will subsequently return true
 *      - usage example: char *x = readLine(stdin);
 *    stringPending(FILE *fp)
 *      - returns true (non-zero) if the next non-whitespace character
 *        is a double quote
 *      - it consumes all the whitespace up to that non-whitespace character
 */

static void skipWhiteSpace(FILE *);

/********** public functions **********************/

extern char *readString(FILE *fp)
{
    char *str;
    if (stringPending(fp))
        str = readPhrase(fp);
    else
        str = readWord(fp);
    return str;
}

extern char *
readPhrase(FILE *fp)
    {
    int ch,index;
    char *buffer;
    int size = 512;

    /* advance to the double quote */

    skipWhiteSpace(fp);

    if (feof(fp)) return 0;

    ch = fgetc(fp);

    if (ch == EOF) return 0;

    /* allocate the buffer */

    buffer = allocateMsg(size,"readString");

    if (ch != '\"')
        {
        fprintf(stderr,"SCAN ERROR: attempt to read a string failed\n");
        fprintf(stderr,"first character was <%c>\n",ch);
        exit(4);
        }

    /* toss the double quote, skip to the next character */

    ch = fgetc(fp);

    /* initialize the buffer index */

    index = 0;

    /* collect characters until the closing double quote */

    while (ch != '\"')
        {
        if (ch == EOF)
            {
            fprintf(stderr,"SCAN ERROR: attempt to read a string failed\n");
            fprintf(stderr,"no closing double quote\n");
            exit(6);
            }
        if (index > size - 2)
            {
            ++size;
            buffer = reallocateMsg(buffer,size,"readString");
            }

        if (ch == '\\')
            {
            //skip escaped character
            ch = fgetc(fp);

            if (ch == EOF)
                {
                fprintf(stderr,"SCAN ERROR: attempt to read a string failed\n");
                fprintf(stderr,"escaped character missing\n");
                exit(6);
                }
            }
        else if (isalpha(ch))
        {
            buffer[index++] = tolower(ch);
        }
        else if (isspace(ch))
        {
            skipWhiteSpace(fp);
            if (index == 0 || (index > 0 && buffer[index - 1] != ' ')) //prolly clean up
            {
                buffer[index++] = ' ';
            }
        }
        ch = fgetc(fp);
        }

    buffer[index] = '\0';

    return buffer;
    }

extern char *
readWord(FILE *fp)
    {
    int ch,index;
    char *buffer;
    int size = 80;

    skipWhiteSpace(fp);

    ch = fgetc(fp);
    if (ch == EOF) return 0;

    buffer = allocateMsg(size,"readToken");

    index = 0;
    while (!isspace(ch))
        {
        if (ch == EOF) break;
        if (index > size - 2)
            {
            ++size;
            buffer = reallocateMsg(buffer,size,"readToken");
            }
        if (isalpha(ch))
        {
            buffer[index++] = tolower(ch);
        }
        ch = fgetc(fp);
        }

    /* push back the character that got us out of this loop */

    ungetc(ch,fp);

    if (index > 0)              //there is something in the buffer
        clearerr(fp);           //so force the read to be good

    buffer[index] = '\0';

    return buffer;
    }

extern int
stringPending(FILE *fp)
    {
    int ch,result = 0;
    skipWhiteSpace(fp);
    ch = fgetc(fp);
    if (ch == EOF) return 0;
    if (ch == '\"') result = 1;
    ungetc(ch,fp);
    return result;
    }

/********** private functions **********************/

static void
skipWhiteSpace(FILE *fp)
    {
    int ch;

    /* read chars until a non-whitespace character is encountered */

    while ((ch = fgetc(fp)) != EOF && isspace(ch))
        continue;

    /* a non-space character got us out of the loop, so push it back */

    if (ch != EOF) ungetc(ch,fp);
    }

void *
allocateMsg(size_t size,char *where)
    {
    char *s;
    s = malloc(size);
    if (s == 0)
        {
        fprintf(stderr,"%s: could not allocate string, out of memory\n",
            where);
        exit(3);
        }

    return s;
    }

static void *
reallocateMsg(void *s,size_t size,char *where)
    {
    char *t;
    t = realloc(s,size);
    if (t == 0)
        {
        fprintf(stderr,"%s: could not reallocate string, out of memory\n",
            where);
        exit(3);
        }

    return t;
    }
