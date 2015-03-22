#ifndef  _FILE_DEAL_H_
#define   _FILE_DEAL_H_

#include <string>
#include <iostream>
#include <afxwin.h>
#include <direct.h>
#include <cstdlib>
#include<cstring>
#include <errno.h>
#include <conio.h>
#include <io.h>
#include <fstream>
#include <stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sstream>
#include <algorithm>
#include<cstdio>
#include <direct.h>
#include"strdeal.h"
#include <map>
#include <windows.h>

#define  FOLDER_ONLY 1
#define  FILE_ONLY 2
#define  FILE_AND_FOLDER 3
#define  MV_  1
#define  CP_  2


using namespace std ;

void mkSingleDir(const string &dir);
void mkMassDir(const string &dir);
bool isPath(string strpath);//�ж��Ƿ�ΪĿ¼
string cstrTostr(CString& csstr) ;//��cstringת��Ϊstring
CString strToCstr(string strTemp);//��string ת��Ϊ cstring


//����cout <<ʵ�������ɫ
inline std::ostream& blue(std::ostream &s)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
	SetConsoleTextAttribute(hStdout, FOREGROUND_BLUE
		|FOREGROUND_GREEN|FOREGROUND_INTENSITY);
	return s;
} 

inline std::ostream& red(std::ostream &s)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
	SetConsoleTextAttribute(hStdout, 
		FOREGROUND_RED|FOREGROUND_INTENSITY);
	return s;
} 



inline std::ostream& green(std::ostream &s)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
	SetConsoleTextAttribute(hStdout, 
		FOREGROUND_GREEN|FOREGROUND_INTENSITY);
	return s;
}

inline std::ostream& yellow(std::ostream &s)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
	SetConsoleTextAttribute(hStdout, 
		FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
	return s;
}

inline std::ostream& white(std::ostream &s)
{
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE); 
	SetConsoleTextAttribute(hStdout, 
		FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);
	return s;
}

struct color {
	color(WORD attribute):m_color(attribute){};
	WORD m_color;
}; 

template <class _Elem, class _Traits>
std::basic_ostream<_Elem,_Traits>& 
operator<<(std::basic_ostream<_Elem,_Traits>& i, color& c)
{
	HANDLE hStdout=GetStdHandle(STD_OUTPUT_HANDLE); 
	SetConsoleTextAttribute(hStdout,c.m_color);
	return i;
}

string getFileName(const string& s);   //����ļ����ļ���
string getPathName(const string &s);   //����ļ���·��
//string getPathName_linux(const string &s);   //����ļ���·��
string getFileExt(const string &s);//����ļ���չ��
string makenewfile(vector<string> stringVector,string filename) ;
  //��Ŀ¼�²����ļ���coutΪ1�������ļ�����Ŀ¼�µ���Ŀ¼Ϊ0�򲻰���
void FindAllFile(string strpath, vector<string> &allFile,int count) ;
int readFile(string strpath, string sourpath);    //��Ŀ¼�²����Ƿ���ͬ���ļ�
void copyFile(const string & src , const string & dest);         //�����ļ���ָ��Ŀ¼
int readMulu(string strpath, string sourpath) ;       //�鿴ָ��Ŀ¼�Ƿ�
void copymulu(string strpath, string destpath,int flag , int choose);//����Ŀ¼
void delFileMulu(string strpath, vector<string> &allFile);
//�ж�Ŀ¼path�Ƿ�Ϊcurrpath�ϲ�Ŀ¼  ������linux����·��
bool isUpPath_Linux(const string  &path , const string & currPath);

void FindFileInfo(string strpath, multimap<string,string> &allFile);
long FindFileSize(string strpath);
void FindFileName(string strpath,vector<string>&fileName);
void getFileOrFolder(string strpath,vector<string>&fileName , int choose);
int cmdKind(string cmd );//�ж�����Ĳ�������
string getFileName_win(const string& s);   //����ļ����ļ���
bool issysCmd(string cmd );//�ж�����Ĳ�������
bool  _sameLowerAndUp(string path);//�ж�·����Сд�Ƿ���ͬ����Ҫ����windows����·��
bool  sameLowerAndUp(string path);//�ж�·����Сд�Ƿ���ͬ����Ҫ����windows����·��
#endif