#include "student.h"

//结构体初始化，防止产生未定义行为
void paperInit(Paper *paper) {
    paper->writer_num = 0; //作者数量
    paper->paper_extra_credit = 0; //论文加分（分值或零）
}

void awardInit(Award *award) {
    award->award_winner_num = 0; //获奖者数量
    award->is_extra_credit = 0; //是否加分（分值或0）
    award->competition_level = NULL; //大赛级别（A /B /C）
}

void studentInit(Student *student) {
    for (int i = 0; i < 100; i++) {
        awardInit(&student->stu_award[i]);
    }
    student->stu_award_num = 0; //获奖数量
    for (int i = 0; i < 100; i++) {
        paperInit(&student->stu_paper[i]);
    }
    student->stu_paper_num = 0; //论文数量
    student->stu_course_num = 0; //课程数量
    student->stu_grade_point = 0; //绩点
    student-> stu_classnum = 0; //班级
}
