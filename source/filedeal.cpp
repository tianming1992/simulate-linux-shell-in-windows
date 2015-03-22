/*********************************************************************************************************************
/*文件名:  filedeal.cpp
/*模块名:目录文件
/*作者:    邹天明&戴潜
/*日期:    2011.10.25
/*功能:    程序主要部分 处理文件目录
/*
/*********************************************************************************************************************/

#include "filedeal.h"
#include <stack>
#include <tchar.h>

#define  _MAX_FILENAME_   100


/*********************************************************************************************************************
/*函数名: mkSingleDir
/*输入:  const string &dir
/*返回值:    void
/*模块名: 目录模块
/*功能描述: 新建目录
/*
/*********************************************************************************************************************/
void mkSingleDir( const string &dir)
{
	if (mkdir(dir.c_str()) == -1)
	{ 
		string str = "make dir of" + dir + "error!"; 
		throw(str);
	}
	return ;
}


/*********************************************************************************************************************
/*函数名: mkMassDir
/*输入:  const string &dir
/*返回值:    void
/*模块名: 目录模块
/*功能描述: 生成多级目录
/*
/*********************************************************************************************************************/
void mkMassDir(const string &dir)  
{  
	stack<string> dirs ;
	string faStr = getfatherDir(dir) ;
	dirs.push(dir);
	while(_access(faStr.c_str(),0) != 0)//父目录不存在，将父目录入栈
	 {
         dirs.push(faStr);
		 faStr = getfatherDir(faStr); //获取更上一级父目录
	 }
	while(false == dirs.empty())
	{
		mkSingleDir(dirs.top());
		dirs.pop();
	}
}


/*********************************************************************************************************************
/*函数名: cstrTostr
/*输入:  CString& csstr
/*返回值:    void
/*模块名: 字符转换模块
/*功能描述: cstr型转化为str
/*
/*********************************************************************************************************************/
string cstrTostr(CString& csstr) 
{ 
	string str; 
	USES_CONVERSION; 
	str=T2A(csstr); 
	return str; 
}  


/*********************************************************************************************************************
/*函数名:  strToCstr
/*输入:  string strTemp
/*返回值:  CString 
/*模块名: 字符转换模块
/*功能描述: str型转化为Cstr
/*
/*********************************************************************************************************************/
CString strToCstr(string strTemp)
{
	return strTemp.c_str();
}


/*********************************************************************************************************************
/*函数名: isPath
/*输入:  string strpath
/*返回值:  bool
/*模块名: 复制模块
/*功能描述: 判断是否为目录
/*
/*********************************************************************************************************************/
bool isPath(string strpath)
{
	CFileFind finder;
	CString path = strToCstr(strpath);
	BOOL working = finder.FindFile(path + "\\*.*");
	if(strpath == "c:" || strpath == "C:" || strpath == "c:\\" || strpath == "C:\\")
		return true ;
	while (working)
	{
		working = finder.FindNextFile();
		if (finder.IsDots())
			return true;
	}
	return false;
}


/*********************************************************************************************************************
/*函数名:  getFileName
/*输入:  const string& s
/*返回值:   string
/*模块名: 文件查找模块
/*功能描述:  获得文件的文件名
/*
/*********************************************************************************************************************/
string getFileName(const string& s)  
{
	char sep='/';

#ifdef _WIN32
	sep='\\';
#endif 

	size_t i=s.rfind(sep,s.length());
	if (i !=string::npos)
	{
		return(s.substr(i+1,s.length()-i));
	}
	return(s);
}


/*********************************************************************************************************************
/*函数名: getFileName_win
/*输入:  const string& s
/*返回值:  string
/*模块名: 文件查找模块
/*功能描述: 获得文件的文件名
/*
/*********************************************************************************************************************/
string getFileName_win(const string& s)   
{
	char sep='\\';
    size_t i=s.rfind(sep,s.length());
	if (i !=string::npos)
	{
		return(s.substr(i+1,s.length()-i));
	}
	return(s);
}


