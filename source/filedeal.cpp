/*********************************************************************************************************************
/*�ļ���:  filedeal.cpp
/*ģ����:Ŀ¼�ļ�
/*����:    ������&��Ǳ
/*����:    2011.10.25
/*����:    ������Ҫ���� �����ļ�Ŀ¼
/*
/*********************************************************************************************************************/

#include "filedeal.h"
#include <stack>
#include <tchar.h>

#define  _MAX_FILENAME_   100


/*********************************************************************************************************************
/*������: mkSingleDir
/*����:  const string &dir
/*����ֵ:    void
/*ģ����: Ŀ¼ģ��
/*��������: �½�Ŀ¼
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
/*������: mkMassDir
/*����:  const string &dir
/*����ֵ:    void
/*ģ����: Ŀ¼ģ��
/*��������: ���ɶ༶Ŀ¼
/*
/*********************************************************************************************************************/
void mkMassDir(const string &dir)  
{  
	stack<string> dirs ;
	string faStr = getfatherDir(dir) ;
	dirs.push(dir);
	while(_access(faStr.c_str(),0) != 0)//��Ŀ¼�����ڣ�����Ŀ¼��ջ
	 {
         dirs.push(faStr);
		 faStr = getfatherDir(faStr); //��ȡ����һ����Ŀ¼
	 }
	while(false == dirs.empty())
	{
		mkSingleDir(dirs.top());
		dirs.pop();
	}
}


/*********************************************************************************************************************
/*������: cstrTostr
/*����:  CString& csstr
/*����ֵ:    void
/*ģ����: �ַ�ת��ģ��
/*��������: cstr��ת��Ϊstr
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
/*������:  strToCstr
/*����:  string strTemp
/*����ֵ:  CString 
/*ģ����: �ַ�ת��ģ��
/*��������: str��ת��ΪCstr
/*
/*********************************************************************************************************************/
CString strToCstr(string strTemp)
{
	return strTemp.c_str();
}


/*********************************************************************************************************************
/*������: isPath
/*����:  string strpath
/*����ֵ:  bool
/*ģ����: ����ģ��
/*��������: �ж��Ƿ�ΪĿ¼
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
/*������:  getFileName
/*����:  const string& s
/*����ֵ:   string
/*ģ����: �ļ�����ģ��
/*��������:  ����ļ����ļ���
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
/*������: getFileName_win
/*����:  const string& s
/*����ֵ:  string
/*ģ����: �ļ�����ģ��
/*��������: ����ļ����ļ���
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
/*������:  getPathName
/*����: const string &s
/*����ֵ:   string
/*ģ����: ��ʼ��ģ��
/*��������: ����ļ���·��
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
/*������:   getFileExt
/*����:  const string &s
/*����ֵ:  string
/*ģ����:  �ļ�����ģ��
/*��������: ����ļ���չ��
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
/*������:   makenewfile
/*����: vector<string> stringVector,string filename
/*����ֵ:   string
/*ģ����:  �ļ�����ģ��
/*��������: �����Ƿ�����ͬ�ļ������ļ����������µİ汾��
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
	if (find(stringVector.begin(),stringVector.end(),filename)==stringVector.end())    //�����ͬ���ļ�
	{
		return (getfilename+getfileext);
	}
	else
	{    
		newrelease=getfilename+"(2)";
		if (z!=string::npos)
		{  //��ͬ���ļ�����������ֱ�����°汾��
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
		{//��ͬ���ļ�����������ֱ�����°汾��
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
/*������:  FindAllFile
/*����:  string strpath, vector<string> &allFile,int count
/*����ֵ:    void
/*ģ����:  �ļ�����ģ��
/*��������: ��Ŀ¼�²����ļ���coutΪ1�������ļ�����Ŀ¼�µ���Ŀ¼Ϊ0�򲻰���
/*
/*********************************************************************************************************************/
void FindAllFile(string strpath, vector<string> &allFile,int count)   
{
	CFileFind finder;
	struct stat fileInfo;
	string theFileName;
	string theFilePath;
	CString path = strToCstr(strpath);
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
				FindAllFile(cstrTostr(finder.GetFilePath()), allFile,count);
			}
		} 
		else               //��ǰ�ҵ������ļ�
		{
			theFileName=cstrTostr( finder.GetFileName());           //��ȡ�ļ���
			theFilePath=cstrTostr(finder.GetFilePath());            //��ȡ�ļ�·��
			if ( stat(theFilePath.c_str(),&fileInfo) != 0)          //��ȡ�ļ���Ϣ
			{
				throw(string("get file info error"));
			}
			allFile.push_back(theFileName);              //�ﵱǰ�ļ�����������
		}
	}
}


