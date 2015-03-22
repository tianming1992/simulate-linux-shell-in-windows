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

void initSys(struct sysInfo *  psys);//��ʼ������
void logIn(  struct sysInfo *  pSys ) ;//��¼ģ��
string getCommand(struct sysInfo *  pSys ); //��ȡ�û�����
void  paraserCommand(struct sysInfo *  pSys  ,const string &cmd  );

void  commandQueue(struct sysInfo *  pSys); //ִ���������
 void exeu(struct sysInfo *  pSys);//ִ�е�������
 void outResult(struct sysInfo *  pSys);//��������� 

void  cmd_useradd( const struct command &currCmd ,struct sysInfo *  pSys);//ִ��useradd ����
void  cmd_passwd( const struct command &currCmd ,struct sysInfo *  pSys);//ִ��passwd ����
void  cmd_su( const struct command &currCmd ,struct sysInfo *  pSys);//ִ��su����
void  cmd_logout( const struct command &currCmd ,struct sysInfo *  pSys);//ִ��logout����
void  cmd_exit( const struct command &currCmd ,struct sysInfo *  pSys);//ִ��exit����
void  cmd_alias( const struct command &currCmd ,struct sysInfo *  pSys);//ִ��alias����
void  cmd_ls( const struct command &currCmd ,struct sysInfo *  pSys);//ִ��ls����
void  cmd_cat( const struct command &currCmd ,struct sysInfo *  pSys);//ִ��cat����
void  cmd_grep( const struct command &currCmd ,struct sysInfo *  pSys);//ִ��grep����
void  cmd_mkdir( const struct command &currCmd ,struct sysInfo *  pSys);//ִ��mkdir����
void  cmd_cd( const struct command &currCmd ,struct sysInfo *  pSys);//ִ��cd����
void  cmd_cp( const struct command &currCmd ,struct sysInfo *  pSys , int choose);//ִ��cp����
void  cmd_rm( const struct command &currCmd ,struct sysInfo *  pSys);//ִ��rm����
void  cmd_mv( const struct command &currCmd ,struct sysInfo *  pSys);//ִ��mv����
void  cmd_find( const struct command &currCmd ,struct sysInfo *  pSys);//ִ��find����



void  cp(command recommand , int choose );//cp�����һ���Ӻ���
void  rm(const string & path);//ɾ���ļ���·��
void FindFile(string strpath, struct sysInfo *  pSys, int count, const string & somename)     ;
void FindFile_Ls(string strpath, struct sysInfo *  pSys)   ;
void tabTips(struct sysInfo *  pSys  ,   string &cmd , int& tabtime ,const vector<string>&systemcmd);
bool myfunction (ex_out  e_i, ex_out e_j) ;
#endif  