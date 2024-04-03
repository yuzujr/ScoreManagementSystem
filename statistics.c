//lcz-统计总人数
#include <stdio.h>
#include "student.h"
#include "student-grademanager.h"
#include "globalVar.h"

int CntNum(stu_list* head)
{
    int cnt = 0;
    stu_list* m_head = head;
    stu_list* p = head->next;

    if (head->next == NULL || head == NULL)
    {
        return -1;
    }
    else
    {
        while (p != NULL)
        {
            cnt++;
            p = p->next;
        }
    }
    return cnt;
}



//统计每种课程GPA之和, 并且直接计算平均值:
//GPA[subject]---[personNum],[GPAsum],[GPAaverage]
double GGPA[200][3] = { 0 };//各科平均绩点
double aveMark[200][1] = { 0 };//各科平均分

void GPA_(stu_list* head)
{
    stu_list* p = head->next;
    int q = 1;//位数
    int jw = 0;

    if (head == NULL || p == NULL)
    {
        printf("空链表\n");
        return;
    }
    while (p != NULL)
    {
        for (int i = 0; i < p->m_stu.stu_course_num; i++)//读入每个学生的课程
        {
            int temGPA = p->m_stu.stu_course_grade[i][2];//该学生该课程的GPA
            int temNbr = p->m_stu.stu_course_grade[i][0];//课程编号
            int temMRK = p->m_stu.stu_course_grade[i][1];
            GGPA[temNbr][0]++;//人数++
            GGPA[temNbr][1] += temGPA;//GPA总数++
            aveMark[temNbr][0] += temMRK;
        }
        p = p->next;
    }


    for (int i = 0; i < 200; i++)
    {
        GGPA[i][2] = GGPA[i][1] / GGPA[i][0];
        if(aveMark[i][0]!=0){
            aveMark[i][0] = aveMark[i][0] / GGPA[i][0];}//总分除以人数
    }
}

//学号, 科目编号  成绩
void TxTtoList(stu_list *phead, int MKnum)
{
    FILE* Out = fopen("list.txt", "r");
    if (Out == NULL)
    {
        printf("文件打开失败\n");
    }
    else
    {
        while (!feof(Out))
        {
            char arr[100] = { 0 };
            double mark = 0;
            fscanf(Out, "%s %lf", arr,&mark);
            fscanf(Out, "\n");
            for (int i = 0; i < 100; i++) {
                find_result[i] = NULL;
            }
            int number = 0;
            stu_list* p = phead->next;
            while (p != NULL)
            {
                if (!strcmp(p->m_stu.stu_number, arr))
                {
                    find_result[number] = p;
                    number++;
                }
                p=p->next;
            }

            int i = 0;
            for (i = 0; i < find_result[0]->m_stu.stu_course_num; i++)
            {
                if (find_result[0]->m_stu.stu_course_grade[i][0] == MKnum)
                {
                    find_result[0]->m_stu.stu_course_grade[i][1] = mark;
                }
            }

        }
    }
}
