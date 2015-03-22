#ifndef   _COMMAND_H_
#define    _COMMAND_H_

#include "sysInfo.h"
#include "userInfo.h"
#include <queue>
#include <list>
#include <stdlib.h>
#include "filedeal.h"
#include <sys/stat.h>
#include <sys/types.h>

//#include <windows.h>

using namespace std;

void initSys(struct sysInfo *  psys);//初始化程序
void logIn(  struct sysInfo *  pSys ) ;//登录模块
string getCommand(struct sysInfo *  pSys ); //获取用户命令
void  paraserCommand(struct sysInfo *  pSys  ,const string &cmd  );

void  commandQueue(struct sysInfo *  pSys); //执行命令队列
 void exeu(struct sysInfo *  pSys);//执行单个命令
 void outResult(struct sysInfo *  pSys);//输出程序结果 

void  cmd_useradd( const struct command &currCmd ,struct sysInfo *  pSys);//执行useradd 命令
void  cmd_passwd( const struct command &currCmd ,struct sysInfo *  pSys);//执行passwd 命令
void  cmd_su( const struct command &currCmd ,struct sysInfo *  pSys);//执行su命令
void  cmd_logout( const struct command &currCmd ,struct sysInfo *  pSys);//执行logout命令
void  cmd_exit( const struct command &currCmd ,struct sysInfo *  pSys);//执行exit命令
void  cmd_alias( const struct command &currCmd ,struct sysInfo *  pSys);//执行alias命令
void  cmd_ls( const struct command &currCmd ,struct sysInfo *  pSys);//执行ls命令
void  cmd_cat( const struct command &currCmd ,struct sysInfo *  pSys);//执行cat命令
void  cmd_grep( const struct command &currCmd ,struct sysInfo *  pSys);//执行grep命令
void  cmd_mkdir( const struct command &currCmd ,struct sysInfo *  pSys);//执行mkdir命令
void  cmd_cd( const struct command &currCmd ,struct sysInfo *  pSys);//执行cd命令
void  cmd_cp( const struct command &currCmd ,struct sysInfo *  pSys , int choose);//执行cp命令
void  cmd_rm( const struct command &currCmd ,struct sysInfo *  pSys);//执行rm命令
void  cmd_mv( const struct command &currCmd ,struct sysInfo *  pSys);//执行mv命令
void  cmd_find( const struct command &currCmd ,struct sysInfo *  pSys);//执行find命令



void  cp(command recommand , int choose );//cp命令的一个子函数
void  rm(const string & path);//删除文件或路径
void FindFile(string strpath, struct sysInfo *  pSys, int count, const string & somename)     ;
void FindFile_Ls(string strpath, struct sysInfo *  pSys)   ;
void tabTips(struct sysInfo *  pSys  ,   string &cmd , int& tabtime ,const vector<string>&systemcmd);
bool myfunction (ex_out  e_i, ex_out e_j) ;
#endif  