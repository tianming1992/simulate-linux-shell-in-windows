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
	string  comName ;  //命令名
	int  argc ;  //命令所带参数个数 
	vector<string> argv ;//命令参数数组 
};

struct ex_out
{
	string result ;//输出结果
    string keyword ; //搜索关键字
	char add_info ; //扩充的附加信息
};
struct sysInfo
{
	 struct currUserInfo curruser ; //当前用户
	 list<struct userInfo > sysUserInfo ; //系统所有用户基本信息 
	 string currPath ;//当前用户所处路径
	 //struct command currCommand ; // 当前执行命令
	 queue<struct command> commandQuene ;  //当前命令执行队列 
	 list<struct ex_out> execu_out ;//上一命令执行完毕后输出结果缓冲区
	 bool    login ;           //是否登录
	 int out_kind ; //程序输出类型
	 string other_info ; //可能的其他信息
};

void readUserList(  list<struct userInfo > &userList,  const  string &path); 
void writeUserList( list<struct userInfo > &userList ,const  string &path); 


#endif