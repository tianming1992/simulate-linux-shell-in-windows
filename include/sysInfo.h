#ifndef  _SYSINFO_H_
#define  _SYSINFO_H_

#include "userInfo.h"
#include <string>
#include <list>
#include <queue>
#include <fstream>
#include <conio.h>
#include <stdio.h>
#include "encrytion.h"

#define  I_AM_FILE  1
#define  I_AM_DIRECTORY 2
using namespace std ;

struct command {
	string  comName ;  //������
	int  argc ;  //���������������� 
	vector<string> argv ;//����������� 
};

struct ex_out
{
	string result ;//������
    string keyword ; //�����ؼ���
	char add_info ; //����ĸ�����Ϣ
};
struct sysInfo
{
	 struct currUserInfo curruser ; //��ǰ�û�
	 list<struct userInfo > sysUserInfo ; //ϵͳ�����û�������Ϣ 
	 string currPath ;//��ǰ�û�����·��
	 //struct command currCommand ; // ��ǰִ������
	 queue<struct command> commandQuene ;  //��ǰ����ִ�ж��� 
	 list<struct ex_out> execu_out ;//��һ����ִ����Ϻ�������������
	 bool    login ;           //�Ƿ��¼
	 int out_kind ; //�����������
	 string other_info ; //���ܵ�������Ϣ
};

void readUserList(  list<struct userInfo > &userList,  const  string &path); 
void writeUserList( list<struct userInfo > &userList ,const  string &path); 


#endif