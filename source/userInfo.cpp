/*********************************************************************************************************************
/*文件名:  userInfo.cpp
/*模块名: 用户
/*作者:    邹天明&戴潜
/*日期:    2011.10.25
/*功能:    程序开始，用户的初始化
/*
/*********************************************************************************************************************/


#include "userInfo.h"
#define  USER_DATA_PATH  "C:\\tianmingseed\\data\\" 


/*********************************************************************************************************************
/*函数名: getMyHisCmd 
/*输入:  struct currUserInfo & curruser
/*返回值:    void
/*模块名: 用户模块
/*功能描述: 获得该用户的数据
/*
/*********************************************************************************************************************/
void getMyHisCmd (struct currUserInfo & curruser)
{
	string path = USER_DATA_PATH + curruser.username +"His"+".dat" ;
	curruser.myHisCmd.clear();
	if (_access(path.c_str() , 0) == 0)
	{
		getStrFromFile(path , curruser.myHisCmd);
	}
	return ;
}


/*********************************************************************************************************************
/*函数名:  saveMyHisCmd 
/*输入: struct currUserInfo & curruser
/*返回值:    void
/*模块名: 初始化模块
/*功能描述: 储存用户的数据
/*
/*********************************************************************************************************************/
void saveMyHisCmd (struct currUserInfo & curruser)
{
	 string path = USER_DATA_PATH  + curruser.username +"His"+".dat" ;
      putStrToFile(path , curruser.myHisCmd);
	  return ;
}


/*********************************************************************************************************************
/*函数名:  getMyAlias
/*输入: struct currUserInfo & curruser
/*返回值:    void
/*模块名: 用户模块
/*功能描述: 从文件读出alias字符集
/*
/*********************************************************************************************************************/
void getMyAlias (struct currUserInfo & curruser)
{ 
	struct alias  temp;
	string path = USER_DATA_PATH + curruser.username+"Alias"+".dat";
	FILE *pFile ;
	//int size ;
	int amount = 0 , i =0;

	if(_access(path.c_str() , 0) != 0)    return ;//若文件不存在，返回

	curruser.myalias.clear();
	pFile = fopen(path.c_str() , "rb");
	fread(&amount, sizeof(int), 1, pFile); 
	while( i< amount)
	{
		fread(&temp, sizeof(struct alias) , 1, pFile);
		curruser.myalias.push_back(temp);
		++i;
	}
	fclose(pFile);
	return ;
}


/*********************************************************************************************************************
/*函数名: saveMyAlias
/*输入: struct currUserInfo & curruser
/*返回值:    void
/*模块名: 用户模块
/*功能描述:  将alias字符集写到文件
/*
/*********************************************************************************************************************/
void saveMyAlias (struct currUserInfo & curruser)
{
	struct alias  temp;
	string path = USER_DATA_PATH + curruser.username+"Alias"+".dat";
	FILE *pFile = fopen(path.c_str() , "wb"); // 用二进制打开
	int size ;
	size = curruser.myalias.size();
	fwrite(&size, sizeof(int), 1, pFile);
	size = sizeof(temp);
	for (list<struct alias>::iterator it = curruser.myalias.begin(); it != curruser.myalias.end() ; it++)
	{
		strcpy_s(temp.sysCmd ,(*it).sysCmd);
		strcpy_s(temp.subCmd ,(*it).subCmd);
		fwrite(&temp, size, 1, pFile);
	}
	fclose(pFile);
	return;
}