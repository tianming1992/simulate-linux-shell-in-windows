/*********************************************************************************************************************
/*�ļ���:  command.cpp
/*ģ����: ������� & ����ִ��
/*����:    ������&��Ǳ
/*����:    2011.10.25
/*����:    ������Ҫ����,��ȡ�û������,�����û�������������ִ�ж��С�ͨ����ͬ����ִ�к���ִ�С�
/*
/*********************************************************************************************************************/

#include "command.h"


#define    USER_LIST_PATH    "C:\\tianmingseed\\data\\userlist.dat"
#define    USER_PATH   "C:\\tianmingseed\\data"
#define    USER_TEMP   "C:\\tianmingseed\\"
#define    UNKNOW_  1
#define    NONE_    2
#define    LS   3
#define    LS_R  4
#define    LINE_OUT   5 
#define   NEXT_CMD 6
#define   KEY_WORD 7
#define   LINE_OUT_FIND 8


/*********************************************************************************************************************
/*������:  initSys
/*����:  struct sysInfo *  psys
/*����ֵ:    void
/*ģ����: ��ʼ��ģ��
/*��������: �������к󣬳�ʼ���û����ݣ�ͬʱ����ϵͳ�������ļ�  
/*
/*********************************************************************************************************************/
void  initSys(struct sysInfo *  psys)
{  
	if(0 != _access(USER_PATH,0))
	{
		mkMassDir(USER_PATH);//��������û����ݵ������ļ�
		SetFileAttributes(TEXT(USER_TEMP) , FILE_ATTRIBUTE_HIDDEN);//�����ļ���Ϊ�����ļ���
	}
    //��ȡϵͳ�����û���Ϣ��
	 readUserList(psys->sysUserInfo , USER_LIST_PATH);
     psys->login  = false ;//�û�δ��¼
	 psys->out_kind = UNKNOW_;
}


/*********************************************************************************************************************
/*������: logIn
/*����:  struct sysInfo * pSys
/*����ֵ:    void
/*ģ����: ��¼ģ��
/*��������: ����û��������룬��½�ɹ����ȡ���û��ĸ���������Ϣ
/*
/*********************************************************************************************************************/
void logIn(  struct sysInfo * pSys )
{
     string  username ;
	 string password ;
	 cout<<"Username:" ;
	 InputUserName(username);
	 if (username == "guest")
	 {
		 cout<<"Login successfully! Welcome to the SeedCup shell!\n"<<endl;
		 pSys->curruser.username = username ;
		 pSys->login = true ;
		 pSys->currPath = "c:\\" ;
		 getMyHisCmd(pSys->curruser);//��ȡ���û���ʷ����
		 getMyAlias(pSys->curruser);//��ȡ���û�alias�ַ���
		 return ;
	 }
	 for (list<struct userInfo>::iterator it = pSys->sysUserInfo.begin(); it != pSys->sysUserInfo.end() ; it++)
	 {
		 if (strcmp((*it).username, username.c_str()) == 0)  //���ҵ����û���
		 {
			 cout<<"Password:";
			 InputPassword(password);
			 if (strcmp((*it).password , password.c_str()) == 0 ) //������ȷ
			 {
				 cout<<"Login successfully! Welcome to the SeedCup shell!\n"<<endl;//��½�ɹ�
				 pSys->curruser.username = username ;
				// pSys->curruser.password = password ;
				 pSys->login = true ;
				 pSys->currPath = "c:" ;
				 getMyHisCmd(pSys->curruser);//��ȡ���û���ʷ����
				 getMyAlias(pSys->curruser);//��ȡ���û�alias�ַ���
				 return ;
			 }
			 else{
				 throw(string("wrong password !")) ;   //�������
			 }   
		 }
	 }
	 throw(string("user do not exist !"));//�û�������
}


/*********************************************************************************************************************
/*������: getCommand
/*����:  struct sysInfo *  psys
/*����ֵ:   string 
/*ģ����: ȡ����ģ��
/*��������: ��ȡ�û�����������������ʷ���
/*
/*********************************************************************************************************************/
 string getCommand(struct sysInfo *  pSys ) 
 {  
	  string  cmd ;	
	  int tabTime = 0;
	  struct sysInfo tempSys;
	  vector<string>systemcmd ;
	  //ϵͳ����
	  char  syscmd [15][20]={"useradd" , "passwd" , "su" , "ls" , "cp","logout" , "mv","mkdir","cd","rm","alias","find","exit","cat"  ,"grep" } ;
	  for (int i = 0 ; i < 15 ;++i)
	  {
		  systemcmd.push_back(syscmd[i]);//��ȡϵͳ����
	  }
	  for (list<alias>::iterator  _it= pSys->curruser.myalias.begin() ; _it != pSys->curruser.myalias.end() ; _it ++)
	  {
		  systemcmd.push_back((*_it).subCmd);//��ȡ�Զ�������
	  }
	   string cmd_str;
	   int   ch = 0; 
	   bool out = true ;
	   list<string>::iterator  it= pSys->curruser.myHisCmd.end();
	 //  if(out == true )
	   cout<<pSys->curruser.username<<'@'<<"SeedCup"<<" "<<winToLinPath(pSys->currPath)<<" "<<'$'<<" ";
	  // if(out == false) out = true ;
	  cmd.clear();
	  while(true) 
	  { 
		  ch   =   _getch(); 

		  //�˸��
		  if(ch   ==   8  ) 
		  { 
			  if(cmd.empty()) continue;
			  cmd = cmd.erase(cmd.length()-1 , 1);
			  cout << "\b \b"; 
			  continue;
		  } 

		  //�������
		  else   if(ch   ==   0x0A   ||   ch   ==   0x0D) 
		  { 
			  cout<<endl;
			  break; 
		  } 

		  //��ȡ�����  
		  if ( ch == 224)
		  {
			  ch   =   _getch(); 
			  if (ch == 72)   //up������
			  {
				  //cout<<"\nup"<<endl;    //��ʷ�������
				  if(it == pSys->curruser.myHisCmd.begin()) continue ;
				  it -- ;
				  for (unsigned int i = 0 ; i < cmd.length() ; ++ i)
				  {
                      cout << "\b \b"; 
				  }
				  cmd.clear();
				  cmd = (*it);
				  cout << (*it);
				  continue; 
			  } 
			  if (ch == 80)   //down������
			  {
				 // cout<<"\ndown"<<endl;  //��ʷ�������
				  if(it == pSys->curruser.myHisCmd.end()) continue ;
				  it ++ ;
                  if(it == pSys->curruser.myHisCmd.end())
				  {
					  for (size_t i = 0 ; i < cmd.length() ; ++ i)
					  {
						  cout << "\b \b"; 
					  }//�ص��������
                       cmd.clear();
					   tabTime = 0;
					  continue;
				  }
				  for (size_t i = 0 ; i < cmd.length() ; ++ i)
				  {
					  cout << "\b \b"; 
				  }
				  cmd.clear();
				  cmd = (*it);
				  cout << (*it);
				  tabTime = 0;
				  continue; 
			  }
			  continue;
		  }
		  //��ȡTAB��
         if(ch == 9)
		 {      
			   
                 tabTips(pSys  , cmd , tabTime ,systemcmd);
				 continue;            
		 }
	
		  cmd+= (char)ch;
		  cout <<(char)ch; 
		  tabTime = 0;
	  }   
  
	   if(cmd.empty()) throw(string(""));
	  //�����������ʷ���
	  pSys->curruser.myHisCmd.push_back(cmd);
	  if (pSys->curruser.myHisCmd.size() > 10)
	  {
		  pSys->curruser.myHisCmd.pop_front();  //ɾ����ǰ�������
	  }
	  saveMyHisCmd(pSys->curruser);
     
	  return cmd ;
}


