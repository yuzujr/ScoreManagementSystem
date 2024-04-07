#pragma once
#ifndef WORKER_H
#define WORKER_H


#include "globalVar.h"
#include "dataProcess.h"

typedef struct Worker {
    bool m_isAdmin;

    char workerNumber[100];//职工号

    char passwd[100];//密码

    char stu_college[100];//学院

    char teachingCourse[100];//课程名称
} Worker;


#endif // WORKER_H
