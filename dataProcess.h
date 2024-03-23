#ifndef DATAPROCESS_H
#define DATAPROCESS_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <malloc.h>
#include "student.h"

extern int isStudent;
extern int lineNumber;
extern FILE *stuFileptr;
extern FILE *workerFileptr;
extern char *STU_FILE;
extern char *ADMIN_FILE;
extern Student *student;

// 修改文件中的某一行
int modify_file(const char *filename, const char *new_content);
//修改密码
int changePasswdTo(char *newPasswd);
//文件指针移至行首
void moveToLineStart(FILE *pf);
//读入登录的学生数据，并返回他的指针
Student *loadSingleStudent();

#endif // DATAPROCESS_H