/*********************************************************************************************************************
/*������:  tabTips
/*����:  struct sysInfo *  pSys  ,   string &cmd , int& tabtime ,const vector<string>&systemcmd
/*����ֵ:    void
/*ģ����:������ʾģ��
/*��������: �ļ�����Ŀ¼�������������ʾ 
/*
/*********************************************************************************************************************/
void tabTips(struct sysInfo *  pSys  ,   string &cmd , int& tabtime ,const vector<string>&systemcmd)
{ 
	 struct sysInfo tempSys ;//��ʱ�����������ʱ����
	 tempSys.curruser.myalias = pSys->curruser.myalias ;
	  command tempcmd ;//�������ĵ�ǰ����
	  string front , maxFront ;//��Ҫ������ǰ׺�ͷ��ص���󹫹�ǰ׺
	  vector<string>results ; //���������Ľ��
	  int amount = 0 ;//����ƥ����ĸ���
	  //maxfront( vector<string> strs ,  string front ,  vector<string> &haveFronts , int & amount );
	
	 //�����ģ��
	 string cmd_str = cmd ;
	 delSpace(  cmd_str );
	 if (true ==  cmd_str.empty() || cmd_str.at(cmd_str.length() -1) == ';')//û����������
	 {
		 for (size_t i = 0 ; i < cmd.length() ; ++ i)
		 {
			 cout << "\b \b"; 
		 }
		 string prin ;
		 int i =0 ;
		 cout<<'\n';
		 for (vector<string>::const_iterator it = systemcmd.begin()  ;  it != systemcmd.end() ; it++ )
		 {   
			 i++ ;
			 prin = (*it);
			 prin = fillstring(prin , 25);//�����������
			 cout<<prin ;
			 if ((i %2 ) == 0 ) cout<<'\n'; //����
		 }
		 if(systemcmd.size() %2 !=0) cout<<'\n';
		
		 cout<<pSys->curruser.username<<'@'<<"SeedCup"<<" "<<winToLinPath(pSys->currPath)<<" "<<'$'<<" ";
		 cout<<cmd;
		 return ;
	 }
	   paraserCommand(&tempSys ,  cmd);//�����Ѿ����������
	   if(!tempSys.commandQuene.empty())
	   {
		   tempcmd = tempSys.commandQuene.back();//��ȡ������������
	   }
	 //�����������������Ϊ0 �����������ĩβ�޿ո� ���ҷ�����ϵͳ�����򲹳�����
	 if(tempcmd.argv.empty()  && cmd[cmd.length() -1] != ' ' &&false == issysCmd(tempcmd.comName))
	 {
          front = tempcmd.comName ;//������Ϊ�Ѿ�����������ַ�
		  maxFront = maxfront(systemcmd , front , results , amount);//����������ǰ��
          if(amount == 0) return ;
          if (amount == 1 && maxFront == front) return ;//�������Ϊһ���Ҷ�����ͬ
		  if (amount == 1 && maxFront != front)//�������Ϊһ���Ҷ�������ͬ �򲹳�
		  {
			  maxFront.erase(0 , front.length());
			  cmd = cmd + maxFront ;
			  cout<<maxFront ;
			  return ;
		  }
          if (amount > 1 )//���������Ŀ����һ
          {
			  if(maxFront != front)//���ǰ׺��������ǰ׺��ͬ
			  {
				  maxFront.erase(0 , front.length()); //�������ǰ׺
				  cmd = cmd + maxFront ;
				  cout<<maxFront ;       
				   ;//��¼tab�������ڶ���ʱ������п��ܽ��
				  return ;
			  }
			  if (maxFront == front && tabtime == 0)//���ǰ׺��������ǰ׺��ͬ �����ǵ�һ�ΰ�tab
			  {
				  tabtime = 1;
				  return ;
			  }
			  if (maxFront == front && tabtime == 1)//���ǰ׺��������ǰ׺��ͬ �����ǵڶ��ΰ�tab��������п��ܽ��
			  {   
				  string prin ;
				  int i =0 ;
				  cout<<'\n';
				  for (vector<string>::iterator it = results.begin()  ;  it != results.end() ; it++ )
				  {   
					      i++ ;
                          prin = (*it);
						  prin = fillstring(prin , 25);
						  cout<<prin ;
						  if ((i %2 ) == 0 ) cout<<'\n'; //����
				  }
				  if(results.size() %2 !=0) cout<<'\n';
				  cout<<pSys->curruser.username<<'@'<<"SeedCup"<<" "<<winToLinPath(pSys->currPath)<<" "<<'$'<<" "<<cmd;
			       return ;
			  }
          }
      }
       //·������ģ��
	 //���������������������Ϊ�գ� �����������ĩβ�пո� ������������п�����
       if( (!tempcmd.comName.empty()) &&
		  ( cmd[cmd.length() -1] == ' ' || 
		  true == issysCmd(tempcmd.comName)&&cmd[cmd.length() -1] != ' '&&tempcmd.argc==0))
	   {
		   string path  = pSys->currPath ;
		   delPathTail_win(path) ;//Ŀ¼Ϊ��ǰĿ¼
		    vector<string> names ; //�������п��ܵ�Ŀ¼���ļ���
           if(FILE_AND_FOLDER == cmdKind(tempcmd.comName))//�ж�·���������� �˴�Ϊ�ļ���Ŀ¼
		       getFileOrFolder(path , names ,  FILE_AND_FOLDER);//��ȡ���п��ܵ�Ŀ¼���ļ���
		   if(FOLDER_ONLY == cmdKind(tempcmd.comName))//�ж�·���������� �˴�ΪĿ¼
			   getFileOrFolder(path , names ,  FOLDER_ONLY);//��ȡ���п��ܵ�Ŀ¼
             
			  if(true == names.empty() ) return ;//���Ϊ��
			  if (names.size() == 1 && cmd[cmd.length() -1] == ' ')
			  {
				  cmd = cmd + names[0];
				  cout<<names[0];
				  return ;
			  }
			  if (names.size() == 1 && cmd[cmd.length() -1] != ' ')
			  {
				  cmd = cmd +' '+ names[0];
				  cout<<" "<<names[0];
				  return ;
			  }
			   string prin ;
			   int i =0 ;
			   cout<<'\n';
			   for (vector<string>::iterator it = names.begin()  ;  it != names.end() ; it++ )
			   {   
				   i++ ;
				   prin = (*it);
				   prin = fillstring(prin , 25);
				   if((*it).at((*it).length() -1) == '/') cout<<blue<<prin<<white ;
				   else cout<<prin ;
				   if ((i %2 ) == 0 ) cout<<'\n'; //����
			   }
			   if(names.size() %2 !=0) cout<<'\n';
			   cout<<pSys->curruser.username<<'@'<<"SeedCup"<<" "<<winToLinPath(pSys->currPath)<<" "<<'$'<<" "<<cmd;
			   return ;
		  }
          //���������Ϊ0 �� ����cmdĩβ�޿ո����ȡĿ¼������Ŀ
	      if( ( tempcmd.argc != 0  && !tempcmd.argv.empty() &&cmd[cmd.length() -1] != ' '))
		  {
              string path = tempcmd.argv[tempcmd.argc - 1]; //�����һ��������Ϊ·����������
			  string fatherPath ;	
			   vector<string> names ; //�������п��ܵ�Ŀ¼���ļ���
               
			  path = delPathTail_linux(path);//ת��Ϊ��׼·����ʽ
			  path = reToAbsPath_linux(path , winToLinPath(pSys->currPath));//��pathת��Ϊlinux����·��
			  fatherPath = getFatherPath_linux(path);//��ȡ��linux��Ŀ¼
			  if (tempcmd.argv[tempcmd.argc -1].at(tempcmd.argv[tempcmd.argc-1].length() -1) == '/')
			  {
				  fatherPath = path ;
			  }
			  path = linToWinPath(path); //��pathת��Ϊwindows·��
              fatherPath = linToWinPath(fatherPath);
              if(0 != _access(fatherPath.c_str() , 0)) return ; //���·�������ڣ�����
			  if(FILE_AND_FOLDER == cmdKind(tempcmd.comName))//�ж�·���������� �˴�Ϊ�ļ���Ŀ¼
				  getFileOrFolder(fatherPath , names ,  FILE_AND_FOLDER);//��ȡ���п��ܵ�Ŀ¼���ļ���
			  if(FOLDER_ONLY == cmdKind(tempcmd.comName))//�ж�·���������� �˴�ΪĿ¼
				  getFileOrFolder(fatherPath , names ,  FOLDER_ONLY);//��ȡ���п��ܵ�Ŀ¼
			    if(true == names.empty() ) return ;//���Ϊ��
             
				if (path == fatherPath ) front.clear();
			  else front = getFileName_win(path);
			  if (true == front.empty())
			  {  
				  if (names.size() == 1)//�������Ϊһ��ֱ�Ӳ���
				  {
					  cmd = cmd + names[0];
					  cout<<names[0];
					  return ;
				  }
				  string prin ;
				  int i =0 ;
				  cout<<'\n';
				  for (vector<string>::iterator it = names.begin()  ;  it != names.end() ; it++ )
				  {   
					  i++ ;
					  prin = (*it);
					  prin = fillstring(prin , 25);
					  if((*it).at((*it).length() -1) == '/') cout<<blue<<prin<<white ;
					  else cout<<prin ;
					  if ((i %2 ) == 0 ) cout<<'\n'; //����
				  }
				  if(names.size() %2 !=0) cout<<'\n';
				  cout<<pSys->curruser.username<<'@'<<"SeedCup"<<" "<<winToLinPath(pSys->currPath)<<" "<<'$'<<" "<<cmd;
				  return ;
			  }
			  else
			  {
				  maxFront = maxfront(names , front , results , amount);//����������ǰ��
				  if(amount == 0) return ;
				  if (amount == 1 && maxFront == front) return ;//�������Ϊһ���Ҷ�����ͬ
				  if (amount == 1 && maxFront != front)//�������Ϊһ���Ҷ�������ͬ �򲹳�
				  {
					  maxFront.erase(0 , front.length());
					  cmd = cmd + maxFront ;
					  cout<<maxFront ;
					  return ;
				  }
				  if (amount > 1 )//���������Ŀ����һ
				  {
					  if(maxFront != front)//���ǰ׺��������ǰ׺��ͬ
					  {
						  maxFront.erase(0 , front.length()); //�������ǰ׺
						  cmd = cmd + maxFront ;
						  cout<<maxFront ;       
						  //��¼tab�������ڶ���ʱ������п��ܽ��
						  return ;
					  }
					  if (maxFront == front && tabtime == 0)//���ǰ׺��������ǰ׺��ͬ �����ǵ�һ�ΰ�tab
					  {
						  tabtime = 1;
						  return ;
					  }
					  if (maxFront == front && tabtime == 1)//���ǰ׺��������ǰ׺��ͬ �����ǵڶ��ΰ�tab��������п��ܽ��
					  {   
						  string prin ;
						  int i =0 ;
						  cout<<'\n';
						  for (vector<string>::iterator it = results.begin()  ;  it != results.end() ; it++ )
						  {   
							  i++ ;
							  prin = (*it);
							  prin = fillstring(prin , 25);
							  if((*it).at((*it).length() -1) == '/') cout<<blue<<prin<<white ;
							  else cout<<prin ;
							  if ((i %2 ) == 0 ) cout<<'\n'; //����
						  }
						  if(results.size() %2 !=0) cout<<'\n';
						  cout<<pSys->curruser.username<<'@'<<"SeedCup"<<" "<<winToLinPath(pSys->currPath)<<" "<<'$'<<" "<<cmd;
						  return ;
					  }
			      }
              }
          }
}


