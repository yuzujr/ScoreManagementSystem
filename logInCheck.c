//登录检查
#pragma once
#include "logInCheck.h"
#include "dataProcess.h"
#include "globalVar.h"
#include <string.h>

// extern FILE* stuFileptr;
// extern FILE* workerFileptr;
// extern int lineNumber;
// extern int isStudent;
// extern int isAdmin;
// extern char* STU_FILE;
// extern char* ADMIN_FILE;

int stuLogInCheck(char* account,char* passwd){
    FILE* pf;
    lineNumber=1;
    char faccount[100],fpasswd[100],restOfLine[10000];
    //打开学生信息文件
    pf=fopen(STU_FILE,"r");
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
                isStudent=1;
                return 1;//即登录成功
            }
            else{
            fgets(restOfLine,1000,pf);//去除此行剩余信息
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
    lineNumber=1;
    char faccount[100],fpasswd[100],restOfLine[100];
    //打开职工信息文件
    pf=fopen(ADMIN_FILE,"r");
    //检查是否打开成功
    if(pf==NULL){
        return -1;//即失败，将在调用处弹出警告
    }
    else{
        while(!feof(pf)){
            char isadmin[10];
            fscanf(pf,"%s %s",faccount,fpasswd);
            if(strcmp(faccount,account)==0&&strcmp(fpasswd,passwd)==0){
                fscanf(pf,"%s",isadmin);
                if(isadmin[0]=='1') {isAdmin=1;}
                else {isAdmin=0;}
                workerFileptr=pf;
                moveToLineStart(workerFileptr);
                isStudent=0;
                return 1;//即登录成功
            }
            else{
            fgets(restOfLine,1000,pf);//去除此行剩余信息
            lineNumber++;
            }
        }
        return -2;//即账号或密码错误
    }
    //文件关闭
    fclose(pf);
}
