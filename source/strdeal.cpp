/*********************************************************************************************************************
/*文件名:  strdeal.cpp
/*模块名: 字符处理
/*作者:    邹天明&戴潜
/*日期:    2011.10.25
/*功能:     处理各种字符串操作
/*
/*********************************************************************************************************************/
#include "strdeal.h"



/*********************************************************************************************************************
/*函数名: InputUserName
/*输入: string & str
/*返回值:  void 
/*模块名: 用户模块
/*功能描述: 输入用户名
/*
/*********************************************************************************************************************/
void   InputUserName(string & str) 
{
	int ch  = 0; 
	while(true) 
	{ 
		ch   =   _getch(); 
     	//退格键
		if(ch   ==   8  ) 
		{ 
			if(str.empty()) continue ;
			str = str.substr(0,str.length() - 1);
			cout << "\b \b"; 
			continue;
		} 
		if ( ch == 224)
		{
			ch   =   _getch(); 
			continue; 
		}
		//输入完成
		else   if(ch   ==   0x0A   ||   ch   ==   0x0D) 
		{ 
			cout<<endl;
			break; 
		} 
		
			str+= (char)ch;
			cout <<(char)ch; 
	} 
}


/*********************************************************************************************************************
/*函数名:  InputPassword
/*输入: string & str
/*返回值: void
/*模块名: 命令模块
/*功能描述: 输入用户密码
/*
/*********************************************************************************************************************/
void   InputPassword(string & str) 
{
	char   ch = 0; 
	while(true) 
	{ 
		ch   =   _getch(); 

		//退格键
		if(ch   ==   8  ) 
		{ 
			if(str.empty()) continue ;
			str = str.substr(0,str.length() - 1);
			cout << "\b \b"; 
			continue;
		} 
       
		if ( ch == 224)
		{
			ch   =   _getch(); 
			continue; 
		}

		//输入完成
		else   if(ch   ==   0x0A   ||   ch   ==   0x0D) 
		{ 
			cout<<endl;
			break; 
		} 
			str+= ch;
			cout <<'*';
	} 
}


/*********************************************************************************************************************
/*函数名:  delSpace
/*输入: string &str
/*返回值: void
/*模块名: 命令模块
/*功能描述: 删除字符串两端空格 
/*
/*********************************************************************************************************************/
void delSpace(string &str) 
{  
     string::iterator it;
	 string::reverse_iterator rit;
     while (!str.empty())
     {
		it = str.begin();
        if (*it == ' ')
        {
			str.erase(it);
			continue;
        }
		break;
     }
	 while (!str.empty())
	 {
		 rit = str.rbegin();
		 if (*rit == ' ')
		 {
		     str = str.substr(0,str.length() - 1);
			 continue;
		 }
		 break;
	 }
}


/*********************************************************************************************************************
/*函数名: winToLinPath
/*输入: const string &winPath
/*返回值:  string 
/*模块名: 路径模块
/*功能描述: 将windows路径格式转化为linux格式
/*
/*********************************************************************************************************************/
string winToLinPath(const string &winPath) 
{
	string str = winPath ;
	size_t i = 0 ;
	if(true == str.empty()) return "";
	while(string::npos  !=   (i   =   str.find( "\\"))) 
		str.replace(i,   1,   "/");
	i = str.find(":") ;
	str.erase( 0 , ++i);
	if(true == str.empty()) str.push_back('/');
	return str ;
}


/*********************************************************************************************************************
/*函数名: linToWinPath
/*输入: const string &linuxPath
/*返回值:  string 
/*模块名: 路径模块
/*功能描述: 将linux路径格式转化为windows格式
/*
/*********************************************************************************************************************/
string linToWinPath(const string &linuxPath)
{
	string str = linuxPath ;
	size_t i = 0 ;
	if(true == str.empty()) return "";
	i = linuxPath.find("\\");
	if(i != string::npos )throw(string("illegal character \\ in the path"));
	while(string::npos  !=   (i   =   str.find( "/"))) 
		str.replace(i ,   1,   "\\");
	if (str.at(0) == '\\')
	{
		str = "C:" + str ;
	}
	delPathTail_win(str);
	return str ;
}


/*********************************************************************************************************************
/*函数名:  getFirPar
/*输入: const string &src ,  string & dest
/*返回值: bool
/*模块名: 命令解析
/*功能描述:获取命令第一个参数
/*
/*********************************************************************************************************************/
bool getFirPar(const string &src ,  string & dest)
{
     unsigned int  i = 0 ;
	  string _src = src ;
      dest.clear();
	  if(true == _src.empty()) return false ;

	  delSpace(_src);
	   if(true == _src.empty()) return false ;
	  while ( i<_src.length() )
	  {
          if (_src[i] == ' ')
          {  
                break;
          }
          dest.push_back(_src[i++]);
	  }
	  return true ;
}


