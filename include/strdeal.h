#ifndef  _STRDEAL_H_
#define  _STRDEAL_H_

#include <string>
#include <iostream>
#include <conio.h>
#include <fstream>
#include <vector>
#include <io.h>
#include<ctype.h>
#include <sstream>
#include <ctime>
#include<queue>

using namespace std;

void   InputPassword(string & str) ;
void   InputUserName(string & str) ;


//从文件读出字符串，模板函数  由于编译器不支持将模板函数与实现分开文件写，故放置在.h中
template <typename T > 
void getStrFromFile(const string  &path , T& strs)
{
	ifstream  infile(path.c_str() , ios ::in);
	if (! infile)
	{ 
		throw(string("open file error ")) ;
	}
	char ch;
	string temp ;
	while(infile.get(ch))
	{
		if (ch == '\n')
		{ 
			strs.push_back(temp);
			temp.clear();
			continue;
		}
		else if (ch == '\r')
		{
			continue;
		}
		temp.insert(temp.end(),1,ch);
	}
	if (false == temp.empty())
	{
		strs.push_back(temp);
	}
	infile.close();
	return ;
}

template <typename T1 >
void putStrToFile(const string &path , T1& strs)
{
	ofstream  outfile(path.c_str() , ios ::out);
	if (! outfile)
	{ 
		throw(string("open file error ")) ;
	}
	for (T1::iterator i = strs.begin();i != strs.end() ; ++i)
	{
		outfile << (*i)<<'\n' ;
	}
	outfile.close() ;
	return ;
}



void delSpace(string &str);
string winToLinPath(const string &winPath);
string linToWinPath(const string &linuxPath);
bool getFirPar(const string &src ,  string & dest); 
bool isNumOrAlpha(const string &check);//检查一个字符串是否仅为数字与字母
string delPathTail_linux( string  path);//删除路径末尾斜杠（非根目录）
void delPathTail_win(string & path);

bool isRelatePath_linux( string & path); //判断是否为相对路径
string reToAbsPath_linux(  string  src , const string & currPath);//linux风格相对路径转绝对路径 
string getLastPath_linux( string & src );//linux风格获取最后一级目录名
string getFatherPath_linux(const string & path);//只能用于绝对路径

bool isValidstr(const string & str);
string getfatherDir( string  dir);
string fillstring(string strpth,int maxlength);
string inttostring(long int size);
string timechange(time_t time) ;

string maxfront( vector<string> strs ,  string front ,  vector<string> &haveFronts , int & amount );

#endif