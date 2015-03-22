#ifndef  _USER_INFO_H_
#define   _USER_INFO_H_

#include <string>
#include <istream>
#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include "strdeal.h"
#include <list>
#include <stdio.h>


#define  _NAME_LENGTH_   15
#define  _PASSWORD_LENGTH_   50
#define  _MAX_ALIAS_LENGTH_   100
using namespace std;
struct alias{
    char  sysCmd[_MAX_ALIAS_LENGTH_] ;    //�����������ԭ��
	char  subCmd[_MAX_ALIAS_LENGTH_]; //�������������
};

struct userInfo{
	char  username[_NAME_LENGTH_] ;   //�û���
	char   password[_PASSWORD_LENGTH_] ;   //�û�����   
	
};


struct userInfo getUserFromFile(string path);
void writeUserToFile(string path , struct userInfo info);

struct  currUserInfo{
string  username ;   //�û���
//string   password ;   //�û�����
list<struct alias > myalias ; //���û��滻���
list<string> myHisCmd ; //���û���ʷ��� 
};

void getMyHisCmd (struct currUserInfo & curruser);
void saveMyHisCmd (struct currUserInfo & curruser);

void getMyAlias (struct currUserInfo & curruser);
void saveMyAlias (struct currUserInfo & curruser);
#endif