/*********************************************************************************************************************
/*函数名: isNumOrAlpha
/*输入: const string &check
/*返回值:  bool
/*模块名: 初始化模块
/*功能描述: 判定是否为文字与数字
/*
/*********************************************************************************************************************/
bool isNumOrAlpha(const string &check)
{
	unsigned int i = 0 ;
    while(i<check.length())
	{
         if (0 == isalnum(check[i]))
         {
			 return false ;
         }
		 i++ ;
	}
	return true ;
}


/*********************************************************************************************************************
/*函数名:  delPathTail_linux
/*输入:string  path
/*返回值:  string 
/*模块名: 命令解析模块
/*功能描述: 将路径处理为标准形式
/*
/*********************************************************************************************************************/
string delPathTail_linux( string  path)
{ 
	delSpace(path);
   if (path.length() > 1 && path[path.length()-1] == '/')
   {
     return  path.erase(path.length()-1 , 1);
   }
   return path ;
}


/*********************************************************************************************************************
/*函数名: delPathTail_win
/*输入: string & path
/*返回值: void
/*模块名: 命令解析模块
/*功能描述: 将路径转换为标准形式
/*
/*********************************************************************************************************************/
void delPathTail_win(string & path)
{ 
	delSpace(path);
	if (path.length() > 1 && path[path.length()-1] == '\\')
	{
		path.erase(path.length()-1 , 1);
	}
}


/*********************************************************************************************************************
/*函数名: isRelatePath_linux
/*输入: string & path
/*返回值: bool
/*模块名: 命令解析模块
/*功能描述: 判断是否为相对路径
/*
/*********************************************************************************************************************/
bool isRelatePath_linux(  string & path) 
{
    delSpace(path);
	if ('/' != path[0])
	{
		return true ;
	}
	return false ;
}


/*********************************************************************************************************************
/*函数名: getFatherPath_linux
/*输入:const string & path
/*返回值:  string 
/*模块名: 路径模块
/*功能描述: 获取父目录，只能用于linux绝对路径
/*
/*********************************************************************************************************************/
string getFatherPath_linux(const string & path)
{  
	string temp = path ;
	if(temp.empty()) return"";
	//delPathTail_linux(temp);
    if(temp.length() == 1 && temp[0] == '/') return "/";
	size_t i = temp.rfind("/");
	if(i ==0) return "/";
	if(i != string ::npos)return temp.erase(i , temp.length() -i);
	return temp ;
}

/*********************************************************************************************************************
/*函数名:  reToAbsPath_linux
/*输入: string  src , const string & currPath
/*返回值:  string 
/*模块名: 路径模块
/*功能描述: linux风格相对路径转绝对路径 
/*
/*********************************************************************************************************************/
string reToAbsPath_linux(  string  src , const string & currPath)
{
	if (true == src.empty())
	{
		return  currPath ;
	}
	
	string tempPath = currPath ;
	if(!src.empty() && src[0] == '/') 
	{
          tempPath = "/";
	}
	
// 	if (src[0] == '/')
// 	{
// 		return src ;
// 	}
	queue<string> pathlist ;
	size_t i = 0 ;
	char c = '/';
	size_t j = src.find(c);
	if(j == string ::npos) pathlist.push(src);
	while(j != string ::npos)
	{
		pathlist.push(src.substr(i , j-i));
		i =++ j;
		j = src.find(c ,j);
		if (j == string::npos)
		{
			pathlist.push(src.substr(i , src.length()));
		}
	}

	while(!pathlist.empty())
	{
		string sr = pathlist.front();
		pathlist.pop();
		if(true == sr.empty() ) continue ;
		if (sr == ".")
		{
			continue;
		}
		if (sr == "..")
		{
			if(tempPath == "/") 
			{
				throw(string("root directory do not have parents directory !"));
			}
			i = tempPath.rfind("/");
			if(i == 0)   
			{
				tempPath  = "/"; 
				continue;
			}
			tempPath.erase(i , tempPath.length());
			continue;
		}
		if(tempPath == "/")    tempPath = tempPath  + sr ;
		else  tempPath = tempPath + "/" + sr ;
	}
	return tempPath ;
}


/*********************************************************************************************************************
/*函数名:   isValidstr
/*输入:const string & str
/*返回值: bool
/*模块名: 路径模块
/*功能描述: 判定是否是合法路径
/*
/*********************************************************************************************************************/
bool isValidstr(const string & str)
{
	size_t i = 0 ;
	for (; i < str.length(); ++i)
	{
		if (str[i] == '\\' || str[i] == '/' ||str[i] ==':' || 
			str[i] =='*' ||str[i] == '?' || str[i] == '\"' || str[i] =='<' ||
			str[i] =='>' ||str[i] ==' '||str[i] =='|'||str[i] =='\''||str[i] ==';')
		{
			return true ;
		}
	}
	return false ;
}


