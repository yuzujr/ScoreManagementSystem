#pragma once
#include"student-grademanager.h"
#include "globalVar.h"

void init_find_result()
{
	for (int i = 0; i < 100; i++) {
		find_result[i] = NULL;
	}
	return;
}

stu_list* build()
{
	stu_list* head;
	head = (stu_list*)malloc(sizeof(stu_list));
	head->next = NULL;
	head->prev = NULL;
	return head;
}

stu_list* init_list(stu_list* head)
{
	stu_list* p = head->next;
	while (p != NULL)
	{
		stu_list* v;
		v = p;
		p = p->next;
		free(v);
	}
	head->next = NULL;
	head->prev = NULL;
	return head;
}

void insert_stu(stu_list* pos, Student* stu)
{
    if (pos == NULL) {
        return;
    }

    stu_list* v = (stu_list*)malloc(sizeof(stu_list));
    if (v == NULL) {
        return;
    }

    v->m_stu = *stu;
    v->next = pos->next;
    v->prev = pos;

    if (pos->next != NULL) {
        pos->next->prev = v;
    }

    pos->next = v;
}

stu_list* sort_ascend_by_name(stu_list* head)
{
    stu_list* p0, * p, * r0, * r, * tmp;
    p0 = r0 = tmp = NULL;
    p0 = head;
    p = head->next;
    while (p != NULL)
    {
        r = head->next;
        while (strcmp(r->m_stu.stu_name, p->m_stu.stu_name)<=0 && r != p)
        {
            r0 = r;
            r = r->next;
        }
        if (r != p)
        {
            tmp = p;
            stu_list* q = p->next;
            p0->next = q;
            if(q!=NULL)q->prev = p0;
            p = p0;
            if (r == head->next)
            {
                head->next = tmp;
                tmp->next = r;
                r->prev = tmp;
                tmp->prev = head;
            }
            else {
                r0->next = tmp;
                tmp->next = r;
                r->prev = tmp;
                tmp->prev = r0;
            }
        }
        p0 = p;
        p = p->next;
    }
    return head;
}

stu_list* sort_descend_by_name(stu_list* head)
{
    stu_list* p0, * p, * r0, * r, * tmp;
    p0 = r0 = tmp = NULL;
    p0 = head;
    p = head->next;
    while (p != NULL)
    {
        r = head->next;
        while (strcmp(r->m_stu.stu_name, p->m_stu.stu_name) >= 0 && r != p)
        {
            r0 = r;
            r = r->next;
        }
        if (r != p)
        {
            tmp = p;
            stu_list* q = p->next;
            p0->next = q;
            if (q != NULL)q->prev = p0;
            p = p0;
            if (r == head->next)
            {
                head->next = tmp;
                tmp->next = r;
                r->prev = tmp;
                tmp->prev = head;
            }
            else {
                r0->next = tmp;
                tmp->next = r;
                r->prev = tmp;
                tmp->prev = r0;
            }
        }
        p0 = p;
        p = p->next;
    }
    return head;
}

stu_list* sort_ascend(stu_list* head)
{
	stu_list* p0, * p, * r0, * r, * tmp;
	p0 = r0 = tmp = NULL;
	p0 = head;
	p = head->next;
	while (p != NULL)
	{
		r = head->next;
		while (r->m_stu.stu_grade_point < p->m_stu.stu_grade_point && r != p)
		{
			r0 = r;
			r = r->next;
		}
		if (r != p)
		{
			tmp = p;
			stu_list* q = p->next;
			p0->next = q;
			if(q!=NULL)q->prev = p0;
			p = p0;
			if (r == head->next)
			{
				head->next = tmp;
				tmp->next = r;
				r->prev = tmp;
				tmp->prev = head;
			}
			else {
				r0->next = tmp;
				tmp->next = r;
				r->prev = tmp;
				tmp->prev = r0;
			}
		}
		p0 = p;
		p = p->next;
	}
	return head;
}

stu_list* sort_descend(stu_list* head)
{
	stu_list* p0, * p, * r0, * r, * tmp;
	p0 = r0 = tmp = NULL;
	p0 = head;
	p = head->next;
	while (p != NULL)
	{
		r = head->next;
		while (r->m_stu.stu_grade_point > p->m_stu.stu_grade_point && r != p)
		{
			r0 = r;
			r = r->next;
		}
		if (r != p)
		{
			tmp = p;
			stu_list* q = p->next;
			p0->next = q;
			if (q != NULL)q->prev = p0;
			p = p0;
			if (r == head->next)
			{
				head->next = tmp;
				tmp->next = r;
				r->prev = tmp;
				tmp->prev = head;
			}
			else {
				r0->next = tmp;
				tmp->next = r;
				r->prev = tmp;
				tmp->prev = r0;
			}
		}
		p0 = p;
		p = p->next;
	}
	return head;
}

