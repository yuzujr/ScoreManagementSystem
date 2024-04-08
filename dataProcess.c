#pragma once
#include "dataProcess.h"

//移动文件指针到行首
void moveToLineStart(FILE* pf) {
  int pos = ftell(pf);
  while (pos > 1) {
    fseek(pf, -2, SEEK_CUR);
    char ch = fgetc(pf);
    if (ch == '\n') {
      break;
    }
    pos = ftell(pf);
  }
  if (pos == 1) {
    rewind(pf);
  }
}

bool containsChinese(const char *str) {
    while (*str) {
        if ((*str & 0x80) && (*(str + 1) & 0x80)) {
            return true; // 检测到汉字
        }
        str++;
    }
    return false;
}

int containsSymbol(const char *str) {
    while (*str) {
        if (!isalnum(*str)) { // 如果字符不是字母或数字
            return 1; // 返回1表示存在符号
        }
        str++; // 指向下一个字符
    }
    return 0; // 返回0表示没有符号
}

bool isPasswordValid(const char *password) {
    // 检查密码是否包含汉字或字符
    if (containsChinese(password)||containsSymbol(password)) {
        return false;
    }
    return true;
}

int modify_file(const char* filename,const int linenumber,const char *new_content) {
    FILE *file = fopen(filename, "r");
        if (file == NULL) {
            perror("源文件打开错误");
            return -1;
        }
        char buffer[1000];
        // 构建临时文件路径
        char temp_file[1000];
        strcpy(temp_file, filename); // 复制原始文件路径
        strcat(temp_file, ".temp");  // 将 ".temp" 添加到文件名后面

        FILE *temp = fopen(temp_file, "w");
        if (temp == NULL) {
            perror("临时文件打开错误");
            return -1;
        }
        int current_line = 1;
        while (fgets(buffer, 1000, file) != NULL) {
            if (current_line == linenumber) {
                fputs(new_content, temp);
            } else {
                fputs(buffer, temp);
            }
            current_line++;
        }//得到修改后的临时文件
//以下把临时文件的内容覆写进源文件
        fclose(file);
        fclose(temp);
        file=fopen(filename,"w");
        temp=fopen(temp_file,"r");
        if(file==NULL){
            return -1;
        }
        while(!feof(temp)){
            fgets(buffer,1000,temp);
            fputs(buffer,file);
        }
        fclose(file);
        fclose(temp);
        return 0;
}


int changePasswdTo(char* newPasswd){
    if(strlen(newPasswd) < 8){
        return -3;//密码过短
    }
    if(strlen(newPasswd)>20){
        return -4;//密码过长
    }
    //FILE* test;
    //test=fopen("test.txt","w");//测试文件
    //检查新密码是否合法(即不含有汉字和字符)
    if(!isPasswordValid(newPasswd)){
        return -1;//密码不合法
    }
    // 得到原行
    char buffer[1000];
    if(isStudent){
        fgets(buffer, 1000, stuFileptr);
    }
    else{
        fgets(buffer, 1000, workerFileptr);
    }
    //fputs(buffer,test);
    //fprintf(test,"\n");//测试,原行
    //得到修改后的行
    int passwdIndex = 0;
    int passwdLength = 0;
    int passwdFlag = 0;
    for (int i = 0; buffer[i] != '\0'; i++) {
      if (buffer[i] == ' ' && passwdFlag || buffer[i] == '\n') {
        passwdLength = i - passwdIndex;
        break;
      }
      if (buffer[i] == ' ' && !passwdFlag) {
        passwdIndex = i + 1;
        passwdFlag = 1;
      }
    }
    int newLenth = strlen(newPasswd);
    int moveSize = newLenth - passwdLength;
    int oldsize = strlen(buffer);
    int newsize = strlen(buffer) + moveSize;
    if (moveSize < 0) {
      moveSize = 0 - moveSize;
      for (int i = passwdIndex + passwdLength - moveSize; i + moveSize < oldsize;
           i++) {
        buffer[i] = buffer[i + moveSize];
        if (i + moveSize >= newsize) {
          buffer[i + moveSize] = NULL;
        }
      }
      for (int i = passwdIndex, k = 0; i < passwdIndex + newLenth; i++, k++) {
        buffer[i] = newPasswd[k];
      }
    } else {
      for (int i = passwdIndex + passwdLength - moveSize, k = strlen(buffer) - 1;
           i + moveSize < oldsize; i++, k--) {
        buffer[k + moveSize] = buffer[k];
      }
      for (int i = passwdIndex, k = 0; i < passwdIndex + newLenth; i++, k++) {
        buffer[i] = newPasswd[k];
      }
    }
    for (int i=newsize;i<1000;i++){
        buffer[i]=NULL;
    }//删除多余字符
    //fputs(buffer,test);//测试，修改后行
    char* fileLocation;
    if(isStudent){
        fileLocation=STU_FILE;
    }
    else{
        fileLocation=ADMIN_FILE;
    }
    if(modify_file(fileLocation,lineNumber,buffer)==-1){
            return -2;//文件打开失败
    }
    else{
    return 0;//修改成功
    }
}

