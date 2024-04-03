#ifndef WORKER_H
#define WORKER_H
#pragma once
typedef struct Worker {
    bool isAdmin;

    char stu_college[100];//学院

    char stu_major[100];//专业

    char allCourses[100][100];//课程名称
}Worker;
void workerInit();
#endif // WORKER_H
