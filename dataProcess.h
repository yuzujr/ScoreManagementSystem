#ifndef DATAPROCESS_H
#define DATAPROCESS_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <malloc.h>
#include "student.h"
#include "globalVar.h"

// 修改文件中的某一行
int modify_file(const char *filename, const char *new_content);
//修改密码
int changePasswdTo(char *newPasswd);
//文件指针移至行首
void moveToLineStart(FILE *pf);
//读入登录的学生数据，并返回他的指针
Student *loadSingleStudent();
//更新单科绩点
double updateCredit(double grade);
//查找学院索引
int findCollegeIndex(const char *college);
//查找专业索引
int findMajorIndex(int collegeIndex, const char *major);

#endif // DATAPROCESS_H