int find_stu_num(stu_list* head, char stu_num[])
{
	init_find_result();
	int number = 0;
	stu_list* p = head->next;
	while (p != NULL)
	{
        if (!strcmp(p->m_stu.stu_number, stu_num))
		{
            find_result[number] = p;
			number++;
		}
        p=p->next;
	}
	return number;
}

int find_stu_name(stu_list* head, char name[])
{
	init_find_result();
	int number = 0;
	stu_list* p = head->next;
	while(p != NULL)
	{
        if (!strcmp(p->m_stu.stu_name, name))
        {
            find_result[number] =p;
			number++;
        }
        p=p->next;
	}
	return number;
}

int find_stu_college(stu_list* head, char college[])
{
	init_find_result();
	int number = 0;
	stu_list* p = head->next;
	while (p != NULL)
	{
        if (!strcmp(p->m_stu.stu_college, college))
		{
            find_result[number] = p;
			number++;
		}
        p=p->next;
	}
	return number;
}

int find_stu_major(stu_list* head, char major[])
{
	init_find_result();
	int number = 0;
	stu_list* p = head->next;
	while (p != NULL)
	{
        if (!strcmp(p->m_stu.stu_major, major))
		{
            find_result[number] = p;
			number++;
		}
        p=p->next;
	}
	return number;
}

int find_stu_class(stu_list* head, char college[],int classnumber)
{
	init_find_result();
	int number = 0;
	stu_list* p = head->next;
	while (p != NULL)
	{
        if (!strcmp(p->m_stu.stu_college, college)&&p->m_stu.stu_classnum==classnumber)
		{
            find_result[number] = p;
			number++;
		}
        p=p->next;
	}
	return number;
}

void delete_stu(stu_list* pos)
{
	if (pos->prev == NULL)return;
	else if (pos->next == NULL) {
		stu_list* p0 = pos->prev;
		p0->next = NULL;
		free(pos);
	}
	else {
		stu_list* p0 = pos->prev, * p = pos->next;
		p0->next = p;
		p->prev = p0;
		free(pos);
	}
	return;
}

void modify_stu(stu_list* pos, Student* new_stu)
{
	pos->m_stu = *new_stu;
	return;
}

stu_list* load_data(stu_list* head)
{
    stu_list* tail;
    // tail = (stu_list*)malloc(sizeof(stu_list));
    tail = head;
    FILE* fp;
    fp= fopen(STU_FILE, "r");
    if (fp == NULL) {
        printf("文件无法打开");
        return head;
    }
    else {
        while (!feof(fp))
        {
            stu_list* p;
            p = (stu_list*)malloc(sizeof(stu_list));
            fscanf(fp, "%s %s %s %s %s", p->m_stu.stu_number, p->m_stu.stu_password, p->m_stu.stu_name, p->m_stu.stu_college, p->m_stu.stu_major);
            fscanf(fp, "%d %lf %d %d %d", &p->m_stu.stu_classnum,&p->m_stu.stu_grade_point, &p->m_stu.stu_course_num, &p->m_stu.stu_award_num,&p->m_stu.stu_paper_num);
            //课程及成绩
            for (int i = 0; i < p->m_stu.stu_course_num; i++)
            {
                fscanf(fp, "%lf %lf %lf", &p->m_stu.stu_course_grade[i][0], &p->m_stu.stu_course_grade[i][1],&p->m_stu.stu_course_grade[i][2]);

            }

            //获奖项目
            for (int i = 0; i < p->m_stu.stu_award_num; i++)
            {
                fscanf(fp,"%s %s %d", p->m_stu.stu_award[i].award_name, p->m_stu.stu_award[i].award_hosted_by, &p->m_stu.stu_award[i].award_winner_num);

                for (int j = 0; j < p->m_stu.stu_award[i].award_winner_num; j++)
                {
                    fscanf(fp, "%s", p->m_stu.stu_award[i].award_allwinner[j]);
                }
                fscanf(fp, "%lf %c %s", &p->m_stu.stu_award[i].is_extra_credit, &p->m_stu.stu_award[i].competition_level, p->m_stu.stu_award[i].award_time);
            }

            //论文
            for (int i = 0; i < p->m_stu.stu_paper_num; i++)
            {
                fscanf(fp, "%s %d",p->m_stu.stu_paper[i].paper_name,&p->m_stu.stu_paper[i].writer_num);

                for (int j = 0; j < p->m_stu.stu_paper[i].writer_num; j++)
                {
                    fscanf(fp, "%s", p->m_stu.stu_paper[i].paper_allwriter[j]);
                }

                fscanf(fp, "%s %s %lf", p->m_stu.stu_paper[i].paper_periodicalname, p->m_stu.stu_paper[i].paper_time, &p->m_stu.stu_paper[i].paper_extra_credit);
            }

            fscanf(fp, "\n");
            p->prev = tail;
            tail->next = p;
            tail = p;
            p->next = NULL;
        }
    }
    fclose(fp);
    return head;
}