/*********************************************************************************************************************
/*函数名:  getPathName
/*输入: const string &s
/*返回值:   string
/*模块名: 初始化模块
/*功能描述: 获得文件的路径
/*
/*********************************************************************************************************************/
string getPathName(const string &s)   
{
	char sep='/';

#ifdef _WIN32
	sep='\\';
#endif 

	size_t i=s.rfind(sep,s.length());
	if (i !=string::npos)
	{
		return(s.substr(0,i));
	}
	return(" ");
}


/*********************************************************************************************************************
/*函数名:   getFileExt
/*输入:  const string &s
/*返回值:  string
/*模块名:  文件查找模块
/*功能描述: 获得文件扩展名
/*
/*********************************************************************************************************************/
string getFileExt(const string &s)
{
	size_t i=s.rfind('.',s.length());
	if (i !=string::npos)
	{
		return(s.substr(i+1,s.length()-i));
	}
	return(" ");
}


/*********************************************************************************************************************
/*函数名:   makenewfile
/*输入: vector<string> stringVector,string filename
/*返回值:   string
/*模块名:  文件查找模块
/*功能描述: 查找是否有相同文件名的文件，返回最新的版本名
/*
/*********************************************************************************************************************/
string makenewfile(vector<string> stringVector,string filename)   
{
	vector<string>::iterator iter;
	int i=2;
	stringstream sstr1;
	string edit1;
	stringstream sstr;
	string edit;
	string newrelease;
	string getfilename;
	string getfileext;
	size_t z=filename.rfind('.',filename.length());
	size_t j=filename.rfind('.',filename.length());
	if (j!=string::npos)
	{
		getfilename=filename.substr(0,j);
	}
	else
	{
		getfilename=getFileName(filename);
	}
	if (z!=string::npos)
	{
		getfileext=filename.substr(z,filename.length()-z);
	}
	else
	{
		getfileext=getFileExt(filename);
	}
	if (find(stringVector.begin(),stringVector.end(),filename)==stringVector.end())    //如果无同名文件
	{
		return (getfilename+getfileext);
	}
	else
	{    
		newrelease=getfilename+"(2)";
		if (z!=string::npos)
		{  //有同名文件，遍历查找直至最新版本名
			while(find(stringVector.begin(), stringVector.end(),newrelease+getfileext)!=stringVector.end()) 
			{ 
				i++;
				sstr<<i;
				sstr>>edit;
				newrelease=getfilename+"("+edit+")";
				sstr.clear();
			} 
		}
		else
		{//有同名文件，遍历查找直至最新版本名
			while(find(stringVector.begin(), stringVector.end(),newrelease)!=stringVector.end()) 
			{ 
				i++;
				sstr<<i;
				sstr>>edit;
				newrelease=getfilename+"("+edit+")";
				sstr.clear();
			}
		} 
		sstr1<<i;
		sstr1>>edit1;
		return (getfilename+"("+edit1+")"+getfileext);
	}


}


/*********************************************************************************************************************
/*函数名:  FindAllFile
/*输入:  string strpath, vector<string> &allFile,int count
/*返回值:    void
/*模块名:  文件查找模块
/*功能描述: 在目录下查找文件，cout为1。查找文件包括目录下的子目录为0则不包括
/*
/*********************************************************************************************************************/
void FindAllFile(string strpath, vector<string> &allFile,int count)   
{
	CFileFind finder;
	struct stat fileInfo;
	string theFileName;
	string theFilePath;
	CString path = strToCstr(strpath);
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
				FindAllFile(cstrTostr(finder.GetFilePath()), allFile,count);
			}
		} 
		else               //当前找到的是文件
		{
			theFileName=cstrTostr( finder.GetFileName());           //获取文件名
			theFilePath=cstrTostr(finder.GetFilePath());            //获取文件路径
			if ( stat(theFilePath.c_str(),&fileInfo) != 0)          //获取文件信息
			{
				throw(string("get file info error"));
			}
			allFile.push_back(theFileName);              //帮当前文件名加入容器
		}
	}
}


