#ifndef DATAPROCESS_H
#define DATAPROCESS_H

#include <stdio.h>
#include <string.h>

extern int lineNumber;
extern FILE *stuFileptr;

int modify_file(const char *filename, const char *new_content);
int changePasswdTo(char *newPasswd);
void moveToLineStart(FILE *pf);

#endif // DATAPROCESS_H