void save_data(stu_list* head)
{
    stu_list* p = head->next;
    FILE* fp;
    fp = fopen(STU_FILE, "w");
    while (p != NULL)
    {
        fprintf(fp, "%s %s %s %s %s", p->m_stu.stu_number, p->m_stu.stu_password, p->m_stu.stu_name, p->m_stu.stu_college, p->m_stu.stu_major);
        fprintf(fp, " %d %lf %d %d %d", p->m_stu.stu_classnum, p->m_stu.stu_grade_point, p->m_stu.stu_course_num, p->m_stu.stu_award_num,p->m_stu.stu_paper_num);
        for (int i = 0; i < p->m_stu.stu_course_num; i++)
        {
            fprintf(fp, " %lf %lf %lf", p->m_stu.stu_course_grade[i][0], p->m_stu.stu_course_grade[i][1],p->m_stu.stu_course_grade[i][2]);
        }

        //获奖项目
        for (int i = 0; i < p->m_stu.stu_award_num; i++)
        {
            fprintf(fp, " %s %s %d", p->m_stu.stu_award[i].award_name, p->m_stu.stu_award[i].award_hosted_by, p->m_stu.stu_award[i].award_winner_num);

            for (int j = 0; j < p->m_stu.stu_award[i].award_winner_num; j++)
            {
                fprintf(fp, " %s", p->m_stu.stu_award[i].award_allwinner[j]);
            }

            fprintf(fp, " %lf %c %s", p->m_stu.stu_award[i].is_extra_credit, p->m_stu.stu_award[i].competition_level, p->m_stu.stu_award[i].award_time);
        }

        //论文
        for (int i = 0; i < p->m_stu.stu_paper_num; i++)
        {
            fprintf(fp, " %s %d", p->m_stu.stu_paper[i].paper_name, p->m_stu.stu_paper[i].writer_num);

            for (int j = 0; j < p->m_stu.stu_paper[i].writer_num; j++)
            {
                fprintf(fp, " %s", p->m_stu.stu_paper[i].paper_allwriter[j]);
            }

            fprintf(fp, " %s %s %lf", p->m_stu.stu_paper[i].paper_periodicalname, p->m_stu.stu_paper[i].paper_time, p->m_stu.stu_paper[i].paper_extra_credit);
        }

        fprintf(fp, "\n");
        p = p->next;
    }
    fclose(fp);
    return;
}

void printAll(stu_list* head)
{
	stu_list* p = head->next;
	while (p != NULL)
	{
		printf("姓名：%s\n", p->m_stu.stu_name);
		printf("学号：%s  密码：%s\n", p->m_stu.stu_number,p->m_stu.stu_password);
		printf("学院：%s  专业：%s  绩点：%.1lf\n", p->m_stu.stu_college, p->m_stu.stu_major, p->m_stu.stu_grade_point);
		printf("课程及其成绩：\n");
		for (int i = 0; i < p->m_stu.stu_course_num; i++)
		{
            printf("课程%d：%s  成绩：%.1lf\n", i + 1, allCourses[(int)p->m_stu.stu_course_grade[i][0]], p->m_stu.stu_course_grade[i][1]);
		}
		printf("获奖信息：\n");
		for (int i = 0; i < p->m_stu.stu_award_num; i++)
		{
			printf("奖项%d：%s\n",i+1, p->m_stu.stu_award[i]);
		}
		p = p->next;
	}	
}