/*********************************************************************************************************************
/*函数名: readFile
/*输入: string strpath, string sourpath
/*返回值:   int 
/*模块名:  文件查找模块
/*功能描述:在目录下查找是否有同名文件
/*
/*********************************************************************************************************************/
int readFile(string strpath, string sourpath)                   
{
	CFileFind finder;
	string sfilename,dfilename;
	CString path = strToCstr(strpath);
	BOOL working = finder.FindFile(path + "\\*.*");
	while (working)                                //遍历该目录
	{
		working = finder.FindNextFile();              
		if (finder.IsDots())
			continue;
		if (finder.IsDirectory())                 
		{
		} 
		else                         //当前找到的是文件
		{   
			sfilename=getFileName(sourpath);
			dfilename=cstrTostr( finder.GetFileName());
			if (dfilename==sfilename)              //有同名文件
			{
				return 1;
			}

		}
	}
	return 0;


}


/*********************************************************************************************************************
/*函数名: copyFile
/*输入: const string & src , const string & dest
/*返回值:    void
/*模块名: 复制模块
/*功能描述: 复制文件到指定目录
/*
/*********************************************************************************************************************/
void copyFile(const string & src , const string & dest)         
{ 
	locale loc = locale::global(locale(""));
	ifstream infile(src.c_str() , ios::in |ios::binary);
	if (! infile)
	{ 
		string  str= "open file  error ";
		throw(str);
	}
	ofstream outfile(dest.c_str() , ios::out|ios::binary);
	if (! outfile)
	{ 
		string  str= "open file  error ";
		throw(str);
	}
	locale::global(loc);

	char buf[_MAX_FILENAME_];
	do 
	{
		infile.read(&buf[0],_MAX_FILENAME_);
		outfile.write(&buf[0],infile.gcount());
	} while (infile.gcount() > 0);

	infile.close();
	outfile.close();
}


/*********************************************************************************************************************
/*函数名: readMulu
/*输入:  string strpath, string sourpath
/*返回值:   int
/*模块名: 初始化模块
/*功能描述: 查看指定目录是否
/*
/*********************************************************************************************************************/
int readMulu(string strpath, string sourpath)        
{
	CFileFind finder;
	string sfilename,dfilename;
	sfilename=getFileName(strpath);
	CString path = strToCstr(sourpath);
	BOOL working = finder.FindFile(path + "\\*.*");
	while (working)
	{
		working = finder.FindNextFile();
		if (finder.IsDots())
			continue;
		if (finder.IsDirectory())
		{
			dfilename=cstrTostr( finder.GetFileName());
			if ( dfilename==sfilename)
			{
				return 1;
			}
		} 
		else 
		{   
		}
	}
	return 0;
}


/*********************************************************************************************************************
/*函数名:  copymulu
/*输入:  string strpath, string destpath,int flag , int choose
/*返回值:    void
/*模块名: 复制模块
/*功能描述: 复制目录 
/*
/*********************************************************************************************************************/
void copymulu(string strpath, string destpath,int flag , int choose)
{
	CFileFind finder;
	FILE *fp1,*fp2;
	struct stat fileInfo;
	string destpath1;
	string destpath2;
	string destpath3;
	string sourpath1;
	string sourpath2;
	string mainpath;
	string lastfilename;
	vector<string>nameVector;
	mainpath=destpath+"\\"+getFileName(strpath);
	mkdir(mainpath.c_str());
	CString sourpath = strToCstr(strpath);
	BOOL working = finder.FindFile(sourpath + "\\*.*");
	while (working)
	{
		working = finder.FindNextFile();
		if (finder.IsDots())
			continue;
		if (finder.IsDirectory())
		{
			if (flag==1)
			{
				copymulu( cstrTostr(finder.GetFilePath()), mainpath+"\\",1 ,choose);
			}
			else
			{
				if (readMulu( cstrTostr(finder.GetFilePath()),destpath))
				{
					copymulu( cstrTostr(finder.GetFilePath()), mainpath+"\\",0 ,choose);
				}
				else
				{
					copymulu( cstrTostr(finder.GetFilePath()), mainpath+"\\",0,choose);
				}
			}
		} 
		else  
		{
			destpath1=mainpath+"\\"+cstrTostr(finder.GetFileName());
			sourpath1=cstrTostr(finder.GetFileName());
			FILE *fp3;
			if (readFile(mainpath,sourpath1))
			{   
				if (choose == CP_)
				{
					cout<<"合并过程中有重名文件: "<<sourpath1 <<endl;
					cout<<"1.  替换"<<endl;
					cout<<"2.  取消复制"<<endl;
					cout<<"3.  复制但不替换   ";
				}
				else{
					cout<<"合并过程中有重名文件:"<<sourpath1<<endl;
					cout<<"1.  替换"<<endl;
					cout<<"2.  取消移动"<<endl;
					cout<<"3.  移动但不替换   ";
				}
			
				int choice;
				cin>>choice;
				if (choice==1)
				{
					//destpath3=mainpath+"\\"+getFileName(sourpath1);
					if ((fp3=fopen(destpath1.c_str(),"wt++"))==NULL)
					{
						throw(string("open file error !"));
					}
					copyFile(cstrTostr(finder.GetFilePath()),destpath1);
					fclose(fp3);
					//return ;
				}
				
				if (choice==3)    
				{
					FindAllFile(mainpath,nameVector,0);
					lastfilename=makenewfile(nameVector,getFileName(sourpath1));
					destpath=mainpath+"\\"+lastfilename;
					if ((fp3=fopen(destpath.c_str(),"wt++"))==NULL)
					{
						throw(string("open file error !"));
					}
					copyFile(cstrTostr(finder.GetFilePath()),destpath) ;
					fclose(fp3);
					//return ;
				}             
                 if(choice == 2)	throw(choice) ;
			}
			else
			{
				copyFile(cstrTostr(finder.GetFilePath()),destpath1);
			}
		}
	}
}