/*********************************************************************************************************************
/*������:  paraserCommand
/*����: struct sysInfo *  pSys  ,const string &cmd
/*����ֵ:    void
/*ģ����: �������ģ��
/*��������: �����û���������� 
/*
/*********************************************************************************************************************/
void  paraserCommand(struct sysInfo *  pSys  ,const string &cmd  )
{
	queue<string >tempCmds ;
	string _cmd = cmd ;
	unsigned int i = 0;
	string temp ;
	temp.clear();
	if(cmd.empty()) throw(string(""));
	

	  delSpace(_cmd);
	  while(i < _cmd.length())
	  {
		  if ( _cmd[i] == '|')  //�ܵ�����
		  {
			   if(false == temp.empty())  tempCmds.push(temp);
			  tempCmds.push("|");
			  temp.clear();
			  i ++ ;
			  continue;
		  }
		
		  if ( _cmd[i] == '>')  //�ض�������
		  {
              tempCmds.push(temp);
              temp.clear();
			  temp.push_back('>');
              temp.push_back(' ');
			  while (++i < _cmd.length())
			  {
				  if(_cmd[i] != ' ') break ;  //���˿ո�
			  }
			  if (i == _cmd.length())
			  {
				  throw(string("cmd > must have a paramater !"));   //�ض�������޲��� ����
			  }
              while(_cmd[i ] != ' '  && _cmd[i ] != '|' && _cmd[i ] != ';'&& i < _cmd.length())
				  temp.push_back(_cmd[i++]);
			  tempCmds.push(temp);
			  temp.clear();
			  continue;
		  }
		  if ( _cmd[i] == ';')  //������������
		  {
			  if(false == temp.empty())  tempCmds.push(temp);
			  temp.clear();
			  i++ ;
			  continue;
		  }
		
		   
		  temp.push_back(_cmd[i]);
		  ++i; 
		  if(i ==_cmd.length())   //�����������������ָ�������ʱ��ӱ�������
		  {
			  if(false == temp.empty())  tempCmds.push(temp);
			  break; 
		  }
	  }

	  string tempCmd ;
	  struct  command tagCmd ;
      tagCmd.argc =0 ;
      
	  while(false == tempCmds.empty())   //���ָ�Ĳ�ͬ�������������
	  {
		 
          tempCmd = tempCmds.front();
		  tagCmd.argc =0 ;
		  tagCmd.argv.clear();
		  tagCmd.comName.clear();
		  tempCmds.pop();
		  delSpace(tempCmd); //ɾ���ո�
          getFirPar(tempCmd,temp);//��ȡ��һ������
		   //���Ҹ��û���Ч�ַ������滻
		  vector<string> recycle ; //�����Ч������ѭ������
		  recycle.clear() ;
		  for (list<struct alias >::iterator it = pSys->curruser.myalias.begin(); it !=  pSys->curruser.myalias.end();)                                
		  {   
			  if(0 == strcmp( (*it).subCmd , temp.c_str()) )//�ҵ�������Ϊ�Զ���������ȡ��ԭ��
			  {
				  tempCmd.erase(0 , temp.length()); //ɾ���������Զ�����
				  string  ss = (*it).sysCmd ;
				  tempCmd = ss + tempCmd ;     //��ԭ�����������������
				  temp = ss ;
				  recycle.push_back((*it).subCmd);//�ж��Ƿ���ѭ��
				  //�ҵ���ĳ���Զ�������ԭ�����һ�ε��Զ�������ͬ����ʾ��ѭ��
				  if (false == recycle.empty() &&  0 == strcmp( recycle[0].c_str() , temp.c_str()) )
				  {   
					  vector<string>::iterator pStr;
					  bool move = true ;
					  for ( pStr = recycle.begin() ; pStr != recycle.end() ; pStr++)
					  {
						  for (it = pSys->curruser.myalias.begin(); it !=  pSys->curruser.myalias.end();)  
						  {
							  if(0 == strcmp( (*it).subCmd , (*pStr).c_str() ))//�ҵ�������ѭ���Ľڵ�
							  {
								  list<struct alias >::iterator pT = it;
								  it++ ;
								  move = false;
								  pSys->curruser.myalias.erase(pT);//ɾ���ýڵ�
							  }
							  if(move)++ it ;
							  else move = true;
						  }
					  }
					  throw(string("a dead recycle in alias cmds has been erased!"));
				  }
				  it =  pSys->curruser.myalias.begin();
				  continue;
			  }
			  it++;
		  }
          delSpace(tempCmd); //ɾ���ո�
		  getFirPar(tempCmd,temp);//���»�ȡ��һ������
		  tagCmd.comName = temp ;
		 
		  if (temp == "ls")  //ls����·�����԰����ո񣬱Ƚ�����
		  {
			  i = tempCmd.find("ls");
			  tempCmd.erase(i,2);
			  delSpace(tempCmd);
			  if (string::npos != (i = tempCmd.find("-l"))) //���и��Ӳ���
			  {
				  tagCmd.argc += 1 ;
				  tagCmd.argv.push_back("-l");
				  i = tempCmd.find("-l");
				  tempCmd.erase(i,2);
				  delSpace(tempCmd);
				  if(true == tempCmd.empty())   //·��Ϊ��
				  {
					  pSys->commandQuene.push(tagCmd);
					  continue;
				  }
				  else
				  {
					  tagCmd.argc += 1;
					  tagCmd.argv.push_back(tempCmd);
					  pSys->commandQuene.push(tagCmd);
					  continue;
				  }
			  }
			 else
			 {                      //δ�����Ӳ���
				  if(true == tempCmd.empty())   //·��Ϊ��
				  {
					  pSys->commandQuene.push(tagCmd);
					  continue;
				  }
				  else
				  {
					  tagCmd.argc += 1;
					  tagCmd.argv.push_back(tempCmd);
					  pSys->commandQuene.push(tagCmd);
					  continue;
				  }
			  }
		  }

		  if (temp == "alias") //alias������ʽ�Ƚ�����
		  {
			  i = tempCmd.find("alias");
			  tempCmd.erase(i,5);
			  delSpace(tempCmd);
			  if(true == tempCmd.empty()) throw(string("cmd alias paramaters can not be empty")) ;
			  i = tempCmd.find("=");
			  if(string :: npos == i ) throw(string("wrong paraters of alias , should have  = ")) ;
              temp = tempCmd.substr(0 , i);
			  if(temp.empty())  throw(string("cmd alias paramaters can not be empty at left of =")) ;
			  delSpace(temp);
			  tagCmd.argc +=1 ;
              tagCmd.argv.push_back(temp);
              tempCmd.erase( 0 , i +1);
			  delSpace(tempCmd);
			  if (tempCmd.empty()) throw(string("cmd alias paramaters can not be empty at right of =")) ;
			  if(tempCmd[0] != '\'' || tempCmd[tempCmd.length() -1] != '\'')
				  throw(string("wrong paraters of alias  at right of =")) ;
			  tempCmd.erase(0,1);
			  tempCmd.erase(tempCmd.length()-1,1);
			  delSpace(tempCmd);
			  if(tempCmd.empty()) throw(string("cmd alias paramaters can not be empty at right of =")) ;
			  tagCmd.argc +=1 ;
			  tagCmd.argv.push_back(tempCmd);
			  pSys->commandQuene.push(tagCmd);
			  continue;
		  }
		  
		  i = tempCmd.find(temp);
		  tempCmd.erase(i,temp.length());
		  delSpace(tempCmd);  //ɾ������ո�
           while(!tempCmd.empty())
		  {
              getFirPar(tempCmd,temp);
			  tagCmd.argc += 1 ;
			  tagCmd.argv.push_back(temp);
			  i = tempCmd.find(temp);
			  tempCmd.erase(i,temp.length());
			  delSpace(tempCmd);  
		  }
		   pSys->commandQuene.push(tagCmd);
	  }
}


/*********************************************************************************************************************
/*������: commandQueue
/*����:  struct sysInfo *  psys
/*����ֵ:    void
/*ģ����: ����ִ��ģ��
/*��������: ִ���������
/*
/*********************************************************************************************************************/
void  commandQueue(struct sysInfo *  pSys) 
{  
	  while(!pSys->commandQuene.empty())
	  { 
		  exeu(pSys); //ִ������
		  outResult(pSys);//���������
	  }
	  pSys->execu_out.clear();
	  pSys->out_kind = UNKNOW_ ;
	  return;
}


/*********************************************************************************************************************
/*������:exeu
/*����:  struct sysInfo *  psys
/*����ֵ:    void
/*ģ����: ����ִ��ģ��
/*��������: ֱ��ִ�и�������
/*
/*********************************************************************************************************************/
void exeu(struct sysInfo *  pSys)
{
      struct command currCmd = pSys->commandQuene.front();//��ȡҪִ�е�����
	  pSys->commandQuene.pop();   //ɾ����ǰ��ִ������

	  if(currCmd.comName == "useradd") {  cmd_useradd(currCmd,pSys);    return ; } //ִ��useradd����
	  if(currCmd.comName == "passwd")  {  cmd_passwd(currCmd, pSys);    return ; } //ִ��passwd����
	  if(currCmd.comName == "su")  {  cmd_su(currCmd, pSys);    return ; } //ִ��su����
	  if(currCmd.comName == "logout")  {  cmd_logout(currCmd, pSys);    return ; } //ִ��logout����
	  if(currCmd.comName == "exit")  {  cmd_exit(currCmd, pSys);    return ; } //ִ��exit����
	  if(currCmd.comName == "alias")  {  cmd_alias(currCmd, pSys);    return ; } //ִ��exit����
	  if(currCmd.comName == "cat")  {  cmd_cat(currCmd, pSys);    return ; } //ִ��cat����
	   if(currCmd.comName == "grep")  {  cmd_grep(currCmd, pSys);    return ; } //ִ��grep����
       if(currCmd.comName == "mkdir")  {  cmd_mkdir(currCmd, pSys);    return ; } //ִ��mkdir����
       if(currCmd.comName == "cd")  {  cmd_cd(currCmd, pSys);    return ; } //ִ��cd����
	   if(currCmd.comName == "cp") {  try{cmd_cp(currCmd, pSys , CP_);}catch(int &e){};  return ; } //ִ��cp����
	   if(currCmd.comName == "find")  {  cmd_find(currCmd, pSys);    return ; } //ִ��find����
	   if(currCmd.comName == "rm")  {  cmd_rm(currCmd, pSys);    return ; } //ִ��rm����
	   if(currCmd.comName == "mv")  {  cmd_mv(currCmd, pSys);    return ; } //ִ��mv����
	   if(currCmd.comName == "ls")  {  cmd_ls(currCmd, pSys);    return ; } //ִ��ls����
	  throw(string("invalid  cmd !"));//�Ƿ������
	  return ; 
}


