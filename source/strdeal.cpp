/*********************************************************************************************************************
/*�ļ���:  strdeal.cpp
/*ģ����: �ַ�����
/*����:    ������&��Ǳ
/*����:    2011.10.25
/*����:     ��������ַ�������
/*
/*********************************************************************************************************************/
#include "strdeal.h"



/*********************************************************************************************************************
/*������: InputUserName
/*����: string & str
/*����ֵ:  void 
/*ģ����: �û�ģ��
/*��������: �����û���
/*
/*********************************************************************************************************************/
void   InputUserName(string & str) 
{
	int ch  = 0; 
	while(true) 
	{ 
		ch   =   _getch(); 
     	//�˸��
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
		//�������
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
/*������:  InputPassword
/*����: string & str
/*����ֵ: void
/*ģ����: ����ģ��
/*��������: �����û�����
/*
/*********************************************************************************************************************/
void   InputPassword(string & str) 
{
	char   ch = 0; 
	while(true) 
	{ 
		ch   =   _getch(); 

		//�˸��
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

		//�������
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
/*������:  delSpace
/*����: string &str
/*����ֵ: void
/*ģ����: ����ģ��
/*��������: ɾ���ַ������˿ո� 
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
/*������: winToLinPath
/*����: const string &winPath
/*����ֵ:  string 
/*ģ����: ·��ģ��
/*��������: ��windows·����ʽת��Ϊlinux��ʽ
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
/*������: linToWinPath
/*����: const string &linuxPath
/*����ֵ:  string 
/*ģ����: ·��ģ��
/*��������: ��linux·����ʽת��Ϊwindows��ʽ
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
/*������:  getFirPar
/*����: const string &src ,  string & dest
/*����ֵ: bool
/*ģ����: �������
/*��������:��ȡ�����һ������
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
/*������: isNumOrAlpha
/*����: const string &check
/*����ֵ:  bool
/*ģ����: ��ʼ��ģ��
/*��������: �ж��Ƿ�Ϊ����������
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
/*������:  delPathTail_linux
/*����:string  path
/*����ֵ:  string 
/*ģ����: �������ģ��
/*��������: ��·������Ϊ��׼��ʽ
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
/*������: delPathTail_win
/*����: string & path
/*����ֵ: void
/*ģ����: �������ģ��
/*��������: ��·��ת��Ϊ��׼��ʽ
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
/*������: isRelatePath_linux
/*����: string & path
/*����ֵ: bool
/*ģ����: �������ģ��
/*��������: �ж��Ƿ�Ϊ���·��
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
/*������: getFatherPath_linux
/*����:const string & path
/*����ֵ:  string 
/*ģ����: ·��ģ��
/*��������: ��ȡ��Ŀ¼��ֻ������linux����·��
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
/*������:  reToAbsPath_linux
/*����: string  src , const string & currPath
/*����ֵ:  string 
/*ģ����: ·��ģ��
/*��������: linux������·��ת����·�� 
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
/*������:   isValidstr
/*����:const string & str
/*����ֵ: bool
/*ģ����: ·��ģ��
/*��������: �ж��Ƿ��ǺϷ�·��
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
/*������:  getfatherDir
/*����: string  dir
/*����ֵ:  string 
/*ģ����: Ŀ¼ģ��
/*��������: ���windows·����Ŀ¼
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
/*������: getLastPath_linux
/*����:  string & src
/*����ֵ:  string 
/*ģ����: ·��ģ��
/*��������:linux����ȡ���һ��Ŀ¼�� ���ھ���·��
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
/*������:  inttostring
/*����: long int size
/*����ֵ:  string 
/*ģ����: �ַ�ת��ģ��
/*��������: int������ת��Ϊstring��
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
/*������:  fillstring
/*����: string strpth,int maxlength
/*����ֵ:  string 
/*ģ����: �ַ�ת��ģ��
/*��������: �ÿո�����ַ�����ĳһ����
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
/*������: timechange(
/*����: time_t time
/*����ֵ:  string 
/*ģ����: �ַ�ת��ģ��
/*��������: time_tʱ��ת��
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
/*������:  maxfront
/*����:vector<string> strs ,  string front ,  vector<string> &haveFronts , int & amount
/*����ֵ:  string 
/*ģ����: ������ʾģ��
/*��������: ��һ���ַ�������������ĳһ��ǰ׺����󹫹�ǰ׺����������ʾ�ĺ��ĺ���
/*
/*********************************************************************************************************************/
string maxfront( vector<string> strs ,  string front ,  vector<string> &haveFronts , int & amount )
{

	haveFronts.clear();
	vector<string>::iterator it;
	size_t i  ;
	amount = 0;
	for (it = strs.begin() ; it != strs.end() ; it++)//���˲�����ǰ׺���ַ���
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
	string temp = front ; //��ȡ�����ǰ׺�����������������ַ���������ַ���������
	while(i < haveFronts[0].length())
	{   
		temp = temp + haveFronts[0].at(i);
		for (it = haveFronts.begin()+1 ;  it != haveFronts.end() ; it++)
		{//�����ַ�����
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