/*********************************************************************************************************************
/*文件名:  sysInfo.cpp
/*模块名: 用户
/*作者:    邹天明&戴潜
/*日期:    2011.10.25
/*功能:    用户的新建和切换
/*
/*********************************************************************************************************************/
#include "sysInfo.h"


/*********************************************************************************************************************
/*函数名:  readUserList
/*输入:  list<struct userInfo > &userList  , const  string &path
/*返回值:    void
/*模块名: 新建用户模块
/*功能描述: 读用户列表
/*
/*********************************************************************************************************************/
void readUserList( list<struct userInfo > &userList  , const  string &path)
{
	struct userInfo  tempuser ;
	FILE *pFile ;
    int amount = 0 , i =0;

	if(_access(path.c_str() , 0) != 0)    return ;//若文件不存在，返回
	
	userList.clear();
	pFile = fopen(path.c_str() , "rb");
    fread(&amount, sizeof(int), 1, pFile); 
	while( i< amount)
	{
		fread(&tempuser, sizeof(struct userInfo) , 1, pFile);
	    strcpy_s(tempuser.password , Decrypt(tempuser.password).c_str());//密码解密后读出
		userList.push_back(tempuser);
		++i;
	}
	fclose(pFile);
}
/*********************************************************************************************************************
/*函数名: writeUserList
/*输入:  list<struct userInfo > &userList ,const  string &path
/*返回值:    void
/*模块名: 新建用户模块
/*功能描述: 写用户列表到文件
/*
/*********************************************************************************************************************/
void writeUserList(list<struct userInfo > &userList ,const  string &path)
{
    struct userInfo  tempuser ;
    FILE *pFile = fopen(path.c_str() , "wb"); // 用二进制打开
	int size ;
	size = userList.size();
    fwrite(&size, sizeof(int), 1, pFile);
	size = sizeof(tempuser);
	for (list<struct userInfo>::iterator it = userList.begin(); it != userList.end() ; it++)
	{
        strcpy_s(tempuser.username ,(*it).username);
	    strcpy_s(tempuser.password , Encrypt((*it).password).c_str());  //密码加密后写入
		fwrite(&tempuser, size, 1, pFile);
	}
	fclose(pFile);
}