/*********************************************************************************************************************
/*������: readFile
/*����: string strpath, string sourpath
/*����ֵ:   int 
/*ģ����:  �ļ�����ģ��
/*��������:��Ŀ¼�²����Ƿ���ͬ���ļ�
/*
/*********************************************************************************************************************/
int readFile(string strpath, string sourpath)                   
{
	CFileFind finder;
	string sfilename,dfilename;
	CString path = strToCstr(strpath);
	BOOL working = finder.FindFile(path + "\\*.*");
	while (working)                                //������Ŀ¼
	{
		working = finder.FindNextFile();              
		if (finder.IsDots())
			continue;
		if (finder.IsDirectory())                 
		{
		} 
		else                         //��ǰ�ҵ������ļ�
		{   
			sfilename=getFileName(sourpath);
			dfilename=cstrTostr( finder.GetFileName());
			if (dfilename==sfilename)              //��ͬ���ļ�
			{
				return 1;
			}

		}
	}
	return 0;


}


/*********************************************************************************************************************
/*������: copyFile
/*����: const string & src , const string & dest
/*����ֵ:    void
/*ģ����: ����ģ��
/*��������: �����ļ���ָ��Ŀ¼
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
/*������: readMulu
/*����:  string strpath, string sourpath
/*����ֵ:   int
/*ģ����: ��ʼ��ģ��
/*��������: �鿴ָ��Ŀ¼�Ƿ�
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
/*������:  copymulu
/*����:  string strpath, string destpath,int flag , int choose
/*����ֵ:    void
/*ģ����: ����ģ��
/*��������: ����Ŀ¼ 
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
					cout<<"�ϲ��������������ļ�: "<<sourpath1 <<endl;
					cout<<"1.  �滻"<<endl;
					cout<<"2.  ȡ������"<<endl;
					cout<<"3.  ���Ƶ����滻   ";
				}
				else{
					cout<<"�ϲ��������������ļ�:"<<sourpath1<<endl;
					cout<<"1.  �滻"<<endl;
					cout<<"2.  ȡ���ƶ�"<<endl;
					cout<<"3.  �ƶ������滻   ";
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
/*������: delFileMulu
/*����: string strpath, vector<string> &allFile
/*����ֵ:    void
/*ģ����: ����ģ��
/*��������: ɾ���ļ���Ŀ¼ 
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
/*������: isUpPath_Linux
/*����: const string  &path , const string & currPath
/*����ֵ:   bool
/*ģ����: ·��ģ��
/*��������:�ж�Ŀ¼path�Ƿ�Ϊcurrpath�ϲ�Ŀ¼  ������linux����·��
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
/*������:  FindFileName
/*����:  string strpath,vector<string>&fileName
/*����ֵ:    void
/*ģ����: �ļ�����ģ��
/*��������: ���ļ�·���л���ļ��� 
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
/*������: getFileOrFolder
/*����:  string strpath,vector<string>&fileName , int choose
/*����ֵ:    void
/*ģ����: ��ʼ��ģ��
/*��������: ��ָ��Ŀ¼��Ѱ���ļ����ļ���
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
			DWORD isHidden=GetFileAttributes(finder.GetFilePath()); //���������ļ�
			if ( (isHidden & FILE_ATTRIBUTE_HIDDEN)) continue ;
			if (choose == FOLDER_ONLY || choose == FILE_AND_FOLDER)
			{
				fileName.push_back(cstrTostr(finder.GetFileName()) + '/' );
			}
						
		} 
		else 
		{  
			DWORD isHidden=GetFileAttributes(finder.GetFilePath()); //���������ļ�
			if ( (isHidden & FILE_ATTRIBUTE_HIDDEN)) continue ;
			if (choose == FILE_ONLY || choose == FILE_AND_FOLDER)
			{
				fileName.push_back(cstrTostr(finder.GetFileName()));
			}
        }
	}
}


/*********************************************************************************************************************
/*������:   FindFileSize
/*����:  string strpath
/*����ֵ:   long
/*ģ����: �ļ�����ģ��
/*��������:����ļ��д�С 
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
			    continue; //�����޷���ô�С���ļ�
			}
			size+=fileInfo.st_size;
		}
	}
	return size;
}


/*********************************************************************************************************************
/*������: FindFileInfo
/*����: string strpath, multimap<string,string> &allFile
/*����ֵ:    void
/*ģ����: �ļ�����ģ��
/*��������: ����ļ���Ŀ¼����Ϣ 
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
	while (working)                                  //������ǰĿ¼
	{
		working = finder.FindNextFile();
		if (finder.IsDots())
			continue;
		if (finder.IsDirectory())                    //Ѱ�ҵ���Ŀ¼ʱ
		{    
			DWORD isHidden=GetFileAttributes(finder.GetFilePath()); //���������ļ�
		     if ( (isHidden & FILE_ATTRIBUTE_HIDDEN)) continue ;
			if ( stat(cstrTostr(finder.GetFilePath()).c_str(),&fileInfo )!= 0)
			{
				 continue; //�����޷������Ϣ���ļ�
			}
			theFiletime=fillstring(timechange(fileInfo.st_mtime),20);          //��ȡ�ϴ��޸�ʱ��
			string nameToadd = cstrTostr( finder.GetFileName());
			theFileName=fillstring(cstrTostr( finder.GetFileName()),20);       //��ȡ�ļ���
			size1+=FindFileSize(cstrTostr(finder.GetFilePath()));      //��ȡ�ļ���С
			//ת���ļ���С��λΪKB
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
			theFile=theFileName+theFileSize+theFiletime+theFiletype;      //���ļ���Ϣ���ϳ�һ���ַ���
			allFile.insert(mymap::value_type(nameToadd,theFile));      //���浱ǰ�ļ�������Ϣ���ȽϵĹؼ���Ϊ�ļ���
			size1=0;
		} 
		else                       //Ѱ�ҵ��ļ�ʱ
		{  
			string nameToadd = cstrTostr( finder.GetFileName());
			DWORD isHidden=GetFileAttributes(finder.GetFilePath()); //���������ļ�
			if ( (isHidden & FILE_ATTRIBUTE_HIDDEN)) continue ;
			theFileName=fillstring(cstrTostr( finder.GetFileName()),20);
			if ( stat(cstrTostr(finder.GetFilePath()).c_str(),&fileInfo )!= 0)
			{
				 continue; //�����޷������Ϣ���ļ�
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
/*������:  cmdKind
/*����:  string cmd 
/*����ֵ:  int
/*ģ����: ����ģ��
/*��������: �ж�����Ĳ�������
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
/*������:  issysCmd
/*����:  string cmd
/*����ֵ:   bool
/*ģ����: ����ģ��
/*��������: �ж��ַ����Ƿ�Ϊϵͳ����
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
/*������:  _sameLowerAndUp
/*����:  string path
/*����ֵ:   bool 
/*ģ����: ·��ģ��
/*��������: �ж�·����Сд�Ƿ���ͬ����Ҫ����windows����·��
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
/*������: sameLowerAndUp
/*����: string path
/*����ֵ:  bool 
/*ģ����:·��ģ��
/*��������: ͳһ·��
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