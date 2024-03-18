#ifndef GLOBALVAR_H
#define GLOBALVAR_H
#include <stdio.h>
//使用前请初始化
FILE *stuFileptr;//登入学生的文件指针
FILE *workerFileptr;//登入职工的文件指针
int isStudent;//学生或职工
int isAdmin;//教师或管理员
int lineNumber;//学生在文件中的位置
char *STU_FILE_NAME = "D:/c++/Qt/ScoreManagementSystem/studentInfo.txt";

void varInit() {
    stuFileptr = NULL;
    lineNumber = NULL;
}

#endif // GLOBALVAR_H
