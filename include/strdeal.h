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


//���ļ������ַ�����ģ�庯��  ���ڱ�������֧�ֽ�ģ�庯����ʵ�ַֿ��ļ�д���ʷ�����.h��
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
bool isNumOrAlpha(const string &check);//���һ���ַ����Ƿ��Ϊ��������ĸ
string delPathTail_linux( string  path);//ɾ��·��ĩβб�ܣ��Ǹ�Ŀ¼��
void delPathTail_win(string & path);

bool isRelatePath_linux( string & path); //�ж��Ƿ�Ϊ���·��
string reToAbsPath_linux(  string  src , const string & currPath);//linux������·��ת����·�� 
string getLastPath_linux( string & src );//linux����ȡ���һ��Ŀ¼��
string getFatherPath_linux(const string & path);//ֻ�����ھ���·��

bool isValidstr(const string & str);
string getfatherDir( string  dir);
string fillstring(string strpth,int maxlength);
string inttostring(long int size);
string timechange(time_t time) ;

string maxfront( vector<string> strs ,  string front ,  vector<string> &haveFronts , int & amount );

#endif