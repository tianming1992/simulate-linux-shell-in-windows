/*********************************************************************************************************************
/*�ļ���:  sysInfo.cpp
/*ģ����: �û�
/*����:    ������&��Ǳ
/*����:    2011.10.25
/*����:    �û����½����л�
/*
/*********************************************************************************************************************/
#include "sysInfo.h"


/*********************************************************************************************************************
/*������:  readUserList
/*����:  list<struct userInfo > &userList  , const  string &path
/*����ֵ:    void
/*ģ����: �½��û�ģ��
/*��������: ���û��б�
/*
/*********************************************************************************************************************/
void readUserList( list<struct userInfo > &userList  , const  string &path)
{
	struct userInfo  tempuser ;
	FILE *pFile ;
    int amount = 0 , i =0;

	if(_access(path.c_str() , 0) != 0)    return ;//���ļ������ڣ�����
	
	userList.clear();
	pFile = fopen(path.c_str() , "rb");
    fread(&amount, sizeof(int), 1, pFile); 
	while( i< amount)
	{
		fread(&tempuser, sizeof(struct userInfo) , 1, pFile);
	    strcpy_s(tempuser.password , Decrypt(tempuser.password).c_str());//������ܺ����
		userList.push_back(tempuser);
		++i;
	}
	fclose(pFile);
}
/*********************************************************************************************************************
/*������: writeUserList
/*����:  list<struct userInfo > &userList ,const  string &path
/*����ֵ:    void
/*ģ����: �½��û�ģ��
/*��������: д�û��б��ļ�
/*
/*********************************************************************************************************************/
void writeUserList(list<struct userInfo > &userList ,const  string &path)
{
    struct userInfo  tempuser ;
    FILE *pFile = fopen(path.c_str() , "wb"); // �ö����ƴ�
	int size ;
	size = userList.size();
    fwrite(&size, sizeof(int), 1, pFile);
	size = sizeof(tempuser);
	for (list<struct userInfo>::iterator it = userList.begin(); it != userList.end() ; it++)
	{
        strcpy_s(tempuser.username ,(*it).username);
	    strcpy_s(tempuser.password , Encrypt((*it).password).c_str());  //������ܺ�д��
		fwrite(&tempuser, size, 1, pFile);
	}
	fclose(pFile);
}

