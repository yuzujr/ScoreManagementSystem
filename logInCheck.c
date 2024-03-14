//登录检查
#pragma once
#include "logInCheck.h"
#include "dataProcess.h"
#include <string.h>

extern FILE* stuFileptr;
extern int lineNumber;

int stuLogInCheck(char* account,char* passwd){
    FILE* pf;
    lineNumber=1;
    char faccount[100],fpasswd[100],restOfLine[100];
    //打开学生信息文件
    pf=fopen("D:/c++/Qt/ScoreManagementSystem/studentInfo.txt","r");
    //检查是否打开成功
    if(pf==NULL){
        return -1;//即失败，将在调用处弹出警告
    }
    else {
        while(!feof(pf)){
            fscanf(pf,"%s %s",faccount,fpasswd);
            if(strcmp(faccount,account)==0&&strcmp(fpasswd,passwd)==0){
                stuFileptr=pf;
                moveToLineStart(stuFileptr);
                return 1;//即登录成功
            }
            else{
            fgets(restOfLine,100,pf);//去除此行剩余信息
            lineNumber++;
            }
        }
        return -2;//即账号或密码错误
    }
    //文件关闭
    fclose(pf);
}

int adminLogInCheck(char* account,char* passwd){
    FILE* pf;
    char faccount[100],fpasswd[100],restOfLine[100];
    //打开学生信息文件
    pf=fopen("adminInfo.txt","r");
    //检查是否打开成功
    if(pf==NULL){
        return -1;//即失败，将在调用处弹出警告
    }
    else{
        while(!feof(pf)){
            fscanf(pf,"%s %s",faccount,fpasswd);
            fgets(restOfLine,100,pf);//去除此行剩余信息
            if(strcmp(faccount,account)==0&&strcmp(fpasswd,passwd)==0){
                return 1;//即登录成功
            }
        }
        return -2;//即账号或密码错误
    }
    //文件关闭
    fclose(pf);
}
