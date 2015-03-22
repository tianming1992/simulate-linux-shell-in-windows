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
bool isPath(string strpath);//判断是否为目录
string cstrTostr(CString& csstr) ;//将cstring转换为string
CString strToCstr(string strTemp);//将string 转换为 cstring


//重载cout <<实现输出变色
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

string getFileName(const string& s);   //获得文件的文件名
string getPathName(const string &s);   //获得文件的路径
//string getPathName_linux(const string &s);   //获得文件的路径
string getFileExt(const string &s);//获得文件扩展名
string makenewfile(vector<string> stringVector,string filename) ;
  //在目录下查找文件，cout为1。查找文件包括目录下的子目录为0则不包括
void FindAllFile(string strpath, vector<string> &allFile,int count) ;
int readFile(string strpath, string sourpath);    //在目录下查找是否有同名文件
void copyFile(const string & src , const string & dest);         //复制文件到指定目录
int readMulu(string strpath, string sourpath) ;       //查看指定目录是否
void copymulu(string strpath, string destpath,int flag , int choose);//拷贝目录
void delFileMulu(string strpath, vector<string> &allFile);
//判断目录path是否为currpath上层目录  就用于linux绝对路径
bool isUpPath_Linux(const string  &path , const string & currPath);

void FindFileInfo(string strpath, multimap<string,string> &allFile);
long FindFileSize(string strpath);
void FindFileName(string strpath,vector<string>&fileName);
void getFileOrFolder(string strpath,vector<string>&fileName , int choose);
int cmdKind(string cmd );//判断命令的参数类型
string getFileName_win(const string& s);   //获得文件的文件名
bool issysCmd(string cmd );//判断命令的参数类型
bool  _sameLowerAndUp(string path);//判断路径大小写是否相同，需要传入windows绝对路径
bool  sameLowerAndUp(string path);//判断路径大小写是否相同，需要传入windows绝对路径
#endif