/*********************************************************************************************************************
/*函数名: delFileMulu
/*输入: string strpath, vector<string> &allFile
/*返回值:    void
/*模块名: 复制模块
/*功能描述: 删除文件或目录 
/*
/*********************************************************************************************************************/
void delFileMulu(string strpath, vector<string> &allFile)
{
	CFileFind finder;
	string theFilePath;
	string filetodel;
//	int i;
	CString path = strToCstr(strpath);
	BOOL working = finder.FindFile(path + "\\*.*");
	while (working)
	{
		working = finder.FindNextFile();
		if (finder.IsDots())
			continue;
		if (finder.IsDirectory())
		{
			delFileMulu(cstrTostr(finder.GetFilePath()),allFile);				
		} 
		else 
		{
			filetodel=cstrTostr(finder.GetFilePath());
			if(remove(filetodel.c_str())==-1)
			{
				cout<<"delete file error"<<endl;
			}
		}
	}
	allFile.push_back(strpath);

}


/*********************************************************************************************************************
/*函数名: isUpPath_Linux
/*输入: const string  &path , const string & currPath
/*返回值:   bool
/*模块名: 路径模块
/*功能描述:判断目录path是否为currpath上层目录  就用于linux绝对路径
/*
/*********************************************************************************************************************/


bool isUpPath_Linux(const string  &path , const string & currPath)
{
    if(path.length() < currPath.length())
	{
		size_t i = currPath.find(path , 0);
		if (i ==0 )return true ;
	}
	return false ;
}


/*********************************************************************************************************************
/*函数名:  FindFileName
/*输入:  string strpath,vector<string>&fileName
/*返回值:    void
/*模块名: 文件查找模块
/*功能描述: 从文件路径中获得文件名 
/*
/*********************************************************************************************************************/
void FindFileName(string strpath,vector<string>&fileName)
{
	CFileFind finder;
	string thefile;
    CString path = strToCstr(strpath);
	long size=0,size1=0;
	BOOL working = finder.FindFile(path + "\\*.*");
	while (working)
	{
		working = finder.FindNextFile();
		if (finder.IsDots())
			continue;
		else
		{
			thefile=cstrTostr( finder.GetFileName());
			thefile=fillstring(thefile,20);
			fileName.push_back(thefile);
		}
	}
}