/*********************************************************************************************************************
/*������: outResult
/*����:  struct sysInfo *  psys
/*����ֵ:    void
/*ģ����: ����ִ��ģ��
/*��������: ��������ִ�к�ķ���ֵ
/*
/*********************************************************************************************************************/
void outResult(struct sysInfo *  pSys)
{   
	string path;
	if (!pSys->commandQuene.empty() &&pSys->commandQuene.front().comName == "|") //����ܵ�����
	{
		pSys->out_kind =  NEXT_CMD ;
		pSys->commandQuene.pop();
		return ;
	}
    if (!pSys->commandQuene.empty() && pSys->commandQuene.front().comName == ">")//��������ض���
    {   
		struct command currcmd = pSys->commandQuene.front();
		int temKind = pSys->out_kind ;
		pSys->out_kind =  NONE_ ;//������
		pSys->commandQuene.pop();
		if(currcmd.argc != 1) throw(string("wrong parameters of redirect command !"));
        delPathTail_linux(currcmd.argv[0]);
	    path= linToWinPath(reToAbsPath_linux(currcmd.argv[0] , winToLinPath(pSys->currPath))) ;
		if (0 == _access(path.c_str() , 0) && false == isPath(path))//����ļ����ڣ���ʾ�Ƿ񸲸�
		{
			cout<<"file exists already , Cover it (Y / N) ?";
			char choose ;
			cin.clear() ;cin>>choose ;cin.clear();
			if (!(choose == 'y' || choose == 'Y')) 
			return;
		}
		ofstream  outfile(path.c_str() , ios ::out);
		if (! outfile)
		{ 
			throw(string("open file error !")) ;
		}
		if(temKind == LS_R)
			outfile<<"Name                "<<"Size(KB)            "<<"Last modified       "<<"Type\n";
		while(!pSys->execu_out.empty())	
		{
			outfile << pSys->execu_out.front().result<<'\n' ;
			pSys->execu_out.pop_front();
		}
		outfile.close() ;
		return ;
    }

	if (pSys->out_kind == LINE_OUT) //�������ӡ����Ļ
	{
		while(!pSys->execu_out.empty())	
		{
			cout << pSys->execu_out.front().result<<'\n' ;
			pSys->execu_out.pop_front();
		}
		cout<<'\n';
        pSys->out_kind =  NONE_ ;//������
		return ;
	}
	if (pSys->out_kind == LS) //LS����������ӡ����Ļ
	{   
		int i =0 ;
		while( ! pSys->execu_out.empty())	
		{ 
			pSys->execu_out.front().result = fillstring(pSys->execu_out.front().result , 20);//����ַ���
			if(pSys->execu_out.front().add_info == I_AM_DIRECTORY)
				cout<<blue<<pSys->execu_out.front().result<<white ;
			else
				cout<<pSys->execu_out.front().result;
			pSys->execu_out.pop_front();
			++i ;
	     }
		if(i%4 != 0) cout<<"\n\n";
		else cout<<"\n";
		pSys->out_kind =  NONE_ ;//������
		return ;
	}
	if (pSys->out_kind == LS_R)
	{
		cout<<"Name                "<<"Size(KB)            "<<"Last modified       "<<"Type"<<endl;
		while(!pSys->execu_out.empty())
		{ 
		   pSys->execu_out.front().keyword = fillstring(pSys->execu_out.front().keyword , 20);//����ַ���
			if(pSys->execu_out.front().add_info == I_AM_DIRECTORY)
			{  
				pSys->execu_out.front().result.erase(0 , 20);
				cout<<blue<<pSys->execu_out.front().keyword<<white<<pSys->execu_out.front().result<<white <<"\n";
			}
			else
		      cout<<pSys->execu_out.front().result<<"\n";
			pSys->execu_out.pop_front();
		}
		cout<<'\n';
		pSys->out_kind =  NONE_ ;//������
		return ;
	}

	if (pSys->out_kind == LINE_OUT_FIND) //FIND����������ӡ����Ļ
	{   
		string key , line ;
		size_t i ;
		while(!pSys->execu_out.empty())	
		{  
			
			key = pSys->execu_out.front().keyword;
			line=  pSys->execu_out.front().result;
			i = line.rfind(key);
			line.erase(i, line.length());
			cout<<white<<line<<red<<key<<white<<'\n';
			pSys->execu_out.pop_front();
		}
		cout<<'\n';
		pSys->out_kind =  NONE_ ;//������
		return ;
	}
    
	if (pSys->out_kind == KEY_WORD)//�ؼ��ָ�����ӡ
	{
		while(!pSys->execu_out.empty())	
		{
			string temp = pSys->execu_out.front().result;
			string key = pSys->other_info;
			size_t i =0 , j = temp.find(key);
			while(j != string::npos)
			{
				cout<<white<<temp.substr(i , j -i)<<red<<key<<white; //��ɫ������ʾ�ؼ���
				i = j = j + key.length();
				j = temp.find(key , j);
				if (j == string ::npos)
				{
                     cout<<white<<temp.substr(i , temp.length());
				}
			}
		    cout<<endl;
			pSys->execu_out.pop_front();
		}
		cout<<'\n';
		pSys->out_kind =  NONE_ ;//������
		pSys->other_info.clear();
		return ;
	}
    return ;
}


/*********************************************************************************************************************
/*������:  cmd_useradd
/*����: const struct command &currcmd , struct sysInfo *  pSys
/*����ֵ:    void
/*ģ����:�û��ж�ģ��
/*��������: ִ��useradd �������½��û�
/*
/*********************************************************************************************************************/
void cmd_useradd( const struct command &currcmd , struct sysInfo *  pSys)
{
	 pSys->out_kind = NONE_ ;//��������������
	 pSys->execu_out.clear();//���Ϊ��

	 if(currcmd.argc == 0)throw(string("cmd of useradd should have a parameter !")) ;
     if(currcmd.argc > 1 )throw(string("cmd of useradd  have too many parameter !")) ;
	 string userName = currcmd.argv[0];
	 //������ƺϷ���
	 if(userName == "guest") throw(string("this name is invalid!")) ;
	 if (false == isNumOrAlpha(userName)) 
		 throw(string("name  can just have numbers and letters !")) ;
	 if( !(userName.length() > 0 && userName.length() < 11) )
          throw(string("the length of name   must be longer than 0 and lesser than 11 !")) ;
	
	 //�жϸ��û����Ƿ��Ѵ���
	 for( list<struct userInfo >::iterator it = pSys->sysUserInfo.begin(); it != pSys->sysUserInfo.end(); ++it)
	 {
          if((*it).username == userName)
			   throw(string("this user name exists already !")) ;
	 }
    
	 string pw1 , pw2 ;
	 cout<<"password:";
	 InputPassword(pw1);
      //�������Ϸ���
	 if (false == isNumOrAlpha(pw1)) 
		 throw(string("password  can just have numbers and letters !")) ;
	 if( !(pw1.length() > 5 && pw1.length() < 21) )
		 throw(string("the length of password   must be longer than 5 and lesser than 21 !")) ;
     //�ٴ���������
	 cout<<"password again:";
	 InputPassword(pw2);
        if(pw1 != pw2) throw(string("two password is not the same "));

		struct userInfo newUser ;
         strcpy_s(newUser.username, userName.c_str());
		 strcpy_s(newUser.password , pw1.c_str());
		 pSys->sysUserInfo.push_back(newUser);
		 writeUserList(pSys->sysUserInfo,USER_LIST_PATH);//д���ļ�
		
		 cout<<userName<<" had been created successfully!\n\n" ;
	     return ;
}


/*********************************************************************************************************************
/*������:  cmd_passwd
/*����: const struct command &currcmd ,struct sysInfo *  pSys
/*����ֵ:    void
/*ģ����: �û��ж�ģ��
/*��������: ִ��passwd �������Ӧ�û����������Ƿ���ȷ
/*
/*********************************************************************************************************************/
void  cmd_passwd( const struct command &currcmd ,struct sysInfo *  pSys)
{
	if(currcmd.argc == 0)throw(string("cmd of passwd should have a parameter !")) ;
	if(currcmd.argc > 1 )throw(string("cmd of passwd  have too many parameter !")) ;
	string userName = currcmd.argv[0]; 
    
   if (userName == "guest") throw(string("guest user do not have password !"));//guest�û�û������
	//�жϸ��û����Ƿ����
	 list<struct userInfo >::iterator it ;
	for(  it = pSys->sysUserInfo.begin(); it != pSys->sysUserInfo.end(); ++it){
		if((*it).username == userName)   break ;  }
    if(it == pSys->sysUserInfo.end()) throw(string("this user do not exists !"));

    string oldpw , newpw1 , newpw2 ;
	cout<<"old password:";
	InputPassword(oldpw);
	if(oldpw != (*it).password) throw(string("password error !")) ;
	cout<<"new password:";
    InputPassword(newpw1);
	//�������Ϸ���
	if (false == isNumOrAlpha(newpw1)) 
		throw(string("password  can just have numbers and letters !")) ;
	if( !(newpw1.length() > 5 && newpw1.length() < 21) )
		throw(string("the length of password   must be longer than 5 and lesser than 21 !")) ;
	cout<<"new password again:";
	InputPassword(newpw2);
	if(newpw1 != newpw2) throw(string("two new password is not the same ! "));

	strcpy_s((*it).password , newpw1.c_str());
	writeUserList(pSys->sysUserInfo,USER_LIST_PATH);
	
	pSys->out_kind = NONE_ ;  //����������Ϊ��
	pSys->execu_out.clear();
    
	cout<<"password of  "<<(*it).username<<" changed successfully!\n\n";
	return ;
}


