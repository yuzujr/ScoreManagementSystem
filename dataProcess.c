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

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

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

// 修改文件中的某一行
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
//1.字符串错误，
//2.无法删除和改名
int changePasswdTo(char* newPasswd){
    if(strlen(newPasswd) < 8){
        return -3;//密码过短
    }
    FILE* test;
    //检查新密码是否合法(即不含有汉字和字符)
    if(!isPasswordValid(newPasswd)){
        return -1;//密码不合法
    }
    // 得到原行
    char buffer[1000];
    fgets(buffer, 1000, stuFileptr);
    test=fopen("test.txt","w");
    fputs(buffer,test);
    fprintf(test,"\n");
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
    if (moveSize < 0) {
      moveSize = 0 - moveSize;
      int newsize = strlen(buffer) - moveSize;
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
    fputs(buffer,test);
    if(modify_file("D:/c++/Qt/ScoreManagementSystem/studentInfo.txt",lineNumber,buffer)==-1){
            return -2;//文件打开失败
    }
    else{
    return 0;//修改成功
    }
}