/*********************************************************************************************************************
/*函数名: getFileOrFolder
/*输入:  string strpath,vector<string>&fileName , int choose
/*返回值:    void
/*模块名: 初始化模块
/*功能描述: 在指定目录下寻找文件或文件夹
/*
/*********************************************************************************************************************/
void getFileOrFolder(string strpath,vector<string>&fileName , int choose)
{
	CFileFind finder;
	string thefile;
	CString path = strToCstr(strpath);
	long size=0,size1=0;
	BOOL working = finder.FindFile(path + "\\*.*");
	while (working)
	{  
		working = finder.FindNextFile();
     	if (finder.IsDots())
			continue;
		if (finder.IsDirectory())
		{  
			DWORD isHidden=GetFileAttributes(finder.GetFilePath()); //过滤隐藏文件
			if ( (isHidden & FILE_ATTRIBUTE_HIDDEN)) continue ;
			if (choose == FOLDER_ONLY || choose == FILE_AND_FOLDER)
			{
				fileName.push_back(cstrTostr(finder.GetFileName()) + '/' );
			}
						
		} 
		else 
		{  
			DWORD isHidden=GetFileAttributes(finder.GetFilePath()); //过滤隐藏文件
			if ( (isHidden & FILE_ATTRIBUTE_HIDDEN)) continue ;
			if (choose == FILE_ONLY || choose == FILE_AND_FOLDER)
			{
				fileName.push_back(cstrTostr(finder.GetFileName()));
			}
        }
	}
}


/*********************************************************************************************************************
/*函数名:   FindFileSize
/*输入:  string strpath
/*返回值:   long
/*模块名: 文件查找模块
/*功能描述:获得文件夹大小 
/*
/*********************************************************************************************************************/
long FindFileSize(string strpath)
{
	CFileFind finder;
	struct stat fileInfo;
	CString path = strToCstr(strpath);
	long size=0;
	BOOL working = finder.FindFile(path + "\\*.*");
	while (working)
	{
		working = finder.FindNextFile();
		if (finder.IsDots())
			continue;
		if (finder.IsDirectory())
		{
			size+=FindFileSize(cstrTostr(finder.GetFilePath()));
		} 
		else    
		{
			if ( stat(cstrTostr(finder.GetFilePath()).c_str(),&fileInfo) != 0)
			{
			    continue; //跳过无法获得大小的文件
			}
			size+=fileInfo.st_size;
		}
	}
	return size;
}


/*********************************************************************************************************************
/*函数名: FindFileInfo
/*输入: string strpath, multimap<string,string> &allFile
/*返回值:    void
/*模块名: 文件查找模块
/*功能描述: 获得文件或目录的信息 
/*
/*********************************************************************************************************************/
void FindFileInfo(string strpath, multimap<string,string> &allFile)
{
	CFileFind finder;
	string theFile;
	long tempsize;
	struct stat fileInfo;

	string theFiletime;
	string theFileName;
	string theFileSize;
	string theFiletype;
	typedef std::multimap<string,string> mymap;

	CString path = strToCstr(strpath);
	long size=0,size1=0;
	BOOL working = finder.FindFile(path + "\\*.*");
	while (working)                                  //遍历当前目录
	{
		working = finder.FindNextFile();
		if (finder.IsDots())
			continue;
		if (finder.IsDirectory())                    //寻找到子目录时
		{    
			DWORD isHidden=GetFileAttributes(finder.GetFilePath()); //过滤隐藏文件
		     if ( (isHidden & FILE_ATTRIBUTE_HIDDEN)) continue ;
			if ( stat(cstrTostr(finder.GetFilePath()).c_str(),&fileInfo )!= 0)
			{
				 continue; //跳过无法获得信息的文件
			}
			theFiletime=fillstring(timechange(fileInfo.st_mtime),20);          //获取上次修改时间
			string nameToadd = cstrTostr( finder.GetFileName());
			theFileName=fillstring(cstrTostr( finder.GetFileName()),20);       //获取文件名
			size1+=FindFileSize(cstrTostr(finder.GetFilePath()));      //获取文件大小
			//转换文件大小单位为KB
			if(size1==0)
			{
				theFileSize =fillstring("0" ,20);  
			}
			else if (size1<=1024)
			{
				theFileSize=fillstring("1",20);
			}
			else
			{
				tempsize=size1/1024;
				if (size1%1024!=0)
				{
					tempsize+=1;
				}
				theFileSize=fillstring(inttostring(tempsize),20);
			}
			theFiletype="<DIR>";
			theFile=theFileName+theFileSize+theFiletime+theFiletype;      //把文件信息整合成一个字符串
			allFile.insert(mymap::value_type(nameToadd,theFile));      //保存当前文件所有信息，比较的关键字为文件名
			size1=0;
		} 
		else                       //寻找到文件时
		{  
			string nameToadd = cstrTostr( finder.GetFileName());
			DWORD isHidden=GetFileAttributes(finder.GetFilePath()); //过滤隐藏文件
			if ( (isHidden & FILE_ATTRIBUTE_HIDDEN)) continue ;
			theFileName=fillstring(cstrTostr( finder.GetFileName()),20);
			if ( stat(cstrTostr(finder.GetFilePath()).c_str(),&fileInfo )!= 0)
			{
				 continue; //跳过无法获得信息的文件
			}
			theFiletime=fillstring(timechange(fileInfo.st_mtime),20);
			if(fileInfo.st_size==0)
			{
				theFileSize =fillstring("0" ,20);  
			}
			else if (fileInfo.st_size<=1024)
			{
				theFileSize =fillstring("1" ,20); 
			}
			else
			{
				tempsize=fileInfo.st_size/1024;
				if (fileInfo.st_size%1024!=0)
				{
					tempsize+=1;
				}
				theFileSize=fillstring(inttostring(tempsize),20);
			}
			theFiletype="";
			theFile=theFileName+theFileSize+theFiletime+theFiletype;
			allFile.insert(mymap::value_type(nameToadd ,theFile));
		}
	}
}


