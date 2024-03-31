#include "globalVar.h"


FILE *stuFileptr=NULL;//登入学生的文件指针

Student *student=NULL; //登入学生

FILE *workerFileptr=NULL;//登入职工的文件指针

int isStudent=0;//学生或职工

int isAdmin=0;//教师或管理员

int lineNumber=0;//学生在文件中的位置

Student *find_result[100]={NULL};//查找结果

char *STU_FILE = "D:/c++/Qt/ScoreManagementSystem/studentInfo.txt";//学生文件位置

char *ADMIN_FILE = "D:/c++/Qt/ScoreManagementSystem/adminInfo.txt"; //教师文件位置

const char allCourses[100][100] = {
    "计算机科学导论",
    "数据结构与算法",
    "操作系统",
    "数据库系统",
    "计算机网络",
    "编译原理",
    "人工智能",
    "机器学习",
    "软件工程",
    "网络安全"
};

const float credits[100] = {
    4.0,
    3.5,
    3.7,
    3.2,
    3.6,
    3.8,
    3.4,
    3.9,
    2.5,
    3.5,
    4.0,
    3.1
};

const char colleges[MAX_COLLEGE_COUNT][MAX_NAME_LENGTH] = {
    "计算机科学与技术学院",
    "电子工程学院",
    "化学与化工学院"
    // 其他学院名称可以继续添加
};

const char majors[MAX_COLLEGE_COUNT][MAX_MAJOR_PER_COLLEGE][MAX_NAME_LENGTH] = {
    // 计算机学院的专业
    {
        "计算机科学与技术",
        "软件工程",
        "物联网工程"
        // 其他专业名称可以继续添加
    },
    // 电子工程学院的专业
    {
        "电子信息工程",
        "通信工程"
        // 其他专业名称可以继续添加
    },
    // 化学与化工学院的专业
    {
        "化学",
        "化学工程"
        // 其他专业名称可以继续添加
    }
    // 其他学院的专业信息可以继续添加
};

const int majorCourses[MAX_COLLEGE_COUNT][MAX_MAJOR_PER_COLLEGE][MAX_COURSES_PER_MAJOR] = {
    // 计算机学院
    {
        // 计算机科学与技术专业
        {1, 2,3},
        // 软件工程专业
        {4, 5,6,7,8,9},
        // 物联网工程专业
        {6, 7, 8}
    },
    // 电子工程学院
    {
        // 电子信息工程专业
        {9,1},
        // 通信工程专业
        {1,2}
    },
    // 化学与化工学院
    {
        // 化学专业
        {1,2,3},
        // 化学工程专业
        {2,3,4}
    }
};
