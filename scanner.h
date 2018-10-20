#ifndef SCANNER_H
#define SCANNER_H
/* VERSION 1.2
 *
 * scanner.h - public interface to scanner.c, the scanner module
 *
 */
extern char *readString(FILE *);
extern char *readPhrase(FILE *);
extern char *readWord(FILE *);
extern int stringPending(FILE *);
extern void *allocate(size_t);
extern void *reallocate(void *,size_t);
#endif