Student* loadSingleStudent(){
    Student* student=(Student*)malloc(sizeof(Student));
    fscanf(stuFileptr, "%s %s %s %s %s", student->stu_number, student->stu_password, student->stu_name, student->stu_college, student->stu_major);
    fscanf(stuFileptr, "%d %lf %d %d %d", &student->stu_classnum,&student->stu_grade_point, &student->stu_course_num, &student->stu_award_num,&student->stu_paper_num);
    //课程及成绩
    for (int i = 0; i < student->stu_course_num; i++)
    {
        fscanf(stuFileptr, "%lf %lf %lf", &student->stu_course_grade[i][0], &student->stu_course_grade[i][1],&student->stu_course_grade[i][2]);
    }

    //获奖项目
    for (int i = 0; i < student->stu_award_num; i++)
    {
        fscanf(stuFileptr,"%s %s %d", student->stu_award[i].award_name, student->stu_award[i].award_hosted_by, &student->stu_award[i].award_winner_num);

        for (int j = 0; j < student->stu_award[i].award_winner_num; j++)
        {
            fscanf(stuFileptr, "%s", student->stu_award[i].award_allwinner[j]);
        }

        fscanf(stuFileptr, "%lf %c %s", &student->stu_award[i].is_extra_credit, &student->stu_award[i].competition_level, student->stu_award[i].award_time);
    }

    //论文
    for (int i = 0; i < student->stu_paper_num; i++)
    {
        fscanf(stuFileptr, "%s %d",student->stu_paper[i].paper_name,&student->stu_paper[i].writer_num);

        for (int j = 0; j < student->stu_paper[i].writer_num; j++)
        {
            fscanf(stuFileptr, "%s", student->stu_paper[i].paper_allwriter[j]);
        }

        fscanf(stuFileptr, "%s %s %lf", student->stu_paper[i].paper_periodicalname, student->stu_paper[i].paper_time, &student->stu_paper[i].paper_extra_credit);
    }
    moveToLineStart(stuFileptr);
    return student;
}

double updateCredit(double grade) {
    //分段记录绩点并计算gpa
    if (grade >= 90) {
        return 4.0;
    } else if (grade >= 85 && grade < 90) {
        return 3.7;
    } else if (grade >= 82 && grade < 85) {
        return 3.3;
    } else if (grade >= 78 && grade < 82) {
        return 3.0;
    } else if (grade >= 75 && grade < 78) {
        return 2.7;
    } else if (grade >= 72 && grade < 75) {
        return 2.3;
    } else if (grade >= 68 && grade < 72) {
        return 2.0;
    } else if (grade >= 64 && grade < 68) {
        return 1.5;
    } else if (grade >= 60 && grade < 64) {
        return 1.0;
    } else {
        return 0.0;
    }
}

//查找学院索引
int findCollegeIndex(const char *college) {
    int index = 0;
    for (; colleges[index][0] != '\0'&&strcmp(college, colleges[index]) != 0; index++);
    return index;
}

//查找专业索引
int findMajorIndex(int collegeIndex, const char *major) {
    int index = 0;
    for (; majors[collegeIndex][index][0] != '\0'&&strcmp(major, majors[collegeIndex][index]) != 0; index++);
    return index;
}
