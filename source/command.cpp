/*********************************************************************************************************************
/*文件名:  command.cpp
/*模块名: 命令解析 & 命令执行
/*作者:    邹天明&戴潜
/*日期:    2011.10.25
/*功能:    程序主要部分,获取用户命令后,解析用户命令推入命令执行队列。通过不同命令执行函数执行。
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
/*函数名:  initSys
/*输入:  struct sysInfo *  psys
/*返回值:    void
/*模块名: 初始化模块
/*功能描述: 程序运行后，初始化用户数据，同时设置系统的配置文件  
/*
/*********************************************************************************************************************/
void  initSys(struct sysInfo *  psys)
{  
	if(0 != _access(USER_PATH,0))
	{
		mkMassDir(USER_PATH);//建立存放用户数据的配置文件
		SetFileAttributes(TEXT(USER_TEMP) , FILE_ATTRIBUTE_HIDDEN);//配置文件夹为隐藏文件夹
	}
    //获取系统所有用户信息表
	 readUserList(psys->sysUserInfo , USER_LIST_PATH);
     psys->login  = false ;//用户未登录
	 psys->out_kind = UNKNOW_;
}


/*********************************************************************************************************************
/*函数名: logIn
/*输入:  struct sysInfo * pSys
/*返回值:    void
/*模块名: 登录模块
/*功能描述: 检测用户名及密码，登陆成功后获取该用户的个人配置信息
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
		 getMyHisCmd(pSys->curruser);//获取该用户历史命令
		 getMyAlias(pSys->curruser);//获取该用户alias字符集
		 return ;
	 }
	 for (list<struct userInfo>::iterator it = pSys->sysUserInfo.begin(); it != pSys->sysUserInfo.end() ; it++)
	 {
		 if (strcmp((*it).username, username.c_str()) == 0)  //查找到该用户名
		 {
			 cout<<"Password:";
			 InputPassword(password);
			 if (strcmp((*it).password , password.c_str()) == 0 ) //密码正确
			 {
				 cout<<"Login successfully! Welcome to the SeedCup shell!\n"<<endl;//登陆成功
				 pSys->curruser.username = username ;
				// pSys->curruser.password = password ;
				 pSys->login = true ;
				 pSys->currPath = "c:" ;
				 getMyHisCmd(pSys->curruser);//获取该用户历史命令
				 getMyAlias(pSys->curruser);//获取该用户alias字符集
				 return ;
			 }
			 else{
				 throw(string("wrong password !")) ;   //密码错误
			 }   
		 }
	 }
	 throw(string("user do not exist !"));//用户不存在
}


/*********************************************************************************************************************
/*函数名: getCommand
/*输入:  struct sysInfo *  psys
/*返回值:   string 
/*模块名: 取命令模块
/*功能描述: 获取用户命令，并将命令保存至历史命令集
/*
/*********************************************************************************************************************/
 string getCommand(struct sysInfo *  pSys ) 
 {  
	  string  cmd ;	
	  int tabTime = 0;
	  struct sysInfo tempSys;
	  vector<string>systemcmd ;
	  //系统命令
	  char  syscmd [15][20]={"useradd" , "passwd" , "su" , "ls" , "cp","logout" , "mv","mkdir","cd","rm","alias","find","exit","cat"  ,"grep" } ;
	  for (int i = 0 ; i < 15 ;++i)
	  {
		  systemcmd.push_back(syscmd[i]);//获取系统命令
	  }
	  for (list<alias>::iterator  _it= pSys->curruser.myalias.begin() ; _it != pSys->curruser.myalias.end() ; _it ++)
	  {
		  systemcmd.push_back((*_it).subCmd);//获取自定义命令
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

		  //退格键
		  if(ch   ==   8  ) 
		  { 
			  if(cmd.empty()) continue;
			  cmd = cmd.erase(cmd.length()-1 , 1);
			  cout << "\b \b"; 
			  continue;
		  } 

		  //输入完成
		  else   if(ch   ==   0x0A   ||   ch   ==   0x0D) 
		  { 
			  cout<<endl;
			  break; 
		  } 

		  //获取方向键  
		  if ( ch == 224)
		  {
			  ch   =   _getch(); 
			  if (ch == 72)   //up键按下
			  {
				  //cout<<"\nup"<<endl;    //历史命令操作
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
			  if (ch == 80)   //down键按下
			  {
				 // cout<<"\ndown"<<endl;  //历史命令操作
				  if(it == pSys->curruser.myHisCmd.end()) continue ;
				  it ++ ;
                  if(it == pSys->curruser.myHisCmd.end())
				  {
					  for (size_t i = 0 ; i < cmd.length() ; ++ i)
					  {
						  cout << "\b \b"; 
					  }//回到光标输入
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
		  //获取TAB键
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
	  //将命令保存至历史命令集
	  pSys->curruser.myHisCmd.push_back(cmd);
	  if (pSys->curruser.myHisCmd.size() > 10)
	  {
		  pSys->curruser.myHisCmd.pop_front();  //删除最前面的命令
	  }
	  saveMyHisCmd(pSys->curruser);
     
	  return cmd ;
}


/*********************************************************************************************************************
/*函数名:  tabTips
/*输入:  struct sysInfo *  pSys  ,   string &cmd , int& tabtime ,const vector<string>&systemcmd
/*返回值:    void
/*模块名:智能提示模块
/*功能描述: 文件名，目录，命令的智能提示 
/*
/*********************************************************************************************************************/
void tabTips(struct sysInfo *  pSys  ,   string &cmd , int& tabtime ,const vector<string>&systemcmd)
{ 
	 struct sysInfo tempSys ;//临时储存解析的临时命令
	 tempSys.curruser.myalias = pSys->curruser.myalias ;
	  command tempcmd ;//解析出的当前命令
	  string front , maxFront ;//需要包含的前缀和返回的最大公共前缀
	  vector<string>results ; //储存搜索的结果
	  int amount = 0 ;//满足匹配项的个数
	  //maxfront( vector<string> strs ,  string front ,  vector<string> &haveFronts , int & amount );
	
	 //命令补充模块
	 string cmd_str = cmd ;
	 delSpace(  cmd_str );
	 if (true ==  cmd_str.empty() || cmd_str.at(cmd_str.length() -1) == ';')//没有命令输入
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
			 prin = fillstring(prin , 25);//输出所有命令
			 cout<<prin ;
			 if ((i %2 ) == 0 ) cout<<'\n'; //换行
		 }
		 if(systemcmd.size() %2 !=0) cout<<'\n';
		
		 cout<<pSys->curruser.username<<'@'<<"SeedCup"<<" "<<winToLinPath(pSys->currPath)<<" "<<'$'<<" ";
		 cout<<cmd;
		 return ;
	 }
	   paraserCommand(&tempSys ,  cmd);//解析已经输入的命令
	   if(!tempSys.commandQuene.empty())
	   {
		   tempcmd = tempSys.commandQuene.back();//获取最后输入的命令
	   }
	 //如果解析出参数个数为0 而输入的命令末尾无空格 而且非完整系统命令则补充命令
	 if(tempcmd.argv.empty()  && cmd[cmd.length() -1] != ' ' &&false == issysCmd(tempcmd.comName))
	 {
          front = tempcmd.comName ;//搜索项为已经输入的命令字符
		  maxFront = maxfront(systemcmd , front , results , amount);//搜索出公共前段
          if(amount == 0) return ;
          if (amount == 1 && maxFront == front) return ;//搜索结果为一并且二者相同
		  if (amount == 1 && maxFront != front)//搜索结果为一并且而这结果不同 则补充
		  {
			  maxFront.erase(0 , front.length());
			  cmd = cmd + maxFront ;
			  cout<<maxFront ;
			  return ;
		  }
          if (amount > 1 )//搜索结果数目大于一
          {
			  if(maxFront != front)//最大前缀与已输入前缀不同
			  {
				  maxFront.erase(0 , front.length()); //补充至最长前缀
				  cmd = cmd + maxFront ;
				  cout<<maxFront ;       
				   ;//记录tab次数，第二次时输出所有可能结果
				  return ;
			  }
			  if (maxFront == front && tabtime == 0)//最大前缀与已输入前缀相同 并且是第一次按tab
			  {
				  tabtime = 1;
				  return ;
			  }
			  if (maxFront == front && tabtime == 1)//最大前缀与已输入前缀相同 并且是第二次按tab，输出所有可能结果
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
						  if ((i %2 ) == 0 ) cout<<'\n'; //换行
				  }
				  if(results.size() %2 !=0) cout<<'\n';
				  cout<<pSys->curruser.username<<'@'<<"SeedCup"<<" "<<winToLinPath(pSys->currPath)<<" "<<'$'<<" "<<cmd;
			       return ;
			  }
          }
      }
       //路径补充模块
	 //如果解析出参数命令名不为空， 而输入的命令末尾有空格 或者则输出所有可能项
       if( (!tempcmd.comName.empty()) &&
		  ( cmd[cmd.length() -1] == ' ' || 
		  true == issysCmd(tempcmd.comName)&&cmd[cmd.length() -1] != ' '&&tempcmd.argc==0))
	   {
		   string path  = pSys->currPath ;
		   delPathTail_win(path) ;//目录为当前目录
		    vector<string> names ; //储存所有可能的目录与文件名
           if(FILE_AND_FOLDER == cmdKind(tempcmd.comName))//判断路径跟随类型 此处为文件和目录
		       getFileOrFolder(path , names ,  FILE_AND_FOLDER);//获取所有可能的目录与文件名
		   if(FOLDER_ONLY == cmdKind(tempcmd.comName))//判断路径跟随类型 此处为目录
			   getFileOrFolder(path , names ,  FOLDER_ONLY);//获取所有可能的目录
             
			  if(true == names.empty() ) return ;//结果为空
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
				   if ((i %2 ) == 0 ) cout<<'\n'; //换行
			   }
			   if(names.size() %2 !=0) cout<<'\n';
			   cout<<pSys->curruser.username<<'@'<<"SeedCup"<<" "<<winToLinPath(pSys->currPath)<<" "<<'$'<<" "<<cmd;
			   return ;
		  }
          //如果参数不为0 ， 并且cmd末尾无空格，则读取目录可能项目
	      if( ( tempcmd.argc != 0  && !tempcmd.argv.empty() &&cmd[cmd.length() -1] != ' '))
		  {
              string path = tempcmd.argv[tempcmd.argc - 1]; //将最后一个参数作为路径参数输入
			  string fatherPath ;	
			   vector<string> names ; //储存所有可能的目录与文件名
               
			  path = delPathTail_linux(path);//转化为标准路径形式
			  path = reToAbsPath_linux(path , winToLinPath(pSys->currPath));//将path转化为linux绝对路径
			  fatherPath = getFatherPath_linux(path);//获取其linux父目录
			  if (tempcmd.argv[tempcmd.argc -1].at(tempcmd.argv[tempcmd.argc-1].length() -1) == '/')
			  {
				  fatherPath = path ;
			  }
			  path = linToWinPath(path); //将path转化为windows路径
              fatherPath = linToWinPath(fatherPath);
              if(0 != _access(fatherPath.c_str() , 0)) return ; //如果路径不存在，返回
			  if(FILE_AND_FOLDER == cmdKind(tempcmd.comName))//判断路径跟随类型 此处为文件和目录
				  getFileOrFolder(fatherPath , names ,  FILE_AND_FOLDER);//获取所有可能的目录与文件名
			  if(FOLDER_ONLY == cmdKind(tempcmd.comName))//判断路径跟随类型 此处为目录
				  getFileOrFolder(fatherPath , names ,  FOLDER_ONLY);//获取所有可能的目录
			    if(true == names.empty() ) return ;//结果为空
             
				if (path == fatherPath ) front.clear();
			  else front = getFileName_win(path);
			  if (true == front.empty())
			  {  
				  if (names.size() == 1)//若果结果为一，直接补充
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
					  if ((i %2 ) == 0 ) cout<<'\n'; //换行
				  }
				  if(names.size() %2 !=0) cout<<'\n';
				  cout<<pSys->curruser.username<<'@'<<"SeedCup"<<" "<<winToLinPath(pSys->currPath)<<" "<<'$'<<" "<<cmd;
				  return ;
			  }
			  else
			  {
				  maxFront = maxfront(names , front , results , amount);//搜索出公共前段
				  if(amount == 0) return ;
				  if (amount == 1 && maxFront == front) return ;//搜索结果为一并且二者相同
				  if (amount == 1 && maxFront != front)//搜索结果为一并且而这结果不同 则补充
				  {
					  maxFront.erase(0 , front.length());
					  cmd = cmd + maxFront ;
					  cout<<maxFront ;
					  return ;
				  }
				  if (amount > 1 )//搜索结果数目大于一
				  {
					  if(maxFront != front)//最大前缀与已输入前缀不同
					  {
						  maxFront.erase(0 , front.length()); //补充至最长前缀
						  cmd = cmd + maxFront ;
						  cout<<maxFront ;       
						  //记录tab次数，第二次时输出所有可能结果
						  return ;
					  }
					  if (maxFront == front && tabtime == 0)//最大前缀与已输入前缀相同 并且是第一次按tab
					  {
						  tabtime = 1;
						  return ;
					  }
					  if (maxFront == front && tabtime == 1)//最大前缀与已输入前缀相同 并且是第二次按tab，输出所有可能结果
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
							  if ((i %2 ) == 0 ) cout<<'\n'; //换行
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
/*函数名:  paraserCommand
/*输入: struct sysInfo *  pSys  ,const string &cmd
/*返回值:    void
/*模块名: 命令解析模块
/*功能描述: 解析用户输入的命令 
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
		  if ( _cmd[i] == '|')  //管道处理
		  {
			   if(false == temp.empty())  tempCmds.push(temp);
			  tempCmds.push("|");
			  temp.clear();
			  i ++ ;
			  continue;
		  }
		
		  if ( _cmd[i] == '>')  //重定向命令
		  {
              tempCmds.push(temp);
              temp.clear();
			  temp.push_back('>');
              temp.push_back(' ');
			  while (++i < _cmd.length())
			  {
				  if(_cmd[i] != ' ') break ;  //过滤空格
			  }
			  if (i == _cmd.length())
			  {
				  throw(string("cmd > must have a paramater !"));   //重定向符号无参数 错误
			  }
              while(_cmd[i ] != ' '  && _cmd[i ] != '|' && _cmd[i ] != ';'&& i < _cmd.length())
				  temp.push_back(_cmd[i++]);
			  tempCmds.push(temp);
			  temp.clear();
			  continue;
		  }
		  if ( _cmd[i] == ';')  //连续运行命令
		  {
			  if(false == temp.empty())  tempCmds.push(temp);
			  temp.clear();
			  i++ ;
			  continue;
		  }
		
		   
		  temp.push_back(_cmd[i]);
		  ++i; 
		  if(i ==_cmd.length())   //如果非以上三种命令分隔，结束时添加本段命令
		  {
			  if(false == temp.empty())  tempCmds.push(temp);
			  break; 
		  }
	  }

	  string tempCmd ;
	  struct  command tagCmd ;
      tagCmd.argc =0 ;
      
	  while(false == tempCmds.empty())   //将分割的不同命令入命令队列
	  {
		 
          tempCmd = tempCmds.front();
		  tagCmd.argc =0 ;
		  tagCmd.argv.clear();
		  tagCmd.comName.clear();
		  tempCmds.pop();
		  delSpace(tempCmd); //删除空格
          getFirPar(tempCmd,temp);//获取第一个参数
		   //查找该用户等效字符集并替换
		  vector<string> recycle ; //处理等效命令死循环问题
		  recycle.clear() ;
		  for (list<struct alias >::iterator it = pSys->curruser.myalias.begin(); it !=  pSys->curruser.myalias.end();)                                
		  {   
			  if(0 == strcmp( (*it).subCmd , temp.c_str()) )//找到该命令为自定义名，获取其原名
			  {
				  tempCmd.erase(0 , temp.length()); //删除命令中自定义名
				  string  ss = (*it).sysCmd ;
				  tempCmd = ss + tempCmd ;     //将原命令名添加至该命令
				  temp = ss ;
				  recycle.push_back((*it).subCmd);//判断是否死循环
				  //找到的某个自定义名的原名与第一次的自定义名相同，表示死循环
				  if (false == recycle.empty() &&  0 == strcmp( recycle[0].c_str() , temp.c_str()) )
				  {   
					  vector<string>::iterator pStr;
					  bool move = true ;
					  for ( pStr = recycle.begin() ; pStr != recycle.end() ; pStr++)
					  {
						  for (it = pSys->curruser.myalias.begin(); it !=  pSys->curruser.myalias.end();)  
						  {
							  if(0 == strcmp( (*it).subCmd , (*pStr).c_str() ))//找到构成死循环的节点
							  {
								  list<struct alias >::iterator pT = it;
								  it++ ;
								  move = false;
								  pSys->curruser.myalias.erase(pT);//删除该节点
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
          delSpace(tempCmd); //删除空格
		  getFirPar(tempCmd,temp);//重新获取第一个参数
		  tagCmd.comName = temp ;
		 
		  if (temp == "ls")  //ls命令路径可以包含空格，比较特殊
		  {
			  i = tempCmd.find("ls");
			  tempCmd.erase(i,2);
			  delSpace(tempCmd);
			  if (string::npos != (i = tempCmd.find("-l"))) //带有附加参数
			  {
				  tagCmd.argc += 1 ;
				  tagCmd.argv.push_back("-l");
				  i = tempCmd.find("-l");
				  tempCmd.erase(i,2);
				  delSpace(tempCmd);
				  if(true == tempCmd.empty())   //路径为空
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
			 {                      //未带附加参数
				  if(true == tempCmd.empty())   //路径为空
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

		  if (temp == "alias") //alias参数形式比较特殊
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
		  delSpace(tempCmd);  //删除命令空格
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
/*函数名: commandQueue
/*输入:  struct sysInfo *  psys
/*返回值:    void
/*模块名: 命令执行模块
/*功能描述: 执行命令队列
/*
/*********************************************************************************************************************/
void  commandQueue(struct sysInfo *  pSys) 
{  
	  while(!pSys->commandQuene.empty())
	  { 
		  exeu(pSys); //执行命令
		  outResult(pSys);//输出命令结果
	  }
	  pSys->execu_out.clear();
	  pSys->out_kind = UNKNOW_ ;
	  return;
}


/*********************************************************************************************************************
/*函数名:exeu
/*输入:  struct sysInfo *  psys
/*返回值:    void
/*模块名: 命令执行模块
/*功能描述: 直接执行各项命令
/*
/*********************************************************************************************************************/
void exeu(struct sysInfo *  pSys)
{
      struct command currCmd = pSys->commandQuene.front();//获取要执行的命令
	  pSys->commandQuene.pop();   //删除当前被执行命令

	  if(currCmd.comName == "useradd") {  cmd_useradd(currCmd,pSys);    return ; } //执行useradd命令
	  if(currCmd.comName == "passwd")  {  cmd_passwd(currCmd, pSys);    return ; } //执行passwd命令
	  if(currCmd.comName == "su")  {  cmd_su(currCmd, pSys);    return ; } //执行su命令
	  if(currCmd.comName == "logout")  {  cmd_logout(currCmd, pSys);    return ; } //执行logout命令
	  if(currCmd.comName == "exit")  {  cmd_exit(currCmd, pSys);    return ; } //执行exit命令
	  if(currCmd.comName == "alias")  {  cmd_alias(currCmd, pSys);    return ; } //执行exit命令
	  if(currCmd.comName == "cat")  {  cmd_cat(currCmd, pSys);    return ; } //执行cat命令
	   if(currCmd.comName == "grep")  {  cmd_grep(currCmd, pSys);    return ; } //执行grep命令
       if(currCmd.comName == "mkdir")  {  cmd_mkdir(currCmd, pSys);    return ; } //执行mkdir命令
       if(currCmd.comName == "cd")  {  cmd_cd(currCmd, pSys);    return ; } //执行cd命令
	   if(currCmd.comName == "cp") {  try{cmd_cp(currCmd, pSys , CP_);}catch(int &e){};  return ; } //执行cp命令
	   if(currCmd.comName == "find")  {  cmd_find(currCmd, pSys);    return ; } //执行find命令
	   if(currCmd.comName == "rm")  {  cmd_rm(currCmd, pSys);    return ; } //执行rm命令
	   if(currCmd.comName == "mv")  {  cmd_mv(currCmd, pSys);    return ; } //执行mv命令
	   if(currCmd.comName == "ls")  {  cmd_ls(currCmd, pSys);    return ; } //执行ls命令
	  throw(string("invalid  cmd !"));//非法命令报错
	  return ; 
}


/*********************************************************************************************************************
/*函数名: outResult
/*输入:  struct sysInfo *  psys
/*返回值:    void
/*模块名: 命令执行模块
/*功能描述: 处理命令执行后的返回值
/*
/*********************************************************************************************************************/
void outResult(struct sysInfo *  pSys)
{   
	string path;
	if (!pSys->commandQuene.empty() &&pSys->commandQuene.front().comName == "|") //处理管道命令
	{
		pSys->out_kind =  NEXT_CMD ;
		pSys->commandQuene.pop();
		return ;
	}
    if (!pSys->commandQuene.empty() && pSys->commandQuene.front().comName == ">")//处理输出重定向
    {   
		struct command currcmd = pSys->commandQuene.front();
		int temKind = pSys->out_kind ;
		pSys->out_kind =  NONE_ ;//输出完毕
		pSys->commandQuene.pop();
		if(currcmd.argc != 1) throw(string("wrong parameters of redirect command !"));
        delPathTail_linux(currcmd.argv[0]);
	    path= linToWinPath(reToAbsPath_linux(currcmd.argv[0] , winToLinPath(pSys->currPath))) ;
		if (0 == _access(path.c_str() , 0) && false == isPath(path))//如果文件存在，提示是否覆盖
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

	if (pSys->out_kind == LINE_OUT) //行输出打印至屏幕
	{
		while(!pSys->execu_out.empty())	
		{
			cout << pSys->execu_out.front().result<<'\n' ;
			pSys->execu_out.pop_front();
		}
		cout<<'\n';
        pSys->out_kind =  NONE_ ;//输出完毕
		return ;
	}
	if (pSys->out_kind == LS) //LS结果行输出打印至屏幕
	{   
		int i =0 ;
		while( ! pSys->execu_out.empty())	
		{ 
			pSys->execu_out.front().result = fillstring(pSys->execu_out.front().result , 20);//填充字符串
			if(pSys->execu_out.front().add_info == I_AM_DIRECTORY)
				cout<<blue<<pSys->execu_out.front().result<<white ;
			else
				cout<<pSys->execu_out.front().result;
			pSys->execu_out.pop_front();
			++i ;
	     }
		if(i%4 != 0) cout<<"\n\n";
		else cout<<"\n";
		pSys->out_kind =  NONE_ ;//输出完毕
		return ;
	}
	if (pSys->out_kind == LS_R)
	{
		cout<<"Name                "<<"Size(KB)            "<<"Last modified       "<<"Type"<<endl;
		while(!pSys->execu_out.empty())
		{ 
		   pSys->execu_out.front().keyword = fillstring(pSys->execu_out.front().keyword , 20);//填充字符串
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
		pSys->out_kind =  NONE_ ;//输出完毕
		return ;
	}

	if (pSys->out_kind == LINE_OUT_FIND) //FIND结果行输出打印至屏幕
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
		pSys->out_kind =  NONE_ ;//输出完毕
		return ;
	}
    
	if (pSys->out_kind == KEY_WORD)//关键字高亮打印
	{
		while(!pSys->execu_out.empty())	
		{
			string temp = pSys->execu_out.front().result;
			string key = pSys->other_info;
			size_t i =0 , j = temp.find(key);
			while(j != string::npos)
			{
				cout<<white<<temp.substr(i , j -i)<<red<<key<<white; //红色高亮显示关键字
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
		pSys->out_kind =  NONE_ ;//输出完毕
		pSys->other_info.clear();
		return ;
	}
    return ;
}


/*********************************************************************************************************************
/*函数名:  cmd_useradd
/*输入: const struct command &currcmd , struct sysInfo *  pSys
/*返回值:    void
/*模块名:用户判断模块
/*功能描述: 执行useradd ，命令新建用户
/*
/*********************************************************************************************************************/
void cmd_useradd( const struct command &currcmd , struct sysInfo *  pSys)
{
	 pSys->out_kind = NONE_ ;//命令输出结果类型
	 pSys->execu_out.clear();//输出为空

	 if(currcmd.argc == 0)throw(string("cmd of useradd should have a parameter !")) ;
     if(currcmd.argc > 1 )throw(string("cmd of useradd  have too many parameter !")) ;
	 string userName = currcmd.argv[0];
	 //检查名称合法性
	 if(userName == "guest") throw(string("this name is invalid!")) ;
	 if (false == isNumOrAlpha(userName)) 
		 throw(string("name  can just have numbers and letters !")) ;
	 if( !(userName.length() > 0 && userName.length() < 11) )
          throw(string("the length of name   must be longer than 0 and lesser than 11 !")) ;
	
	 //判断该用户名是否已存在
	 for( list<struct userInfo >::iterator it = pSys->sysUserInfo.begin(); it != pSys->sysUserInfo.end(); ++it)
	 {
          if((*it).username == userName)
			   throw(string("this user name exists already !")) ;
	 }
    
	 string pw1 , pw2 ;
	 cout<<"password:";
	 InputPassword(pw1);
      //检查密码合法性
	 if (false == isNumOrAlpha(pw1)) 
		 throw(string("password  can just have numbers and letters !")) ;
	 if( !(pw1.length() > 5 && pw1.length() < 21) )
		 throw(string("the length of password   must be longer than 5 and lesser than 21 !")) ;
     //再次输入密码
	 cout<<"password again:";
	 InputPassword(pw2);
        if(pw1 != pw2) throw(string("two password is not the same "));

		struct userInfo newUser ;
         strcpy_s(newUser.username, userName.c_str());
		 strcpy_s(newUser.password , pw1.c_str());
		 pSys->sysUserInfo.push_back(newUser);
		 writeUserList(pSys->sysUserInfo,USER_LIST_PATH);//写入文件
		
		 cout<<userName<<" had been created successfully!\n\n" ;
	     return ;
}


/*********************************************************************************************************************
/*函数名:  cmd_passwd
/*输入: const struct command &currcmd ,struct sysInfo *  pSys
/*返回值:    void
/*模块名: 用户判断模块
/*功能描述: 执行passwd 命令，检测对应用户名的密码是否正确
/*
/*********************************************************************************************************************/
void  cmd_passwd( const struct command &currcmd ,struct sysInfo *  pSys)
{
	if(currcmd.argc == 0)throw(string("cmd of passwd should have a parameter !")) ;
	if(currcmd.argc > 1 )throw(string("cmd of passwd  have too many parameter !")) ;
	string userName = currcmd.argv[0]; 
    
   if (userName == "guest") throw(string("guest user do not have password !"));//guest用户没有密码
	//判断该用户名是否存在
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
	//检查密码合法性
	if (false == isNumOrAlpha(newpw1)) 
		throw(string("password  can just have numbers and letters !")) ;
	if( !(newpw1.length() > 5 && newpw1.length() < 21) )
		throw(string("the length of password   must be longer than 5 and lesser than 21 !")) ;
	cout<<"new password again:";
	InputPassword(newpw2);
	if(newpw1 != newpw2) throw(string("two new password is not the same ! "));

	strcpy_s((*it).password , newpw1.c_str());
	writeUserList(pSys->sysUserInfo,USER_LIST_PATH);
	
	pSys->out_kind = NONE_ ;  //输出结果类型为空
	pSys->execu_out.clear();
    
	cout<<"password of  "<<(*it).username<<" changed successfully!\n\n";
	return ;
}


/*********************************************************************************************************************
/*函数名:  cmd_su
/*输入: const struct command &currcmd ,struct sysInfo *  pSys
/*返回值:    void
/*模块名: 切换用户模块
/*功能描述: 新建用户
/*
/*********************************************************************************************************************/
void  cmd_su( const struct command &currcmd ,struct sysInfo *  pSys)
{
	if(currcmd.argc == 0)throw(string("cmd of su should have a parameter !")) ;
	if(currcmd.argc > 1 )throw(string("cmd of su  have too many parameter !")) ;
    string name = currcmd.argv[0];

	if(name == pSys->curruser.username) throw(string("same to current user!"));//与当前用户相同
	//判断该用户名是否存在
	list<struct userInfo >::iterator it ;
	for(  it = pSys->sysUserInfo.begin(); it != pSys->sysUserInfo.end(); ++it){
		if((*it).username == name)   break ;  }

	if(it == pSys->sysUserInfo.end() && name != "guest") throw(string("this user do not exists !"));//用户不存在
  
	if (name != "guest")//非guest用户需要输入密码
   {
	   string pw1 ;
	   cout<<"password:";
	   InputPassword(pw1);
	   if(pw1 != (*it).password) throw(string("password error!"));
   }
	
    pSys->currPath = "C:";//回到根目录
	pSys->curruser.username = name ;
	pSys->curruser.myalias.clear();
	pSys->curruser.myHisCmd.clear();
	getMyHisCmd(pSys->curruser);//获取该用户历史命令
    getMyAlias(pSys->curruser);   //获取该用户alias字符集
	cout<<"user changed successfully!\n\n";
	pSys->out_kind = NONE_ ;
	pSys->execu_out.clear();
	while(!pSys->commandQuene.empty()) pSys->commandQuene.pop();//由于控制权转换，故清空命令队列
	return;
} 


/*********************************************************************************************************************
/*函数名:  cmd_logout
/*输入: const struct command &currcmd ,struct sysInfo *  pSys
/*返回值:    void
/*模块名: 用户登出模块
/*功能描述: 用户登出命令
/*
/*********************************************************************************************************************/
void  cmd_logout( const struct command &currcmd ,struct sysInfo *  pSys)
{
   if(currcmd.argc > 0 )throw(string("cmd of logout  have too many parameter !")) ;//参数形式错误
   
   while(!pSys->commandQuene.empty()) pSys->commandQuene.pop();//清空命令队列
   pSys->execu_out.clear();//清空输出结果
   pSys->login = false ;       //登出
   pSys->out_kind = UNKNOW_ ;  //清空当前用户信息
   pSys->curruser.myalias.clear();
   pSys->curruser.myHisCmd.clear();
   pSys->curruser.username.clear();
   cout<<"\n";
   return;
}


/*********************************************************************************************************************
/*函数名:cmd_exit
/*输入: const struct command &currcmd ,struct sysInfo *  pSys
/*返回值:    void
/*模块名:程序结束模块
/*功能描述: 执行exit命令 退出程序
/*
/*********************************************************************************************************************/
void  cmd_exit( const struct command &currcmd ,struct sysInfo *  pSys)
{  
     if(currcmd.argc > 0 )throw(string("cmd of exit  have too many parameter !")) ;//参数形式错误
	 delete pSys ;
	 exit(0);
}


/*********************************************************************************************************************
/*函数名: cmd_alias
/*输入: const struct command &currcmd ,struct sysInfo *  pSys
/*返回值:    void
/*模块名: alias模块
/*功能描述: 执行alias命令，将该自定义名添加至该用户alias字符集
/*
/*********************************************************************************************************************/
void  cmd_alias( const struct command &currcmd ,struct sysInfo *  pSys)
{
    if(currcmd.argc != 2 )throw(string("cmd of alias  have take two parameters !")) ;//参数形式错误
	string mycmd = currcmd.argv[0];
	string syscmd = currcmd.argv[1];
    list<struct alias >::iterator it;

	for ( it = pSys->curruser.myalias.begin(); it !=  pSys->curruser.myalias.end() ;++it)                                
	{
	    if ( 0 == strcmp( (*it).subCmd , mycmd.c_str()) ) //若果该自定义名已存在，提示是否覆盖
	    {
			cout<<"The alias of "<<"\""<<mycmd<<"\""<<"already exsits , cover it ? <Y/N>";
			char choose;
			cin >> choose;
			if('y' == choose || 'Y' == choose) 
			{
				pSys->curruser.myalias.erase(it);//释放原来自定义
				break;
			}
			else  throw(string(""));
	    }
	}
		struct alias  tempAlias ;   //将该自定义名添加至该用户alias字符集
		strcpy_s(tempAlias.subCmd,mycmd.c_str());
		strcpy_s(tempAlias.sysCmd , syscmd.c_str());
        pSys->curruser.myalias.push_back(tempAlias);
		saveMyAlias(pSys->curruser);//写入文件
        
		cout<<"The alias has been set successfully!\n\n";
		pSys->out_kind = NONE_ ;  //该程序输出类型为空
		pSys->execu_out.clear();
}


/*********************************************************************************************************************
/*函数名:  myfunction 
/*输入: ex_out  e_i, ex_out e_j
/*返回值:   bool 
/*模块名: 初始化模块
/*功能描述: 判定大小
/*
/*********************************************************************************************************************/
bool myfunction (ex_out  e_i, ex_out e_j) 
{ 
	return (e_i.keyword < e_j.keyword);
}


/*********************************************************************************************************************
/*函数名:  FindFile_Ls
/*输入: string strpath, struct sysInfo *  pSys , bool getInfo
/*返回值:    void
/*模块名: 文件查找模块
/*功能描述: 执行ls命令时，寻找对应目录的文件 
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
	BOOL working = finder.FindFile(path + "\\*.*");     //是否存在文件或目录，是则为1
	while (working)//填充keyword与add_info
	{
		working = finder.FindNextFile();
		if (finder.IsDots())            //当前找到的是自身
			continue;
		if (finder.IsDirectory())        //当前找到的是目录
		{   
			  DWORD isHidden=GetFileAttributes(finder.GetFilePath()); //过滤隐藏文件夹
			  if ( !(isHidden & FILE_ATTRIBUTE_HIDDEN))
			  {
				  temp.keyword= cstrTostr(finder.GetFileName());
				  temp.add_info = I_AM_DIRECTORY ;
				  pSys->execu_out.push_back(temp);
			  }
		 } 
		else               //当前找到的是文件
		{   
			  DWORD isHidden=GetFileAttributes(finder.GetFilePath()); //过滤隐藏文件
			  if ( !(isHidden & FILE_ATTRIBUTE_HIDDEN))
			  {
				  temp.keyword  =cstrTostr( finder.GetFileName());     //获取文件名
		          temp.add_info = I_AM_FILE ;
		          pSys->execu_out.push_back(temp);
			  }
		}
	}
	if (false == getInfo)
	{
		for (list<struct ex_out>::iterator it = pSys->execu_out.begin() ; it != pSys->execu_out.end() ; ++it)
		{
			(*it).result = (*it).keyword ; //不用获取文件信息
		}
		pSys->execu_out.sort(myfunction);
		return ;
	}
	else
	{
		multimap<string,string>aFile;
		FindFileInfo(strpath ,aFile);            //获取该目录下所有文件和目录的信息
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
/*函数名: cmd_ls
/*输入: const struct command &currcmd ,struct sysInfo *  pSys
/*返回值:    void
/*模块名: 初始化模块
/*功能描述: 执行ls命令
/*
/*********************************************************************************************************************/
void  cmd_ls( const struct command &currcmd ,struct sysInfo *  pSys)
{
      if(currcmd.argc > 2 )throw(string("cmd of cat  have error parameter !")) ;//参数形式错误
      string win_path ;	 
	  if (currcmd.argc ==0)//没有参数
	  {  
		   win_path = pSys->currPath; //无参数时目录为当前目录
           FindFile_Ls(win_path , pSys , false);
		   pSys->out_kind = LS ;//该命令输出类型为行输出
	  }
	  if (currcmd.argc == 1 && currcmd.argv[0] == "-l")
	  {
		  win_path = pSys->currPath; //无路径参数时目录为当前目录
		  FindFile_Ls(win_path , pSys , true);
		  pSys->out_kind = LS_R ;//该命令输出类型为行输出
	  }
	  if (currcmd.argc == 1 && currcmd.argv[0] != "-l")
	  {
		  win_path = linToWinPath(reToAbsPath_linux(delPathTail_linux(currcmd.argv[0]), winToLinPath(pSys->currPath)) ); //
		  if(_access(win_path.c_str() , 0) != 0 || false == sameLowerAndUp(win_path) || false == isPath(win_path)) 
			  throw(string("invalid path!"));
		  FindFile_Ls(win_path , pSys , false);
		  pSys->out_kind = LS ;//该命令输出类型为行输出
	  }
	  if (currcmd.argc == 2)
	  {
		  if(currcmd.argv[0] != "-l") throw(string("error args of this cmd!"));
		  win_path = linToWinPath(reToAbsPath_linux(delPathTail_linux(currcmd.argv[1]), winToLinPath(pSys->currPath)) ); //
		  if(_access(win_path.c_str() , 0) != 0 || false == sameLowerAndUp(win_path) || false == isPath(win_path)) 
			  throw(string("invalid path!"));
		  FindFile_Ls(win_path , pSys , true);
		  pSys->out_kind = LS_R ;//该命令输出类型为行输出
	  }
     return ;
}


/*********************************************************************************************************************
/*函数名:  cmd_cat
/*输入:  const struct command &currcmd ,struct sysInfo *  pSys
/*返回值:    void
/*模块名: 命令模块
/*功能描述: 执行cat命令  
/*
/*********************************************************************************************************************/
void  cmd_cat( const struct command &currcmd ,struct sysInfo *  pSys)
{  
	struct ex_out  temResult ;
	temResult.add_info = 0;
	string path;
	pSys->execu_out.clear();
    if(currcmd.argc == 0 || currcmd.argc > 2 )throw(string("cmd of cat  have error parameter !")) ;//参数形式错误
	if (currcmd.argc == 1) //有1个参数，即文件路径
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

		pSys->out_kind = LINE_OUT ;//该命令输出类型为行输出
		return ;
}


/*********************************************************************************************************************
/*函数名:  cmd_grep
/*输入: const struct command &currcmd ,struct sysInfo *  pSys
/*返回值:    void
/*模块名: 命令模块
/*功能描述:执行grep命令
/*
/*********************************************************************************************************************/
void  cmd_grep( const struct command &currcmd ,struct sysInfo *  pSys)
{
     if(currcmd.argc != 1 )throw(string("cmd of cat  have error parameter !")) ;//参数形式错误
     if(false == isNumOrAlpha(currcmd.argv[0]) )throw(string("the parameter has invalid character !")) ;//参数形式错误
	 if(pSys->out_kind != NEXT_CMD)throw(string("there should be a cmd before the command !"));

	 for (list<ex_out>::iterator it = pSys->execu_out.begin() ; it != pSys->execu_out.end() ;)
     {
		 if (string::npos == (*it).result.find(currcmd.argv[0]))//查找关键字，如果没有，将该条目从结果中删除
         {  
             list<ex_out>::iterator del = it ; 
			 it ++ ;
             pSys->execu_out.erase(del);
			 continue;
         }
		 ++it;
	 }
     pSys->other_info = currcmd.argv[0] ;//记录关键字
	 pSys->out_kind = KEY_WORD ;//该命令输出类型为关键字输出模式
	 return ;
} 


/*********************************************************************************************************************
/*函数名:  cmd_mkdir
/*输入:  const struct command &currcmd ,struct sysInfo *  pSys
/*返回值:    void
/*模块名: 命令模块
/*功能描述: 执行mkdir命令
/*
/*********************************************************************************************************************/
void  cmd_mkdir( const struct command &currcmd ,struct sysInfo *  pSys)
{    
      if(currcmd.argc == 0 || currcmd.argc > 2 )
		  throw(string("cmd of mkdir  have error parameter !")) ;//参数形式错误
      if(currcmd.argc == 2 && currcmd.argv[0] != "-p") 
		  throw(string("cmd of mkdir  have error parameter !")) ;//参数形式错误
	  if((currcmd.argc == 1 && currcmd.argv[0] == "/") || (currcmd.argc == 2 && currcmd.argv[1] == "/"))
           throw(string("can not establish this path  !")) ;
	  if((currcmd.argc == 1 && delPathTail_linux(currcmd.argv[0]).at(delPathTail_linux(currcmd.argv[0]).length()-1)== '/')
		  || (currcmd.argc == 2 &&  delPathTail_linux(currcmd.argv[1]).at(delPathTail_linux(currcmd.argv[1]).length()-1) == '/'))
		  throw(string("invalid path name   !")) ;
      if (currcmd.argc == 1 &&
		  ( true == isValidstr(getLastPath_linux( reToAbsPath_linux(delPathTail_linux(currcmd.argv[0]), winToLinPath(pSys->currPath) ) ))
		  || currcmd.argv[0].length() > 255))
		  throw(string("cmd of mkdir  fail to work!")) ;//参数形式错误
	  if (currcmd.argc == 2 &&
		  ( true == isValidstr(getLastPath_linux( reToAbsPath_linux(delPathTail_linux(currcmd.argv[1]), winToLinPath(pSys->currPath) ) ))
		  || currcmd.argv[0].length() > 255))
		  throw(string("cmd of mkdir  fail to work !")) ;//参数形式错误
      string path ;
	  if (currcmd.argc ==1)
	  {
		  path = currcmd.argv[0];
		  path =delPathTail_linux(path);
		 path= linToWinPath(reToAbsPath_linux(path , winToLinPath(pSys->currPath))) ;
		 if(0 == _access(path.c_str() , 0))throw(string("this path exists already !"));//路径已存在报错
		 if(0 != _access(getfatherDir(path).c_str(), 0))throw(string("can not  establish this path  !"));//父路径不存在报错
		 mkSingleDir(path);
		 pSys->out_kind = NONE_ ;  //该命令输出类型为空
		 pSys->execu_out.clear();
		 return ;
	  }
	  if (currcmd.argc ==2)
	  {   
		  path = currcmd.argv[1];
		  path = delPathTail_linux(path);
		  path= linToWinPath(reToAbsPath_linux(path , winToLinPath(pSys->currPath))) ;
		  if(0 == _access(path.c_str() , 0))throw(string("this path exists already !"));//路径已存在报错
		  mkMassDir(path);
		  pSys->out_kind = NONE_ ;  //该命令输出类型为空
		  pSys->execu_out.clear();
		  return ;
	  }
	  return ;
}


/*********************************************************************************************************************
/*函数名:  cmd_cd
/*输入: const struct command &currcmd ,struct sysInfo *  pSys
/*返回值:    void
/*模块名: 命令模块
/*功能描述: 执行cd命令
/*
/*********************************************************************************************************************/
void  cmd_cd( const struct command &currcmd ,struct sysInfo *  pSys)
{
	if(currcmd.argc > 1)throw(string("too many paraters of the cd cmd"));
    if (currcmd.argc == 0)
    {
		pSys->currPath = "c:"; //参数为空，回到根目录
		pSys->out_kind = NONE_ ;  //该命令输出类型为空
		pSys->execu_out.clear();
        return ;
    }
	if (currcmd.argc == 1 && currcmd.argv[0] == "/")
	{
		pSys->currPath = "c:"; //参数为/，回到根目录
		pSys->out_kind = NONE_ ;  //该命令输出类型为空
		pSys->execu_out.clear();
		return ;
	}
	string path = currcmd.argv[0]; //获取输入目录
    path = delPathTail_linux(path);//将目录转换为标准格式
	path = reToAbsPath_linux(path,winToLinPath(pSys->currPath));//将路径转换为绝对路径
	path = linToWinPath(path); //将路径格式转换为windows格式
	if(0 != _access(path.c_str() , 0))//路径是否存在
		throw(string("this is a invalid path !"));
	if (false == sameLowerAndUp(path))
	    throw(string("this is a invalid path !"));
   
	if(false == isPath(path))//路径是否为目录
	 {
		 throw(string("this is not a directory !"));//is   not directory
	 } 

    pSys->currPath = path ;
	pSys->out_kind = NONE_ ;
	pSys->execu_out.clear();
	return ;
}


/*********************************************************************************************************************
/*函数名:  cp
/*输入: command recommand , int choose
/*返回值:    void
/*模块名: 复制模块
/*功能描述: 复制文件或目录
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
				cout<<"1.  替换"<<endl;
				cout<<"2.  取消复制"<<endl;
				cout<<"3.  复制但不替换   ";
			}
			else{
				cout<<"1.  替换"<<endl;
				cout<<"2.  取消移动"<<endl;
				cout<<"3.  移动但不替换   ";
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
			if(choose == 2) throw(choice);//选2取消复制
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
					cout<<"1. 合并目录"<<endl;
					cout<<"2. 取消复制   ";
				}
				else{
					cout<<"1. 合并目录"<<endl;
					cout<<"2. 取消移动   ";
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
/*函数名:  rm
/*输入:  const string & path
/*返回值:    void
/*模块名: 目录模块
/*功能描述: 删除目录或文件 
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
/*函数名:   cmd_cp
/*输入: const struct command &currcmd ,struct sysInfo *  pSys, int choose
/*返回值:    void
/*模块名: 复制模块
/*功能描述:执行cp命令
/*
/*********************************************************************************************************************/
void  cmd_cp( const struct command &currcmd ,struct sysInfo *  pSys, int choose)
{ 
	struct command tempcmd   ;
  if(currcmd.argc < 2 || currcmd.argc > 3)throw(string("wrong parameters of the  cmd"));
   if (currcmd.argc == 2)//只有两个参数
   {
	   try{ 
	   tempcmd.argc = 2;
	   tempcmd.argv.push_back(linToWinPath(reToAbsPath_linux(delPathTail_linux(currcmd.argv[0]), winToLinPath(pSys->currPath)) ));
	   tempcmd.argv.push_back(linToWinPath(reToAbsPath_linux(delPathTail_linux(currcmd.argv[1]), winToLinPath(pSys->currPath) )));
	   if( _access(tempcmd.argv[0].c_str() , 0) != 0 || false ==sameLowerAndUp(tempcmd.argv[0]) )
		   throw(string("src path is invalid !"));//判断源路径是否有效
	   if (true == isPath(tempcmd.argv[0]))//如果源路径是目录，不可拷贝
	   {
		   throw(string("can not do this cmd to  directory without args!"));
	   }
	  if( _access(tempcmd.argv[1].c_str() , 0) == 0 && false == isPath(tempcmd.argv[1]))//如果目标路径存在并且不是目录
	   {           
		   //先将源文件复制到临时文件夹
           string filename  = USER_TEMP +  getFileName(tempcmd.argv[1]);//获取临时目标文件名
		   copyFile(tempcmd.argv[0] , filename );//将文件赋值到临时文件夹
		   tempcmd.argv[0] = filename ;
		   tempcmd.argv[1] = getPathName(tempcmd.argv[1]);//获取目标文件所在的目录
		   cp(tempcmd,choose);
		   remove( filename.c_str() );//删除临时文件
		   pSys->out_kind = NONE_ ;  //该命令输出类型为空
		   pSys->execu_out.clear();
		   return ;
        }
	  cp(tempcmd , choose);}catch(int & e){
		  pSys->out_kind = NONE_ ;  //该命令输出类型为空
		  pSys->execu_out.clear();
		  throw;
	  };
	   pSys->out_kind = NONE_ ;  //该命令输出类型为空
	   pSys->execu_out.clear();
	   return ;
    }
   if (currcmd.argc == 3)//有3个参数
   {   
	   try{
	   if(currcmd.argv[0] != "-r") throw(string(" unknow args of  cp !"));
	 
	   if(currcmd.argv[1] == "/")throw(string("can not do this cmd to root directory  !"));//根目录不可拷贝
	   tempcmd.argc = 3;
	   tempcmd.argv.push_back("-r");
	   tempcmd.argv.push_back(linToWinPath(reToAbsPath_linux(delPathTail_linux(currcmd.argv[1]), winToLinPath(pSys->currPath) )));
	   tempcmd.argv.push_back(linToWinPath(reToAbsPath_linux(delPathTail_linux(currcmd.argv[2]), winToLinPath(pSys->currPath) )));
	   if( _access(tempcmd.argv[1].c_str() , 0) != 0 || false ==sameLowerAndUp(tempcmd.argv[1]) )
		   throw(string("src path is invalid !"));//判断源路径是否有效
	   if(0==_access(tempcmd.argv[1].c_str() ,0) && 0 == _access(tempcmd.argv[2].c_str() ,0) &&//若原路径为目录而目标路径为非目录报错
		   true == isPath(tempcmd.argv[1] )&& false == isPath(tempcmd.argv[2]))
		   throw(string("can not copy or move a directory to file!"));
	   //若原路径为目录而目标路径不存在，报错
	   if(0==_access(tempcmd.argv[1].c_str() ,0) &&true == isPath(tempcmd.argv[1] )&& 
		   (0 != _access(tempcmd.argv[2].c_str() , 0 ) || false == sameLowerAndUp(tempcmd.argv[2])))
		   throw(string("dest directory do not exists!"));

	   if( _access(tempcmd.argv[2].c_str() , 0) == 0 && false == isPath(tempcmd.argv[2]))//如果目标路径存在并且不是目录
	   {           
		   //先将源文件复制到临时文件夹
		   string filename  = USER_TEMP +  getFileName(tempcmd.argv[1]);//获取临时目标文件名
		   copyFile(tempcmd.argv[1] , filename );//将文件赋值到临时文件夹
		   tempcmd.argv[1] = filename ;
		   tempcmd.argv[2] = getPathName(tempcmd.argv[2]);//获取目标文件所在的目录
		   cp(tempcmd,choose);
		   remove( filename.c_str() );//删除临时文件
		   pSys->out_kind = NONE_ ;  //该命令输出类型为空
		   pSys->execu_out.clear();
		   return ;
	   }
	   cp(tempcmd ,choose);}catch(int & e){
		   pSys->out_kind = NONE_ ;  //该命令输出类型为空
		   pSys->execu_out.clear();
		   throw;
	   };
	   pSys->out_kind = NONE_ ;  //该命令输出类型为空
	   pSys->execu_out.clear();
	   return ;
   }
}


/*********************************************************************************************************************
/*函数名:  cmd_rm
/*输入: const struct command &currCmd ,struct sysInfo *  pSys
/*返回值:    void
/*模块名: 目录删除模块
/*功能描述: 执行rm命令 
/*
/*********************************************************************************************************************/
void  cmd_rm( const struct command &currCmd ,struct sysInfo *  pSys)//执行rm命令
{
     if(currCmd.argc == 0 || currCmd.argc > 2) throw(string("cmd parameters error !"));//参数错误
     if(currCmd.argc == 2 && currCmd.argv[0] != "-r") throw(string("cmd args error ! need -r to delete directory"));//附加参数错误
	 
	 string win_path ;	
	 if(currCmd.argc == 1) //获得windows格式路径
	 {
		 win_path = linToWinPath(  reToAbsPath_linux(delPathTail_linux(currCmd.argv[0]), winToLinPath(pSys->currPath) )); 
		 if(true == isUpPath_Linux(  reToAbsPath_linux(delPathTail_linux(currCmd.argv[0]), winToLinPath(pSys->currPath) ) ,winToLinPath(pSys->currPath)) )
		 throw(string("can not remove this path !"));
		if( "/" == reToAbsPath_linux(delPathTail_linux(currCmd.argv[0]), winToLinPath(pSys->currPath) ))
         throw(string("can not remove root directory !"));//根目录不可删除
	 }
	 if(currCmd.argc == 2)
	 {
        win_path = linToWinPath(reToAbsPath_linux(delPathTail_linux(currCmd.argv[1]), winToLinPath(pSys->currPath) )); 
		if(true == isUpPath_Linux(reToAbsPath_linux(delPathTail_linux(currCmd.argv[1]), winToLinPath(pSys->currPath) ) ,winToLinPath(pSys->currPath)))
		throw(string("can not remove this path !"));
		if( "/" == reToAbsPath_linux(delPathTail_linux(currCmd.argv[1]), winToLinPath(pSys->currPath) ))
			throw(string("can not remove root directory !"));//根目录不可删除
	 }
	 if(0 != _access(win_path.c_str() , 0))//路径不存在，报错
		 throw(string("the path you input do not exists !"));
	 if(false == sameLowerAndUp(win_path))  throw(string("the path you input do not exists !")); //大小写不同，报错
	 if(pSys->currPath == win_path)throw(string("can not delete current directory!"));
	 if(currCmd.argc == 1 &&  true == isPath(win_path))
		 throw(string("can not delete directory without args !"));//未加附加参数试图删除目录
	 if(true == isPath(win_path))   rm(win_path);//如果是目录，删除目录
	 else  remove(win_path.c_str());	//是文件，删除文件
	 pSys->out_kind = NONE_ ;  //该命令输出类型为空
	 pSys->execu_out.clear();
	 return ;
}


/*********************************************************************************************************************
/*函数名:  cmd_mv
/*输入:  const struct command &currcmd ,struct sysInfo *  pSys
/*返回值:    void
/*模块名: 文件剪切模块
/*功能描述:执行mv命令 
/*
/*********************************************************************************************************************/
void  cmd_mv( const struct command &currcmd ,struct sysInfo *  pSys)//执行mv命令
{
	struct command tempcmd   ;
	string srcPath ,destPath;
	if(currcmd.argc < 2 || currcmd.argc > 3)//参数形式检查
		throw(string("wrong parameters of the this cmd"));
	if(currcmd.argc == 3 && currcmd.argv[0] != "-r")
        throw(string("wrong parameters of the this cmd"));
	if(currcmd.argc == 2)
	{//获取源路径
	  srcPath = linToWinPath(reToAbsPath_linux(delPathTail_linux(currcmd.argv[0]), winToLinPath(pSys->currPath)) );
      destPath = linToWinPath(reToAbsPath_linux(delPathTail_linux(currcmd.argv[1]), winToLinPath(pSys->currPath)) );
	} 
	if(currcmd.argc == 3)
	{
		srcPath = linToWinPath(reToAbsPath_linux(delPathTail_linux(currcmd.argv[1]), winToLinPath(pSys->currPath)) );
		 destPath = linToWinPath(reToAbsPath_linux(delPathTail_linux(currcmd.argv[2]), winToLinPath(pSys->currPath)) );
	}
	if (_access(srcPath.c_str() , 0) != 0 || false == sameLowerAndUp(srcPath))
	    throw(string("source path do not exists!"));//源路径不存在
	if (srcPath == destPath) return ; //源路径和目标路径相同，什么都不做
  if (0 == _access(destPath.c_str() , 0) && true == isPath(destPath))//目标路径是已存在的目录
  {
	  string paDir = getfatherDir( srcPath);
	  delPathTail_win(paDir);
	  if (paDir == destPath)
	  {
		  pSys->out_kind = NONE_ ;  //该命令输出类型为空
		  pSys->execu_out.clear();
		   return ;  //如果源路径的父目录与目标路径相同，则表明是自身移动到自身，则直接返回
	  }
  }
  try{
	  cmd_cp(currcmd,pSys , MV_);
     if (true == isPath(srcPath))
	    rm(srcPath);//删除源路径
	else  remove(srcPath.c_str());//删除源文件
  }catch(int & e){  } ;
	pSys->out_kind = NONE_ ;  //该命令输出类型为空
	pSys->execu_out.clear();
	return ;
}


/*********************************************************************************************************************
/*函数名: FindFile
/*输入: string strpath, struct sysInfo *  pSys, int count , const string & somename
/*返回值:    void
/*模块名: 文件查找
/*功能描述: 在目录下查找文件，cout为1查找文件包括目录下的子目录,为0则不包括 somename为过滤关键字
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
	BOOL working = finder.FindFile(path + "\\*.*");     //是否存在文件或目录，是则为1
	while (working)
	{
		working = finder.FindNextFile();
		if (finder.IsDots())            //当前找到的是自身
			continue;
		if (finder.IsDirectory())        //当前找到的是子目录
		{
			if (count==1)
			{   
				 temp.result = winToLinPath(cstrTostr(finder.GetFilePath()));
				 temp.keyword=winToLinPath(getFileName(cstrTostr(finder.GetFilePath())));
				 if(true == somename.empty()) pSys->execu_out.push_back(temp);//如果关键字参数为空，加入所有结果
				 else if(temp.keyword ==somename) pSys->execu_out.push_back(temp);//否则只加入与关键字匹配的结果
				 FindFile(cstrTostr(finder.GetFilePath()) ,  pSys,  count , somename);
			}
		} 
		else               //当前找到的是文件
		{
			theFileName=cstrTostr( finder.GetFileName());           //获取文件名
			theFilePath=cstrTostr(finder.GetFilePath());            //获取文件路径
			temp.keyword = theFileName;
			temp.result = winToLinPath(theFilePath)  ;
			if(true == somename.empty()) pSys->execu_out.push_back(temp);//如果关键字参数为空，加入所有结果
			else if(temp.keyword ==somename) pSys->execu_out.push_back(temp);//否则只加入与关键字匹配的结果
		}
	}
}


/*********************************************************************************************************************
/*函数名:cmd_find
/*输入: const struct command &currCmd ,struct sysInfo *  pSys
/*返回值:    void
/*模块名: find模块
/*功能描述:执行find命令  
/*
/*********************************************************************************************************************/
void  cmd_find( const struct command &currCmd ,struct sysInfo *  pSys)
{
	if ( currCmd.argc > 3  )
	{
		throw(string("wrong parameters of this cmd !"));
	}
	if (currCmd.argc == 3 && currCmd.argv[1] != "-name")//参数错误报错
	{
       throw(string("wrong args of this cmd !"));
	}
	if (currCmd.argc == 2 && currCmd.argv[0] != "-name")//参数错误报错
	{
		throw(string("wrong args of this cmd !"));
	}
	    string somename ;//过滤关键字
        //转换为windows路径
		string path ;
		if(currCmd.argc ==0 || currCmd.argc == 2) path = pSys->currPath ; //无路径参数时为当前路径
		else  path = linToWinPath(reToAbsPath_linux(delPathTail_linux(currCmd.argv[0]), winToLinPath(pSys->currPath) )); 
		if (_access(path.c_str() , 0) != 0)throw(string("this path do not exists !"));//路径不存在报错
	     if(false == sameLowerAndUp(path))  throw(string("the path you input do not exists !")); //大小写不同，报错

		if(false == isPath(path)) 
		{
			//Result.push_back(path); //指定路径是文件
		}
		else
		{  
			somename.clear();
			pSys->other_info = path ;//传递搜索路径
			if(currCmd.argc == 3) somename = currCmd.argv[2];//获取查找关键字
			if(currCmd.argc == 2) somename = currCmd.argv[1];
			FindFile(path,pSys,1 , somename);//递归搜索路径
		}
		
        string str;
		pSys->other_info = somename ;//psy的附加信息用于记录本次搜索关键词
		if(currCmd.argc ==1 || currCmd.argc == 3 ) str = currCmd.argv[0];
		if(currCmd.argc == 0 || currCmd.argc == 2 ) str = ".";//路径为空，默认搜索当前路径
	       delPathTail_linux(str);
		   path = winToLinPath(path);
             if(somename == getFileName(str) || true == somename.empty())
			 {  
				 struct ex_out temp_out ;
				 temp_out.result = reToAbsPath_linux(str , winToLinPath(pSys->currPath)) ;
				 temp_out.keyword =getFileName(str);
				 pSys->execu_out.push_front(temp_out);
			 }
		     
			 for (list<ex_out>::iterator it = pSys->execu_out.begin(); it != pSys->execu_out.end() ; it++)//遍历结果，将路径拼接成以用户输入的
			 {                                                                                                                                         //参数为起始的路径
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

			if(true == somename.empty()) pSys->out_kind = LINE_OUT ; //关键字为空，行输出
			else  pSys->out_kind =LINE_OUT_FIND ;//关键字非空，高亮输出
			return ;
}

