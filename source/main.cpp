/*********************************************************************************************************************
/*�ļ���:  main.cpp
/*ģ����: ������ģ��
/*����:    ������&��Ǳ
/*����:    2011.10.25
/*����:    ������
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
  	            if (false == pSys->login)//�û�δ��¼
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
 		catch(string & s){                     //���������쳣
 			cout << s <<'\n'<<endl; 
 			while(!pSys->commandQuene.empty()) pSys->commandQuene.pop();//����������
 			pSys->execu_out.clear();//���������
 		}
    }
        delete pSys ;	
	   return   0;   
}