#pragma once
#include <stdio.h>
#include <malloc.h>
#include "student.h"


//学生名单
struct stu_list {
    struct Student m_stu;
    struct stu_list *next;
    struct stu_list *prev;
};

//改名
typedef struct stu_list stu_list;
typedef struct Student Student;

void init_find_result();//清空查找结果

stu_list *build();//创建空链表

//注意：这里传入的是Student指针，Student生命周期不能确定，所以插入后立即保存
void insert_stu(stu_list *pos/*在此节点后插入*/, Student *stu); //增加学生

stu_list *init_list(stu_list *);//初始化名单

stu_list *sort_ascend(stu_list *);//GPA升序

stu_list* sort_ascend_by_name(stu_list*);//姓名升序

stu_list *sort_descend(stu_list *);//GPA降序

stu_list* sort_descend_by_name(stu_list *);//姓名降序

//多方式查找
int find_stu_num(stu_list *, char[]);

int find_stu_name(stu_list *, char[]);

int find_stu_college(stu_list *, char[]);

int find_stu_major(stu_list *, char[]);

int find_stu_class(stu_list *, char[], int);

void modify_stu(stu_list *, Student *); //修改学生信息

void delete_stu(stu_list *);//删除学生信息

void printAll(stu_list *head);//打印所有学生信息

stu_list *load_data(stu_list *); //加载文件数据

void save_data(stu_list *head); //保存文件数据
