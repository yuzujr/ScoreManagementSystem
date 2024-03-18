#pragma once
#include<stdio.h>
#include<string.h>

char allCourse[100][100] = { {"微积分"},{"线性代数"},{"大学体育"},{"程序设计基础"}};//课程

 float credit[100] = { 3.5,2.5,2,4};//学分

 typedef struct Award//获奖信息
 {
	 char award_name[100];//大赛名称及获奖级别

	 char award_hosted_by[100];//主办单位

	 int award_winner_num;//获奖者数量

	 char award_allwinner[100][100];//所有获奖者 

	 double is_extra_credit;//是否加分（分值或0）

	 char competition_level;//大赛级别（A /B /C）

	 char award_time[100];//获奖时间

 }Award;

 typedef struct Paper//论文信息
 {
	 char paper_name[100];//论文名称

	 int writer_num;//作者数量

	 char paper_allwriter[100][100];//所有作者（如果有通讯作者放第一位并且加上@）
	 
	 char paper_periodicalname[100];//期刊名称/会议名称+（卷/期/论文号/页码范围（Pxx~Pxx））
	 
	 char paper_time[100];//发表时间
	 
	 double paper_extra_credit;//论文加分（分值或零）

 }Paper;



struct Student
{
	char stu_number[100];//学号

	char stu_password[100];//密码

	char stu_name[100];//姓名

	char stu_college[100];//学院

	char stu_major[100];//专业

	Award stu_award[10];//获奖信息

	int stu_award_num;//获奖数量
	
	Paper stu_paper[10];
	
	int stu_paper_num;//论文数量

	double stu_course_grade[100][2];//课程编号及成绩

	int stu_course_num;//课程数量

	double stu_grade_point;//绩点

	int stu_classnum;//班级
};


