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
    char  sysCmd[_MAX_ALIAS_LENGTH_] ;    //所代替的命令原名
	char  subCmd[_MAX_ALIAS_LENGTH_]; //代替的新命令名
};

struct userInfo{
	char  username[_NAME_LENGTH_] ;   //用户名
	char   password[_PASSWORD_LENGTH_] ;   //用户密码   
	
};


struct userInfo getUserFromFile(string path);
void writeUserToFile(string path , struct userInfo info);

struct  currUserInfo{
string  username ;   //用户名
//string   password ;   //用户密码
list<struct alias > myalias ; //该用户替换命令集
list<string> myHisCmd ; //该用户历史命令集 
};

void getMyHisCmd (struct currUserInfo & curruser);
void saveMyHisCmd (struct currUserInfo & curruser);

void getMyAlias (struct currUserInfo & curruser);
void saveMyAlias (struct currUserInfo & curruser);
#endif