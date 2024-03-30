#pragma once
#include <stdio.h>
#include "student.h"

#define MAX_COLLEGE_COUNT 100//最多学院数量
#define MAX_MAJOR_PER_COLLEGE 10//各学院最多专业数量
#define MAX_NAME_LENGTH 100//学院、专业、课程名最大长度
#define MAX_COURSES_PER_MAJOR 100//各专业最大课程数量

//使用前请初始化
extern FILE *stuFileptr;//登入学生的文件指针

extern Student *student; //登入学生

extern FILE *workerFileptr;//登入职工的文件指针

extern int isStudent;//学生或职工

extern int isAdmin;//教师或管理员

extern int lineNumber;//学生在文件中的位置

extern char *STU_FILE;//学生文件位置

extern char *ADMIN_FILE; //教师文件位置

extern Student *find_result[100];//查找结果

extern const char allCourses[100][100];//所有课程

extern float credits[100];//各课程绩点

extern char colleges[MAX_COLLEGE_COUNT][MAX_NAME_LENGTH];//所有学院

extern char majors[MAX_COLLEGE_COUNT][MAX_MAJOR_PER_COLLEGE][MAX_NAME_LENGTH];//各学院对应专业

extern char majorCourses[MAX_COLLEGE_COUNT][MAX_MAJOR_PER_COLLEGE][MAX_COURSES_PER_MAJOR];//各专业对应课程