/*********************************************************************************************************************
/*������:  cmd_su
/*����: const struct command &currcmd ,struct sysInfo *  pSys
/*����ֵ:    void
/*ģ����: �л��û�ģ��
/*��������: �½��û�
/*
/*********************************************************************************************************************/
void  cmd_su( const struct command &currcmd ,struct sysInfo *  pSys)
{
	if(currcmd.argc == 0)throw(string("cmd of su should have a parameter !")) ;
	if(currcmd.argc > 1 )throw(string("cmd of su  have too many parameter !")) ;
    string name = currcmd.argv[0];

	if(name == pSys->curruser.username) throw(string("same to current user!"));//�뵱ǰ�û���ͬ
	//�жϸ��û����Ƿ����
	list<struct userInfo >::iterator it ;
	for(  it = pSys->sysUserInfo.begin(); it != pSys->sysUserInfo.end(); ++it){
		if((*it).username == name)   break ;  }

	if(it == pSys->sysUserInfo.end() && name != "guest") throw(string("this user do not exists !"));//�û�������
  
	if (name != "guest")//��guest�û���Ҫ��������
   {
	   string pw1 ;
	   cout<<"password:";
	   InputPassword(pw1);
	   if(pw1 != (*it).password) throw(string("password error!"));
   }
	
    pSys->currPath = "C:";//�ص���Ŀ¼
	pSys->curruser.username = name ;
	pSys->curruser.myalias.clear();
	pSys->curruser.myHisCmd.clear();
	getMyHisCmd(pSys->curruser);//��ȡ���û���ʷ����
    getMyAlias(pSys->curruser);   //��ȡ���û�alias�ַ���
	cout<<"user changed successfully!\n\n";
	pSys->out_kind = NONE_ ;
	pSys->execu_out.clear();
	while(!pSys->commandQuene.empty()) pSys->commandQuene.pop();//���ڿ���Ȩת����������������
	return;
} 


/*********************************************************************************************************************
/*������:  cmd_logout
/*����: const struct command &currcmd ,struct sysInfo *  pSys
/*����ֵ:    void
/*ģ����: �û��ǳ�ģ��
/*��������: �û��ǳ�����
/*
/*********************************************************************************************************************/
void  cmd_logout( const struct command &currcmd ,struct sysInfo *  pSys)
{
   if(currcmd.argc > 0 )throw(string("cmd of logout  have too many parameter !")) ;//������ʽ����
   
   while(!pSys->commandQuene.empty()) pSys->commandQuene.pop();//����������
   pSys->execu_out.clear();//���������
   pSys->login = false ;       //�ǳ�
   pSys->out_kind = UNKNOW_ ;  //��յ�ǰ�û���Ϣ
   pSys->curruser.myalias.clear();
   pSys->curruser.myHisCmd.clear();
   pSys->curruser.username.clear();
   cout<<"\n";
   return;
}


/*********************************************************************************************************************
/*������:cmd_exit
/*����: const struct command &currcmd ,struct sysInfo *  pSys
/*����ֵ:    void
/*ģ����:�������ģ��
/*��������: ִ��exit���� �˳�����
/*
/*********************************************************************************************************************/
void  cmd_exit( const struct command &currcmd ,struct sysInfo *  pSys)
{  
     if(currcmd.argc > 0 )throw(string("cmd of exit  have too many parameter !")) ;//������ʽ����
	 delete pSys ;
	 exit(0);
}


/*********************************************************************************************************************
/*������: cmd_alias
/*����: const struct command &currcmd ,struct sysInfo *  pSys
/*����ֵ:    void
/*ģ����: aliasģ��
/*��������: ִ��alias��������Զ�������������û�alias�ַ���
/*
/*********************************************************************************************************************/
void  cmd_alias( const struct command &currcmd ,struct sysInfo *  pSys)
{
    if(currcmd.argc != 2 )throw(string("cmd of alias  have take two parameters !")) ;//������ʽ����
	string mycmd = currcmd.argv[0];
	string syscmd = currcmd.argv[1];
    list<struct alias >::iterator it;

	for ( it = pSys->curruser.myalias.begin(); it !=  pSys->curruser.myalias.end() ;++it)                                
	{
	    if ( 0 == strcmp( (*it).subCmd , mycmd.c_str()) ) //�������Զ������Ѵ��ڣ���ʾ�Ƿ񸲸�
	    {
			cout<<"The alias of "<<"\""<<mycmd<<"\""<<"already exsits , cover it ? <Y/N>";
			char choose;
			cin >> choose;
			if('y' == choose || 'Y' == choose) 
			{
				pSys->curruser.myalias.erase(it);//�ͷ�ԭ���Զ���
				break;
			}
			else  throw(string(""));
	    }
	}
		struct alias  tempAlias ;   //�����Զ�������������û�alias�ַ���
		strcpy_s(tempAlias.subCmd,mycmd.c_str());
		strcpy_s(tempAlias.sysCmd , syscmd.c_str());
        pSys->curruser.myalias.push_back(tempAlias);
		saveMyAlias(pSys->curruser);//д���ļ�
        
		cout<<"The alias has been set successfully!\n\n";
		pSys->out_kind = NONE_ ;  //�ó����������Ϊ��
		pSys->execu_out.clear();
}


/*********************************************************************************************************************
/*������:  myfunction 
/*����: ex_out  e_i, ex_out e_j
/*����ֵ:   bool 
/*ģ����: ��ʼ��ģ��
/*��������: �ж���С
/*
/*********************************************************************************************************************/
bool myfunction (ex_out  e_i, ex_out e_j) 
{ 
	return (e_i.keyword < e_j.keyword);
}


/*********************************************************************************************************************
/*������:  FindFile_Ls
/*����: string strpath, struct sysInfo *  pSys , bool getInfo
/*����ֵ:    void
/*ģ����: �ļ�����ģ��
/*��������: ִ��ls����ʱ��Ѱ�Ҷ�ӦĿ¼���ļ� 
/*
/*********************************************************************************************************************/
void FindFile_Ls(string strpath, struct sysInfo *  pSys , bool getInfo)   
{
	CFileFind finder;
	//struct stat fileInfo;
	string theFileName;
	string theFilePath;
	CString path = strToCstr(strpath);
	struct ex_out temp ;
	BOOL working = finder.FindFile(path + "\\*.*");     //�Ƿ�����ļ���Ŀ¼������Ϊ1
	while (working)//���keyword��add_info
	{
		working = finder.FindNextFile();
		if (finder.IsDots())            //��ǰ�ҵ���������
			continue;
		if (finder.IsDirectory())        //��ǰ�ҵ�����Ŀ¼
		{   
			  DWORD isHidden=GetFileAttributes(finder.GetFilePath()); //���������ļ���
			  if ( !(isHidden & FILE_ATTRIBUTE_HIDDEN))
			  {
				  temp.keyword= cstrTostr(finder.GetFileName());
				  temp.add_info = I_AM_DIRECTORY ;
				  pSys->execu_out.push_back(temp);
			  }
		 } 
		else               //��ǰ�ҵ������ļ�
		{   
			  DWORD isHidden=GetFileAttributes(finder.GetFilePath()); //���������ļ�
			  if ( !(isHidden & FILE_ATTRIBUTE_HIDDEN))
			  {
				  temp.keyword  =cstrTostr( finder.GetFileName());     //��ȡ�ļ���
		          temp.add_info = I_AM_FILE ;
		          pSys->execu_out.push_back(temp);
			  }
		}
	}
	if (false == getInfo)
	{
		for (list<struct ex_out>::iterator it = pSys->execu_out.begin() ; it != pSys->execu_out.end() ; ++it)
		{
			(*it).result = (*it).keyword ; //���û�ȡ�ļ���Ϣ
		}
		pSys->execu_out.sort(myfunction);
		return ;
	}
	else
	{
		multimap<string,string>aFile;
		FindFileInfo(strpath ,aFile);            //��ȡ��Ŀ¼�������ļ���Ŀ¼����Ϣ
		 multimap<string,string>::iterator it_map = aFile.begin() ; 
		 list<struct ex_out>::iterator it = pSys->execu_out.begin() ;
		 while(it_map != aFile.end())
		 {
			  for (it = pSys->execu_out.begin() ; it != pSys->execu_out.end() ; it ++)
			  {
				  if((*it).keyword == (*it_map).first)
				  {
					  (*it).result = (*it_map).second;
				       break;
				  }
			  }
			   it_map++;
		 }
	}
	pSys->execu_out.sort(myfunction);
}


/*********************************************************************************************************************
/*������: cmd_ls
/*����: const struct command &currcmd ,struct sysInfo *  pSys
/*����ֵ:    void
/*ģ����: ��ʼ��ģ��
/*��������: ִ��ls����
/*
/*********************************************************************************************************************/
void  cmd_ls( const struct command &currcmd ,struct sysInfo *  pSys)
{
      if(currcmd.argc > 2 )throw(string("cmd of cat  have error parameter !")) ;//������ʽ����
      string win_path ;	 
	  if (currcmd.argc ==0)//û�в���
	  {  
		   win_path = pSys->currPath; //�޲���ʱĿ¼Ϊ��ǰĿ¼
           FindFile_Ls(win_path , pSys , false);
		   pSys->out_kind = LS ;//�������������Ϊ�����
	  }
	  if (currcmd.argc == 1 && currcmd.argv[0] == "-l")
	  {
		  win_path = pSys->currPath; //��·������ʱĿ¼Ϊ��ǰĿ¼
		  FindFile_Ls(win_path , pSys , true);
		  pSys->out_kind = LS_R ;//�������������Ϊ�����
	  }
	  if (currcmd.argc == 1 && currcmd.argv[0] != "-l")
	  {
		  win_path = linToWinPath(reToAbsPath_linux(delPathTail_linux(currcmd.argv[0]), winToLinPath(pSys->currPath)) ); //
		  if(_access(win_path.c_str() , 0) != 0 || false == sameLowerAndUp(win_path) || false == isPath(win_path)) 
			  throw(string("invalid path!"));
		  FindFile_Ls(win_path , pSys , false);
		  pSys->out_kind = LS ;//�������������Ϊ�����
	  }
	  if (currcmd.argc == 2)
	  {
		  if(currcmd.argv[0] != "-l") throw(string("error args of this cmd!"));
		  win_path = linToWinPath(reToAbsPath_linux(delPathTail_linux(currcmd.argv[1]), winToLinPath(pSys->currPath)) ); //
		  if(_access(win_path.c_str() , 0) != 0 || false == sameLowerAndUp(win_path) || false == isPath(win_path)) 
			  throw(string("invalid path!"));
		  FindFile_Ls(win_path , pSys , true);
		  pSys->out_kind = LS_R ;//�������������Ϊ�����
	  }
     return ;
}


