#pragma once
#include<stdio.h>
#include<string.h>

char allCourse[100][100] = { {"微积分"},{"线性代数"},{"大学体育"},{"程序设计基础"}};//课程

 float credit[100] = { 3.5,2.5,2,4};//学分

struct Student
{
	char stu_number[100];//学号

	char stu_password[100];//密码

	char stu_name[100];//姓名

	char stu_college[100];//学院

	char stu_major[100];//专业

	char stu_award[100][100];//获奖信息

	int stu_award_num;//获奖数量

	double stu_course_grade[100][2];//课程编号及成绩

	int stu_course_num;//课程数量

	double stu_grade_point;//绩点

	int stu_classnum;//班级
};