/*********************************************************************************************************************
/*函数名:  getfatherDir
/*输入: string  dir
/*返回值:  string 
/*模块名: 目录模块
/*功能描述: 获得windows路径父目录
/*
/*********************************************************************************************************************/
string getfatherDir( string  dir) 
{ 
	delPathTail_win(dir);
	size_t i = dir.rfind( '\\',dir.length());
	if (i != string::npos)
    {
		return (dir.substr(0,i));
    }
	 return dir ;
}


/*********************************************************************************************************************
/*函数名: getLastPath_linux
/*输入:  string & src
/*返回值:  string 
/*模块名: 路径模块
/*功能描述:linux风格获取最后一级目录名 用于绝对路径
/*
/*********************************************************************************************************************/
string getLastPath_linux( string & src )
{
     string temp = src ;
	// delPathTail_linux(temp);
	 if(src.length() == 1) return temp ;
	 size_t i = temp.rfind('/' );
     temp = temp.substr(i+1,temp.length());
	 return temp;
}


/*********************************************************************************************************************
/*函数名:  inttostring
/*输入: long int size
/*返回值:  string 
/*模块名: 字符转换模块
/*功能描述: int型数据转化为string型
/*
/*********************************************************************************************************************/
string inttostring(long int size)
{
	stringstream sstr;
	int i,j;
	string ac,temp;
	sstr<<size;
	sstr>>ac;
	i=ac.size()/3;
	j=ac.size()-i*3;
	i-=(3-j)/3;
	temp=ac.substr(0,j+(3-j)/3*3);
	for (int z=0;z<i;z++)
	{
		temp=temp+","+ac.substr(j+(3-j)/3*3+z*3,3);
	}
	return temp;
}


/*********************************************************************************************************************
/*函数名:  fillstring
/*输入: string strpth,int maxlength
/*返回值:  string 
/*模块名: 字符转换模块
/*功能描述: 用空格填充字符串至某一长度
/*
/*********************************************************************************************************************/
string fillstring(string strpth,int maxlength)
{
	while (strpth.length()<maxlength)
	{
		strpth+=" ";
	}
	return strpth;
}


/*********************************************************************************************************************
/*函数名: timechange(
/*输入: time_t time
/*返回值:  string 
/*模块名: 字符转换模块
/*功能描述: time_t时间转化
/*
/*********************************************************************************************************************/
string timechange(time_t time) 
{
	struct tm*now;
	now=localtime(&time);
	stringstream sstr;
	sstr<<now->tm_year+1900;
	sstr<<"/";
	if (now->tm_mon < 9)   sstr<<'0' ;
    sstr<<now->tm_mon+1;
	sstr<<"/";
	if(now->tm_mday < 10) sstr<<'0';
	sstr<<now->tm_mday;
	string ac;
	sstr>>ac;
	return ac;
}


/*********************************************************************************************************************
/*函数名:  maxfront
/*输入:vector<string> strs ,  string front ,  vector<string> &haveFronts , int & amount
/*返回值:  string 
/*模块名: 智能提示模块
/*功能描述: 在一组字符串中搜索包含某一个前缀的最大公共前缀，是智能提示的核心函数
/*
/*********************************************************************************************************************/
string maxfront( vector<string> strs ,  string front ,  vector<string> &haveFronts , int & amount )
{

	haveFronts.clear();
	vector<string>::iterator it;
	size_t i  ;
	amount = 0;
	for (it = strs.begin() ; it != strs.end() ; it++)//过滤不包含前缀的字符串
	{
		i = (*it).find(front);
		if (i ==0)
		{
			haveFronts.push_back((*it));
		}
	}
	if(true == haveFronts.empty())
	{
		amount = 0;
		return "";
	}
	if (haveFronts.size() == 1)
	{
		amount = 1;
		return haveFronts[0];
	}
	
	if(haveFronts[0] == front) 
	{
		amount = haveFronts.size();
		return front ;
	}
	i  =front.length()  ;
	string temp = front ; //获取最长公共前缀，并将满足条件的字符串填充至字符串数组中
	while(i < haveFronts[0].length())
	{   
		temp = temp + haveFronts[0].at(i);
		for (it = haveFronts.begin()+1 ;  it != haveFronts.end() ; it++)
		{//遍历字符数组
			if ((*it).find(temp) == string::npos)
			{
				amount = haveFronts.size();
				return temp.erase(temp.length()-1) ;
			}
		}
		i ++ ;
	}
	amount = haveFronts.size();
	return haveFronts[0];
}