/*********************************************************************************************************************
/*������:  cmd_cat
/*����:  const struct command &currcmd ,struct sysInfo *  pSys
/*����ֵ:    void
/*ģ����: ����ģ��
/*��������: ִ��cat����  
/*
/*********************************************************************************************************************/
void  cmd_cat( const struct command &currcmd ,struct sysInfo *  pSys)
{  
	struct ex_out  temResult ;
	temResult.add_info = 0;
	string path;
	pSys->execu_out.clear();
    if(currcmd.argc == 0 || currcmd.argc > 2 )throw(string("cmd of cat  have error parameter !")) ;//������ʽ����
	if (currcmd.argc == 1) //��1�����������ļ�·��
	{   
        if(currcmd.argv[0] == "-n")throw(string("wrong parameter of cat !"));
		path = delPathTail_linux(currcmd.argv[0]);
        path= linToWinPath(reToAbsPath_linux(path , winToLinPath(pSys->currPath))) ;
	}
	else
	{
		if(currcmd.argv[0] != "-n")throw(string("wrong parameter of cat !"));
		path = currcmd.argv[1];
		path = delPathTail_linux(path);
		 path= linToWinPath(reToAbsPath_linux(path , winToLinPath(pSys->currPath))) ;
	}
	if (0 != _access(path.c_str() , 0) || false == sameLowerAndUp(path) || true == isPath(path))
	{
		throw(string("the file do not exists!"));
	}
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
				if(currcmd.argc == 1)   temResult.result = temResult.keyword =temp ;
                if (currcmd.argc == 2)
                {  
					char buffer[10];
					int i = pSys->execu_out.size() + 1;
					string line = _itoa(i , buffer , 16);
					while(line.length() != 4) { line = '0'+line ;}
                    temResult.keyword =temp ;
					temResult.result = line +':'+"  "+temp ;
                }
				pSys->execu_out.push_back(temResult);
				temResult.result.clear();
				temResult.keyword.clear();
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
			if(currcmd.argc == 1)   temResult.result = temResult.keyword =temp ;
			if (currcmd.argc == 2)
			{  
				char buffer[10];
				int i = pSys->execu_out.size() + 1;
				string line = _itoa(i , buffer , 16);
				while(line.length() != 4) { line = '0'+line ;}
				temResult.keyword =temp ;
				temResult.result = line +':'+"  "+temp ;
			}
			pSys->execu_out.push_back(temResult);
		}
		infile.close();

		pSys->out_kind = LINE_OUT ;//�������������Ϊ�����
		return ;
}


/*********************************************************************************************************************
/*������:  cmd_grep
/*����: const struct command &currcmd ,struct sysInfo *  pSys
/*����ֵ:    void
/*ģ����: ����ģ��
/*��������:ִ��grep����
/*
/*********************************************************************************************************************/
void  cmd_grep( const struct command &currcmd ,struct sysInfo *  pSys)
{
     if(currcmd.argc != 1 )throw(string("cmd of cat  have error parameter !")) ;//������ʽ����
     if(false == isNumOrAlpha(currcmd.argv[0]) )throw(string("the parameter has invalid character !")) ;//������ʽ����
	 if(pSys->out_kind != NEXT_CMD)throw(string("there should be a cmd before the command !"));

	 for (list<ex_out>::iterator it = pSys->execu_out.begin() ; it != pSys->execu_out.end() ;)
     {
		 if (string::npos == (*it).result.find(currcmd.argv[0]))//���ҹؼ��֣����û�У�������Ŀ�ӽ����ɾ��
         {  
             list<ex_out>::iterator del = it ; 
			 it ++ ;
             pSys->execu_out.erase(del);
			 continue;
         }
		 ++it;
	 }
     pSys->other_info = currcmd.argv[0] ;//��¼�ؼ���
	 pSys->out_kind = KEY_WORD ;//�������������Ϊ�ؼ������ģʽ
	 return ;
} 


/*********************************************************************************************************************
/*������:  cmd_mkdir
/*����:  const struct command &currcmd ,struct sysInfo *  pSys
/*����ֵ:    void
/*ģ����: ����ģ��
/*��������: ִ��mkdir����
/*
/*********************************************************************************************************************/
void  cmd_mkdir( const struct command &currcmd ,struct sysInfo *  pSys)
{    
      if(currcmd.argc == 0 || currcmd.argc > 2 )
		  throw(string("cmd of mkdir  have error parameter !")) ;//������ʽ����
      if(currcmd.argc == 2 && currcmd.argv[0] != "-p") 
		  throw(string("cmd of mkdir  have error parameter !")) ;//������ʽ����
	  if((currcmd.argc == 1 && currcmd.argv[0] == "/") || (currcmd.argc == 2 && currcmd.argv[1] == "/"))
           throw(string("can not establish this path  !")) ;
	  if((currcmd.argc == 1 && delPathTail_linux(currcmd.argv[0]).at(delPathTail_linux(currcmd.argv[0]).length()-1)== '/')
		  || (currcmd.argc == 2 &&  delPathTail_linux(currcmd.argv[1]).at(delPathTail_linux(currcmd.argv[1]).length()-1) == '/'))
		  throw(string("invalid path name   !")) ;
      if (currcmd.argc == 1 &&
		  ( true == isValidstr(getLastPath_linux( reToAbsPath_linux(delPathTail_linux(currcmd.argv[0]), winToLinPath(pSys->currPath) ) ))
		  || currcmd.argv[0].length() > 255))
		  throw(string("cmd of mkdir  fail to work!")) ;//������ʽ����
	  if (currcmd.argc == 2 &&
		  ( true == isValidstr(getLastPath_linux( reToAbsPath_linux(delPathTail_linux(currcmd.argv[1]), winToLinPath(pSys->currPath) ) ))
		  || currcmd.argv[0].length() > 255))
		  throw(string("cmd of mkdir  fail to work !")) ;//������ʽ����
      string path ;
	  if (currcmd.argc ==1)
	  {
		  path = currcmd.argv[0];
		  path =delPathTail_linux(path);
		 path= linToWinPath(reToAbsPath_linux(path , winToLinPath(pSys->currPath))) ;
		 if(0 == _access(path.c_str() , 0))throw(string("this path exists already !"));//·���Ѵ��ڱ���
		 if(0 != _access(getfatherDir(path).c_str(), 0))throw(string("can not  establish this path  !"));//��·�������ڱ���
		 mkSingleDir(path);
		 pSys->out_kind = NONE_ ;  //�������������Ϊ��
		 pSys->execu_out.clear();
		 return ;
	  }
	  if (currcmd.argc ==2)
	  {   
		  path = currcmd.argv[1];
		  path = delPathTail_linux(path);
		  path= linToWinPath(reToAbsPath_linux(path , winToLinPath(pSys->currPath))) ;
		  if(0 == _access(path.c_str() , 0))throw(string("this path exists already !"));//·���Ѵ��ڱ���
		  mkMassDir(path);
		  pSys->out_kind = NONE_ ;  //�������������Ϊ��
		  pSys->execu_out.clear();
		  return ;
	  }
	  return ;
}


/*********************************************************************************************************************
/*������:  cmd_cd
/*����: const struct command &currcmd ,struct sysInfo *  pSys
/*����ֵ:    void
/*ģ����: ����ģ��
/*��������: ִ��cd����
/*
/*********************************************************************************************************************/
void  cmd_cd( const struct command &currcmd ,struct sysInfo *  pSys)
{
	if(currcmd.argc > 1)throw(string("too many paraters of the cd cmd"));
    if (currcmd.argc == 0)
    {
		pSys->currPath = "c:"; //����Ϊ�գ��ص���Ŀ¼
		pSys->out_kind = NONE_ ;  //�������������Ϊ��
		pSys->execu_out.clear();
        return ;
    }
	if (currcmd.argc == 1 && currcmd.argv[0] == "/")
	{
		pSys->currPath = "c:"; //����Ϊ/���ص���Ŀ¼
		pSys->out_kind = NONE_ ;  //�������������Ϊ��
		pSys->execu_out.clear();
		return ;
	}
	string path = currcmd.argv[0]; //��ȡ����Ŀ¼
    path = delPathTail_linux(path);//��Ŀ¼ת��Ϊ��׼��ʽ
	path = reToAbsPath_linux(path,winToLinPath(pSys->currPath));//��·��ת��Ϊ����·��
	path = linToWinPath(path); //��·����ʽת��Ϊwindows��ʽ
	if(0 != _access(path.c_str() , 0))//·���Ƿ����
		throw(string("this is a invalid path !"));
	if (false == sameLowerAndUp(path))
	    throw(string("this is a invalid path !"));
   
	if(false == isPath(path))//·���Ƿ�ΪĿ¼
	 {
		 throw(string("this is not a directory !"));//is   not directory
	 } 

    pSys->currPath = path ;
	pSys->out_kind = NONE_ ;
	pSys->execu_out.clear();
	return ;
}


