/*********************************************************************************************************************
/*文件名:  main.cpp
/*模块名: 主程序模块
/*作者:    邹天明&戴潜
/*日期:    2011.10.25
/*功能:    主程序。
/*
/*********************************************************************************************************************/


#include "encrytion.h"
#include "sysInfo.h"
#include "userInfo.h"
#include "strdeal.h"
#include "filedeal.h"
#include "command.h"
#include <iomanip>

using namespace std  ;

int main(int   argc,   char   **argv[])   
{   
    struct sysInfo *pSys = new struct sysInfo ;
 	 initSys(pSys);
     while(true)
    { 
 		try{
  	            if (false == pSys->login)//用户未登录
	           {    
                       logIn(pSys);
  	           }
 		     else
 	           {
                       string  cmd  = getCommand(pSys);
 				       paraserCommand(pSys ,cmd);
        			   commandQueue(pSys);
 			    }
 		   }
 		catch(string & s){                     //处理运行异常
 			cout << s <<'\n'<<endl; 
 			while(!pSys->commandQuene.empty()) pSys->commandQuene.pop();//清空命令队列
 			pSys->execu_out.clear();//清空输出结果
 		}
    }
        delete pSys ;	
	   return   0;   
}