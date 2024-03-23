#pragma once
#include <stdio.h>
#include "student.h"
//使用前请初始化
FILE *stuFileptr;//登入学生的文件指针
Student *student = NULL; //登入学生
FILE *workerFileptr;//登入职工的文件指针
int isStudent;//学生或职工
int isAdmin;//教师或管理员
int lineNumber;//学生在文件中的位置
char *STU_FILE = "D:/c++/Qt/ScoreManagementSystem/studentInfo.txt";//学生文件位置
char *ADMIN_FILE = "D:/c++/Qt/ScoreManagementSystem/adminInfo.txt"; //教师文件位置
Student *find_result[100];
char allCourse[100][100] = {"666", "555", "444", "666", "666", "666", "666", "666", "666", "666", "666", "666"};
float credit[100] = {
    4.0, // 高等数学的绩点
    3.5, // 线性代数的绩点
    3.7, // 概率论与数理统计的绩点
    3.2, // 大学物理的绩点
    3.6, // 化学基础的绩点
    3.8, // 生物学导论的绩点
    3.4, // 地球科学的绩点
    3.9,
    2.5,
    3.5,
    4.0,
};

void varInit() {
    stuFileptr = NULL;
    workerFileptr = NULL;
    student = NULL;
    isStudent = NULL;
    isAdmin = NULL;
    lineNumber = NULL;
}