/*********************************************************************************************************************
/*������:  cp
/*����: command recommand , int choose
/*����ֵ:    void
/*ģ����: ����ģ��
/*��������: �����ļ���Ŀ¼
/*
/*********************************************************************************************************************/
void  cp(command recommand , int choose )
{
	vector<string>::iterator first,second,third;
	int choice;
	FILE *fp;
	vector<string> nameVector;
	string destpath;
	string lastfilename;
	if (recommand.argc == 2)
	{
		first=recommand.argv.begin();
		second=recommand.argv.end()-1;
	}
	if (recommand.argc == 3)
	{
		first=recommand.argv.begin() + 1;
		second=recommand.argv.end()-1 ;
	}
	
	if ((!isPath(*first))&&isPath((*second)))
	{
		if (readFile(*second,*first))
		{ 
			if (choose == CP_)
			{
				cout<<"1.  �滻"<<endl;
				cout<<"2.  ȡ������"<<endl;
				cout<<"3.  ���Ƶ����滻   ";
			}
			else{
				cout<<"1.  �滻"<<endl;
				cout<<"2.  ȡ���ƶ�"<<endl;
				cout<<"3.  �ƶ������滻   ";
			}
			
			cin>>choice;
			if (choice==1)
			{
				destpath=(*second)+"\\"+getFileName(*first);
				if(*first == destpath ) return ;
				if ((fp=fopen(destpath.c_str(),"wt++"))==NULL)
				{
					throw(string("open file error !"));
				}
				fclose(fp);
				copyFile(*first,destpath);
				return;
			}
			if (choice==3)    
			{
				FindAllFile(*second,nameVector,0);
				lastfilename=makenewfile(nameVector,getFileName(*first));
				destpath=*second+"\\"+lastfilename;
				if ((fp=fopen(destpath.c_str(),"wt++"))==NULL)
				{
					throw(string("open file error !"));
				}
				copyFile(*first,destpath) ;
				fclose(fp);
				return;
			} 
			if(choose == 2) throw(choice);//ѡ2ȡ������
		}
		else
		{
			destpath=*second+"\\"+getFileName(*first);
			if ((fp=fopen(destpath.c_str(),"wt++"))==NULL)
			{
				throw(string("open file error !"));
			}
			copyFile(*first,destpath) ;
			fclose(fp);
		} 
	}
	if ((!isPath(*first))&&(!isPath((*second))))
	{
		copyFile(*first,*second) ;
	}
	if (isPath(*first)&&isPath(*second))
	{
		if (recommand.argc==3)
		{   
			if (*first == *second)
			{ 
				return ;
			}
			if (readMulu(*first,*second))
			{  
				if (choose == CP_)
				{
					cout<<"1. �ϲ�Ŀ¼"<<endl;
					cout<<"2. ȡ������   ";
				}
				else{
					cout<<"1. �ϲ�Ŀ¼"<<endl;
					cout<<"2. ȡ���ƶ�   ";
				}
				
				cin>>choice;
				if (choice !=1 )
				{
					throw(choice);
				}
				if (choice==1)
				{
					copymulu(*first,*second,0 ,choose);
				}
			}
			else
			{
				copymulu(*first,*second,1 ,choose);
			}
		}
		else
		{
			throw(string("can not copy directory without args!"));
		}
   }
}


/*********************************************************************************************************************
/*������:  rm
/*����:  const string & path
/*����ֵ:    void
/*ģ����: Ŀ¼ģ��
/*��������: ɾ��Ŀ¼���ļ� 
/*
/*********************************************************************************************************************/
void  rm(const string & path )
{
	
	int size,j;
	vector<string>stringVector; 
	delFileMulu(path ,stringVector);
	size=stringVector.size();
	for(j=0; j<size;j++)
	{
		rmdir((stringVector[j]).c_str());
	}
}


/*********************************************************************************************************************
/*������:   cmd_cp
/*����: const struct command &currcmd ,struct sysInfo *  pSys, int choose
/*����ֵ:    void
/*ģ����: ����ģ��
/*��������:ִ��cp����
/*
/*********************************************************************************************************************/
void  cmd_cp( const struct command &currcmd ,struct sysInfo *  pSys, int choose)
{ 
	struct command tempcmd   ;
  if(currcmd.argc < 2 || currcmd.argc > 3)throw(string("wrong parameters of the  cmd"));
   if (currcmd.argc == 2)//ֻ����������
   {
	   try{ 
	   tempcmd.argc = 2;
	   tempcmd.argv.push_back(linToWinPath(reToAbsPath_linux(delPathTail_linux(currcmd.argv[0]), winToLinPath(pSys->currPath)) ));
	   tempcmd.argv.push_back(linToWinPath(reToAbsPath_linux(delPathTail_linux(currcmd.argv[1]), winToLinPath(pSys->currPath) )));
	   if( _access(tempcmd.argv[0].c_str() , 0) != 0 || false ==sameLowerAndUp(tempcmd.argv[0]) )
		   throw(string("src path is invalid !"));//�ж�Դ·���Ƿ���Ч
	   if (true == isPath(tempcmd.argv[0]))//���Դ·����Ŀ¼�����ɿ���
	   {
		   throw(string("can not do this cmd to  directory without args!"));
	   }
	  if( _access(tempcmd.argv[1].c_str() , 0) == 0 && false == isPath(tempcmd.argv[1]))//���Ŀ��·�����ڲ��Ҳ���Ŀ¼
	   {           
		   //�Ƚ�Դ�ļ����Ƶ���ʱ�ļ���
           string filename  = USER_TEMP +  getFileName(tempcmd.argv[1]);//��ȡ��ʱĿ���ļ���
		   copyFile(tempcmd.argv[0] , filename );//���ļ���ֵ����ʱ�ļ���
		   tempcmd.argv[0] = filename ;
		   tempcmd.argv[1] = getPathName(tempcmd.argv[1]);//��ȡĿ���ļ����ڵ�Ŀ¼
		   cp(tempcmd,choose);
		   remove( filename.c_str() );//ɾ����ʱ�ļ�
		   pSys->out_kind = NONE_ ;  //�������������Ϊ��
		   pSys->execu_out.clear();
		   return ;
        }
	  cp(tempcmd , choose);}catch(int & e){
		  pSys->out_kind = NONE_ ;  //�������������Ϊ��
		  pSys->execu_out.clear();
		  throw;
	  };
	   pSys->out_kind = NONE_ ;  //�������������Ϊ��
	   pSys->execu_out.clear();
	   return ;
    }
   if (currcmd.argc == 3)//��3������
   {   
	   try{
	   if(currcmd.argv[0] != "-r") throw(string(" unknow args of  cp !"));
	 
	   if(currcmd.argv[1] == "/")throw(string("can not do this cmd to root directory  !"));//��Ŀ¼���ɿ���
	   tempcmd.argc = 3;
	   tempcmd.argv.push_back("-r");
	   tempcmd.argv.push_back(linToWinPath(reToAbsPath_linux(delPathTail_linux(currcmd.argv[1]), winToLinPath(pSys->currPath) )));
	   tempcmd.argv.push_back(linToWinPath(reToAbsPath_linux(delPathTail_linux(currcmd.argv[2]), winToLinPath(pSys->currPath) )));
	   if( _access(tempcmd.argv[1].c_str() , 0) != 0 || false ==sameLowerAndUp(tempcmd.argv[1]) )
		   throw(string("src path is invalid !"));//�ж�Դ·���Ƿ���Ч
	   if(0==_access(tempcmd.argv[1].c_str() ,0) && 0 == _access(tempcmd.argv[2].c_str() ,0) &&//��ԭ·��ΪĿ¼��Ŀ��·��Ϊ��Ŀ¼����
		   true == isPath(tempcmd.argv[1] )&& false == isPath(tempcmd.argv[2]))
		   throw(string("can not copy or move a directory to file!"));
	   //��ԭ·��ΪĿ¼��Ŀ��·�������ڣ�����
	   if(0==_access(tempcmd.argv[1].c_str() ,0) &&true == isPath(tempcmd.argv[1] )&& 
		   (0 != _access(tempcmd.argv[2].c_str() , 0 ) || false == sameLowerAndUp(tempcmd.argv[2])))
		   throw(string("dest directory do not exists!"));

	   if( _access(tempcmd.argv[2].c_str() , 0) == 0 && false == isPath(tempcmd.argv[2]))//���Ŀ��·�����ڲ��Ҳ���Ŀ¼
	   {           
		   //�Ƚ�Դ�ļ����Ƶ���ʱ�ļ���
		   string filename  = USER_TEMP +  getFileName(tempcmd.argv[1]);//��ȡ��ʱĿ���ļ���
		   copyFile(tempcmd.argv[1] , filename );//���ļ���ֵ����ʱ�ļ���
		   tempcmd.argv[1] = filename ;
		   tempcmd.argv[2] = getPathName(tempcmd.argv[2]);//��ȡĿ���ļ����ڵ�Ŀ¼
		   cp(tempcmd,choose);
		   remove( filename.c_str() );//ɾ����ʱ�ļ�
		   pSys->out_kind = NONE_ ;  //�������������Ϊ��
		   pSys->execu_out.clear();
		   return ;
	   }
	   cp(tempcmd ,choose);}catch(int & e){
		   pSys->out_kind = NONE_ ;  //�������������Ϊ��
		   pSys->execu_out.clear();
		   throw;
	   };
	   pSys->out_kind = NONE_ ;  //�������������Ϊ��
	   pSys->execu_out.clear();
	   return ;
   }
}