/*********************************************************************************************************************
/*函数名:  cmdKind
/*输入:  string cmd 
/*返回值:  int
/*模块名: 命令模块
/*功能描述: 判断命令的参数类型
/*
/*********************************************************************************************************************/
int cmdKind(string cmd )
{
	if (cmd == "cp" || cmd == "mv" || cmd == "rm" || cmd == "cat" || cmd == ">")
	{
         return FILE_AND_FOLDER ;
	}
	if (cmd == "cd" || cmd == "mkdir" || cmd == "find" ||cmd == "ls")
	{
		return FOLDER_ONLY ;
	}
	return 0 ;
}


/*********************************************************************************************************************
/*函数名:  issysCmd
/*输入:  string cmd
/*返回值:   bool
/*模块名: 命令模块
/*功能描述: 判断字符串是否为系统命令
/*
/*********************************************************************************************************************/
bool issysCmd(string cmd )
{
	if (cmd == "ls" || cmd == "cp" || cmd == "mv" ||
		cmd == "rm" || cmd == "cat" || cmd == ">" || cmd == "cd" || cmd == "mkdir" || cmd == "find")
	{
		return true ;
	}
	
	return false ;
}


/*********************************************************************************************************************
/*函数名:  _sameLowerAndUp
/*输入:  string path
/*返回值:   bool 
/*模块名: 路径模块
/*功能描述: 判断路径大小写是否相同，需要传入windows绝对路径
/*
/*********************************************************************************************************************/
bool  _sameLowerAndUp(string path)
{
	if (path == "C:" || path == "c:" || path == "c:\\" ||path == "C:\\")
    {
		return true ;
    }
    string parPath = getfatherDir(path);
	
	delPathTail_win(parPath);
	CFileFind finder;
	string name;
	CString cstrpath = strToCstr(parPath);
    BOOL working = finder.FindFile(cstrpath + "\\*.*");
	
	while (working)
	{
		working = finder.FindNextFile();
		if (finder.IsDots())
			continue;
		else
		{
			 name=cstrTostr( finder.GetFilePath());
			 if(name == path)
				 return true ;
		}
	}
	return false ;
}


/*********************************************************************************************************************
/*函数名: sameLowerAndUp
/*输入: string path
/*返回值:  bool 
/*模块名:路径模块
/*功能描述: 统一路径
/*
/*********************************************************************************************************************/
bool sameLowerAndUp(string path)
{
	if (path == "C:" || path == "c:" || path == "c:\\" ||path == "C:\\")
	{
		return true ;
	}
    
	 while(true )
	 {   
		 if (path == "C:" || path == "c:" || path == "c:\\" ||path == "C:\\")
		 {
			 return true ;
		 }
		  if (false == _sameLowerAndUp(path)) return false ;
	         path = getfatherDir(path);
			 delPathTail_win(path);
			 continue; 
		 }
}