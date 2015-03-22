/*********************************************************************************************************************
/*�ļ���:  userInfo.cpp
/*ģ����: �û�
/*����:    ������&��Ǳ
/*����:    2011.10.25
/*����:    ����ʼ���û��ĳ�ʼ��
/*
/*********************************************************************************************************************/


#include "userInfo.h"
#define  USER_DATA_PATH  "C:\\tianmingseed\\data\\" 


/*********************************************************************************************************************
/*������: getMyHisCmd 
/*����:  struct currUserInfo & curruser
/*����ֵ:    void
/*ģ����: �û�ģ��
/*��������: ��ø��û�������
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
/*������:  saveMyHisCmd 
/*����: struct currUserInfo & curruser
/*����ֵ:    void
/*ģ����: ��ʼ��ģ��
/*��������: �����û�������
/*
/*********************************************************************************************************************/
void saveMyHisCmd (struct currUserInfo & curruser)
{
	 string path = USER_DATA_PATH  + curruser.username +"His"+".dat" ;
      putStrToFile(path , curruser.myHisCmd);
	  return ;
}


/*********************************************************************************************************************
/*������:  getMyAlias
/*����: struct currUserInfo & curruser
/*����ֵ:    void
/*ģ����: �û�ģ��
/*��������: ���ļ�����alias�ַ���
/*
/*********************************************************************************************************************/
void getMyAlias (struct currUserInfo & curruser)
{ 
	struct alias  temp;
	string path = USER_DATA_PATH + curruser.username+"Alias"+".dat";
	FILE *pFile ;
	//int size ;
	int amount = 0 , i =0;

	if(_access(path.c_str() , 0) != 0)    return ;//���ļ������ڣ�����

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
/*������: saveMyAlias
/*����: struct currUserInfo & curruser
/*����ֵ:    void
/*ģ����: �û�ģ��
/*��������:  ��alias�ַ���д���ļ�
/*
/*********************************************************************************************************************/
void saveMyAlias (struct currUserInfo & curruser)
{
	struct alias  temp;
	string path = USER_DATA_PATH + curruser.username+"Alias"+".dat";
	FILE *pFile = fopen(path.c_str() , "wb"); // �ö����ƴ�
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