/*********************************************************************************************************************
/*������:  cmd_rm
/*����: const struct command &currCmd ,struct sysInfo *  pSys
/*����ֵ:    void
/*ģ����: Ŀ¼ɾ��ģ��
/*��������: ִ��rm���� 
/*
/*********************************************************************************************************************/
void  cmd_rm( const struct command &currCmd ,struct sysInfo *  pSys)//ִ��rm����
{
     if(currCmd.argc == 0 || currCmd.argc > 2) throw(string("cmd parameters error !"));//��������
     if(currCmd.argc == 2 && currCmd.argv[0] != "-r") throw(string("cmd args error ! need -r to delete directory"));//���Ӳ�������
	 
	 string win_path ;	
	 if(currCmd.argc == 1) //���windows��ʽ·��
	 {
		 win_path = linToWinPath(  reToAbsPath_linux(delPathTail_linux(currCmd.argv[0]), winToLinPath(pSys->currPath) )); 
		 if(true == isUpPath_Linux(  reToAbsPath_linux(delPathTail_linux(currCmd.argv[0]), winToLinPath(pSys->currPath) ) ,winToLinPath(pSys->currPath)) )
		 throw(string("can not remove this path !"));
		if( "/" == reToAbsPath_linux(delPathTail_linux(currCmd.argv[0]), winToLinPath(pSys->currPath) ))
         throw(string("can not remove root directory !"));//��Ŀ¼����ɾ��
	 }
	 if(currCmd.argc == 2)
	 {
        win_path = linToWinPath(reToAbsPath_linux(delPathTail_linux(currCmd.argv[1]), winToLinPath(pSys->currPath) )); 
		if(true == isUpPath_Linux(reToAbsPath_linux(delPathTail_linux(currCmd.argv[1]), winToLinPath(pSys->currPath) ) ,winToLinPath(pSys->currPath)))
		throw(string("can not remove this path !"));
		if( "/" == reToAbsPath_linux(delPathTail_linux(currCmd.argv[1]), winToLinPath(pSys->currPath) ))
			throw(string("can not remove root directory !"));//��Ŀ¼����ɾ��
	 }
	 if(0 != _access(win_path.c_str() , 0))//·�������ڣ�����
		 throw(string("the path you input do not exists !"));
	 if(false == sameLowerAndUp(win_path))  throw(string("the path you input do not exists !")); //��Сд��ͬ������
	 if(pSys->currPath == win_path)throw(string("can not delete current directory!"));
	 if(currCmd.argc == 1 &&  true == isPath(win_path))
		 throw(string("can not delete directory without args !"));//δ�Ӹ��Ӳ�����ͼɾ��Ŀ¼
	 if(true == isPath(win_path))   rm(win_path);//�����Ŀ¼��ɾ��Ŀ¼
	 else  remove(win_path.c_str());	//���ļ���ɾ���ļ�
	 pSys->out_kind = NONE_ ;  //�������������Ϊ��
	 pSys->execu_out.clear();
	 return ;
}


/*********************************************************************************************************************
/*������:  cmd_mv
/*����:  const struct command &currcmd ,struct sysInfo *  pSys
/*����ֵ:    void
/*ģ����: �ļ�����ģ��
/*��������:ִ��mv���� 
/*
/*********************************************************************************************************************/
void  cmd_mv( const struct command &currcmd ,struct sysInfo *  pSys)//ִ��mv����
{
	struct command tempcmd   ;
	string srcPath ,destPath;
	if(currcmd.argc < 2 || currcmd.argc > 3)//������ʽ���
		throw(string("wrong parameters of the this cmd"));
	if(currcmd.argc == 3 && currcmd.argv[0] != "-r")
        throw(string("wrong parameters of the this cmd"));
	if(currcmd.argc == 2)
	{//��ȡԴ·��
	  srcPath = linToWinPath(reToAbsPath_linux(delPathTail_linux(currcmd.argv[0]), winToLinPath(pSys->currPath)) );
      destPath = linToWinPath(reToAbsPath_linux(delPathTail_linux(currcmd.argv[1]), winToLinPath(pSys->currPath)) );
	} 
	if(currcmd.argc == 3)
	{
		srcPath = linToWinPath(reToAbsPath_linux(delPathTail_linux(currcmd.argv[1]), winToLinPath(pSys->currPath)) );
		 destPath = linToWinPath(reToAbsPath_linux(delPathTail_linux(currcmd.argv[2]), winToLinPath(pSys->currPath)) );
	}
	if (_access(srcPath.c_str() , 0) != 0 || false == sameLowerAndUp(srcPath))
	    throw(string("source path do not exists!"));//Դ·��������
	if (srcPath == destPath) return ; //Դ·����Ŀ��·����ͬ��ʲô������
  if (0 == _access(destPath.c_str() , 0) && true == isPath(destPath))//Ŀ��·�����Ѵ��ڵ�Ŀ¼
  {
	  string paDir = getfatherDir( srcPath);
	  delPathTail_win(paDir);
	  if (paDir == destPath)
	  {
		  pSys->out_kind = NONE_ ;  //�������������Ϊ��
		  pSys->execu_out.clear();
		   return ;  //���Դ·���ĸ�Ŀ¼��Ŀ��·����ͬ��������������ƶ���������ֱ�ӷ���
	  }
  }
  try{
	  cmd_cp(currcmd,pSys , MV_);
     if (true == isPath(srcPath))
	    rm(srcPath);//ɾ��Դ·��
	else  remove(srcPath.c_str());//ɾ��Դ�ļ�
  }catch(int & e){  } ;
	pSys->out_kind = NONE_ ;  //�������������Ϊ��
	pSys->execu_out.clear();
	return ;
}


/*********************************************************************************************************************
/*������: FindFile
/*����: string strpath, struct sysInfo *  pSys, int count , const string & somename
/*����ֵ:    void
/*ģ����: �ļ�����
/*��������: ��Ŀ¼�²����ļ���coutΪ1�����ļ�����Ŀ¼�µ���Ŀ¼,Ϊ0�򲻰��� somenameΪ���˹ؼ���
/*
/*********************************************************************************************************************/


void FindFile(string strpath, struct sysInfo *  pSys, int count , const string & somename)   
{
	CFileFind finder;
//	struct stat fileInfo;
	string theFileName;
	string theFilePath;
	CString path = strToCstr(strpath);
	 struct ex_out temp ;
	BOOL working = finder.FindFile(path + "\\*.*");     //�Ƿ�����ļ���Ŀ¼������Ϊ1
	while (working)
	{
		working = finder.FindNextFile();
		if (finder.IsDots())            //��ǰ�ҵ���������
			continue;
		if (finder.IsDirectory())        //��ǰ�ҵ�������Ŀ¼
		{
			if (count==1)
			{   
				 temp.result = winToLinPath(cstrTostr(finder.GetFilePath()));
				 temp.keyword=winToLinPath(getFileName(cstrTostr(finder.GetFilePath())));
				 if(true == somename.empty()) pSys->execu_out.push_back(temp);//����ؼ��ֲ���Ϊ�գ��������н��
				 else if(temp.keyword ==somename) pSys->execu_out.push_back(temp);//����ֻ������ؼ���ƥ��Ľ��
				 FindFile(cstrTostr(finder.GetFilePath()) ,  pSys,  count , somename);
			}
		} 
		else               //��ǰ�ҵ������ļ�
		{
			theFileName=cstrTostr( finder.GetFileName());           //��ȡ�ļ���
			theFilePath=cstrTostr(finder.GetFilePath());            //��ȡ�ļ�·��
			temp.keyword = theFileName;
			temp.result = winToLinPath(theFilePath)  ;
			if(true == somename.empty()) pSys->execu_out.push_back(temp);//����ؼ��ֲ���Ϊ�գ��������н��
			else if(temp.keyword ==somename) pSys->execu_out.push_back(temp);//����ֻ������ؼ���ƥ��Ľ��
		}
	}
}


/*********************************************************************************************************************
/*������:cmd_find
/*����: const struct command &currCmd ,struct sysInfo *  pSys
/*����ֵ:    void
/*ģ����: findģ��
/*��������:ִ��find����  
/*
/*********************************************************************************************************************/
void  cmd_find( const struct command &currCmd ,struct sysInfo *  pSys)
{
	if ( currCmd.argc > 3  )
	{
		throw(string("wrong parameters of this cmd !"));
	}
	if (currCmd.argc == 3 && currCmd.argv[1] != "-name")//�������󱨴�
	{
       throw(string("wrong args of this cmd !"));
	}
	if (currCmd.argc == 2 && currCmd.argv[0] != "-name")//�������󱨴�
	{
		throw(string("wrong args of this cmd !"));
	}
	    string somename ;//���˹ؼ���
        //ת��Ϊwindows·��
		string path ;
		if(currCmd.argc ==0 || currCmd.argc == 2) path = pSys->currPath ; //��·������ʱΪ��ǰ·��
		else  path = linToWinPath(reToAbsPath_linux(delPathTail_linux(currCmd.argv[0]), winToLinPath(pSys->currPath) )); 
		if (_access(path.c_str() , 0) != 0)throw(string("this path do not exists !"));//·�������ڱ���
	     if(false == sameLowerAndUp(path))  throw(string("the path you input do not exists !")); //��Сд��ͬ������

		if(false == isPath(path)) 
		{
			//Result.push_back(path); //ָ��·�����ļ�
		}
		else
		{  
			somename.clear();
			pSys->other_info = path ;//��������·��
			if(currCmd.argc == 3) somename = currCmd.argv[2];//��ȡ���ҹؼ���
			if(currCmd.argc == 2) somename = currCmd.argv[1];
			FindFile(path,pSys,1 , somename);//�ݹ�����·��
		}
		
        string str;
		pSys->other_info = somename ;//psy�ĸ�����Ϣ���ڼ�¼���������ؼ���
		if(currCmd.argc ==1 || currCmd.argc == 3 ) str = currCmd.argv[0];
		if(currCmd.argc == 0 || currCmd.argc == 2 ) str = ".";//·��Ϊ�գ�Ĭ��������ǰ·��
	       delPathTail_linux(str);
		   path = winToLinPath(path);
             if(somename == getFileName(str) || true == somename.empty())
			 {  
				 struct ex_out temp_out ;
				 temp_out.result = reToAbsPath_linux(str , winToLinPath(pSys->currPath)) ;
				 temp_out.keyword =getFileName(str);
				 pSys->execu_out.push_front(temp_out);
			 }
		     
			 for (list<ex_out>::iterator it = pSys->execu_out.begin(); it != pSys->execu_out.end() ; it++)//�����������·��ƴ�ӳ����û������
			 {                                                                                                                                         //����Ϊ��ʼ��·��
				 (*it).result.erase(0 , path.length());
				 if (str[str.length()-1] == '/' && false == (*it).result.empty() && (*it).result[0] == '/')
				 {
					 (*it).result = str+ (*it).result.erase(0,1) ;
					 continue; 
				 }
				 if (str[str.length()-1] != '/' && false == (*it).result.empty() && (*it).result[0] != '/')
				 {
					 (*it).result = str + '/' + (*it).result ;
					 continue; 
				 }
				 (*it).result = str + (*it).result ;
			 }

			if(true == somename.empty()) pSys->out_kind = LINE_OUT ; //�ؼ���Ϊ�գ������
			else  pSys->out_kind =LINE_OUT_FIND ;//�ؼ��ַǿգ��������
			return ;
}

