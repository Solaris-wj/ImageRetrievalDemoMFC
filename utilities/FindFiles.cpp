/************************************************************************/
/* 
*
* @ �ļ����� FindFiles.cpp
* @ ������ ��ȡ�ļ�����
* @ ���ߣ� ��־��
* @ ʱ�䣺 2011.11.11
* @ �汾�� 1.0
*
*/
/************************************************************************/
#include "stdafx.h"
#include "FindFiles.h"

FindFiles::FindFiles(void)
{
}

FindFiles::~FindFiles(void)
{
}


/************************************************************************/
/* 
 *
 * @ �������� getFatherPath
 * @ ������ ��ȡ����Ŀ¼���ϼ�Ŀ¼��foldername���ر���Ŀ¼������
 * @ ���ߣ� 
 * @ ʱ�䣺 2011.12.3
 *
*/
/************************************************************************/

string FindFiles::getFatherPath(string strDir,string &folderName)
{
	if (-1==_access(strDir.c_str(),0))
	{
		return "";
	}
	//string::npos
	int l_nPos1 = strDir.find_last_of("\\");
	int l_nPos2 = strDir.find_last_of("/");
	int l_nStrLen = strDir.size();
	if (l_nStrLen - 1 != l_nPos1)
	{
		folderName=strDir.substr(l_nPos1+1);
		return string(strDir.substr(0,l_nPos1-0));
	}
	else if (l_nStrLen - 1 != l_nPos2)
	{
		folderName=strDir.substr(l_nPos2+1);
		return string(strDir.substr(0,l_nPos2-1));
	}
	else
	{
		folderName=strDir;
		return strDir;
	}
}


/************************************************************************/
/* 
 *
 * @ �������� normalizeDir
 * @ ������ �淶��Ŀ¼��ʹĿ¼��β��Ϊ"\\"
 * @ ���ߣ� ��־��
 * @ ʱ�䣺 2011.12.3
 *
*/
/************************************************************************/


void FindFiles::normalizeDir( string& strDir )
{
	int l_nPos1 = strDir.find_last_of("\\");
	int l_nPos2 = strDir.find_last_of("/");
	int l_nStrLen = strDir.size();
	if ((l_nStrLen - 1 != l_nPos1) && (l_nStrLen - 1 != l_nPos2))
	{
		strDir += "\\";
	}
}

/************************************************************************/
/* 
 *
 * @ �������� findFilesInDir
 * @ ������ ��ȡĿ¼strDir�µ���������ΪstrFileType���ļ����������ļ��У�
 * @ ���ߣ� ��־��
 * @ ʱ�䣺 2011.11.11
 *
*/
/************************************************************************/

void FindFiles::findFilesInDir( FileList& listResltFiles , const string strDir /*= "."*/ , const string strFileType /*= "*.*"*/ )
{
	_finddata_t l_fileInfo;
	unsigned long l_hFile;
	string l_strDir = strDir;

	//�淶Ŀ¼��ʽ
	if (!(strDir.find_last_of("\\") == strDir.size() - 1))
	{
		l_strDir += "\\";
	}
	l_strDir += strFileType;

	//���ҵ�һ���ļ�
	l_hFile = _findfirst(l_strDir.c_str() , &l_fileInfo);
	if (-1L == l_hFile)
	{
		//û���ҵ�
		return;
	}

	if ("*.*" == strFileType)
	{
		//���������ļ�
		do 
		{
			if ((string(l_fileInfo.name) == ".") || (string(l_fileInfo.name) == ".."))
			{
				//�ϼ��ļ��У�������
			}
			else if (_A_SUBDIR == l_fileInfo.attrib)
			{
				//�ļ��У�������
			}
			else
			{
				//Ҫ���ҵ��ļ�������
				listResltFiles.push_back(string(l_fileInfo.name));
			}
		} while (0 ==_findnext(l_hFile , &l_fileInfo));

	}
	else
	{
		//����ָ����ʽ���ļ�
		do 
		{
			//Ҫ���ҵ��ļ�������
			listResltFiles.push_back(string(l_fileInfo.name));
		} while (0 ==_findnext(l_hFile , &l_fileInfo));
	}

	_findclose(l_hFile);
}

/************************************************************************/
/* 
*
* @ �������� findFilesInDirAndSubdirs
* @ ������ ��ȡĿ¼strDir������Ŀ¼�µ���������ΪstrFileType���ļ����������ļ��У�
* @ ���ߣ� ��־��
* @ ʱ�䣺 2011.11.11
*
*/
/************************************************************************/

void FindFiles::findFilesInDirAndSubdirs( FileList& listResltFiles , const string strDir /*= "." */, const string strFileType /*= "*.*"*/ )
{
	_finddata_t l_fileInfo;
	unsigned long l_hFile;
	string l_strDir = strDir;
	string l_strFileType = strFileType;

	//�淶Ŀ¼��ʽ
// 	if (!(strDir.find_last_of("\\") == strDir.size() - 1))
// 	{
// 		l_strDir += "\\";
// 	}
	FindFiles::normalizeDir(l_strDir);
	l_strDir += "*.*";

	//���ҵ�һ���ļ�
	l_hFile = _findfirst(l_strDir.c_str() , &l_fileInfo);
	if (-1L == l_hFile)
	{
		//û���ҵ�
		return;
	}

	//���¹淶Ŀ¼��ʽ
	l_strDir = strDir;
// 	if (!(strDir.find_last_of("\\") == strDir.size() - 1))
// 	{
// 		l_strDir +=  "\\";
// 	}
	FindFiles::normalizeDir(l_strDir);

	//�������ļ����ļ���
	FileList l_strSubDirs;
	findSubdirsInDir(l_strSubDirs , l_strDir);

	FileList::iterator l_iterator = l_strSubDirs.begin();
	while(l_iterator != l_strSubDirs.end())
	{
		findFilesInDirAndSubdirs(listResltFiles , l_strDir + (*l_iterator) , strFileType);
		l_iterator ++;
	}

	string l_strFileName = string(l_fileInfo.name);

	if ("*.*" == strFileType)
	{
		do
		{
			l_strFileName = string(l_fileInfo.name);
			if ((l_strFileName == ".") || (l_strFileName == ".."))
			{
				//�ϼ��ļ��У�������
			}
			else if (_A_SUBDIR == l_fileInfo.attrib)
			{
				//�ļ���ǰ���Ѿ������˴����ٴ���
			}
			else
			{
				//�ҵ��ļ�������
				listResltFiles.push_back(string(l_strDir + l_fileInfo.name));
			}
		}while(0 == _findnext(l_hFile , &l_fileInfo));
	}
	else
	{
		_findclose(l_hFile);
		l_hFile = -1L;
		l_hFile = _findfirst(string(l_strDir + strFileType).c_str() , &l_fileInfo);
		if (-1L == l_hFile)
		{
			return;
		}
		do 
		{
			listResltFiles.push_back(l_strDir + string(l_fileInfo.name));
		} while (0 == _findnext(l_hFile , &l_fileInfo));
	}

	_findclose(l_hFile);

}

/************************************************************************/
/* 
*
* @ �������� findSubdirsInDir
* @ ������ ��ȡĿ¼strDir�µ������ļ���
* @ ���ߣ� ��־��
* @ ʱ�䣺 2011.11.11
*
*/
/************************************************************************/


void FindFiles::findSubdirsInDir( FileList& listResltFiles , const string strDir /* = "."*/ )
{
	_finddata_t l_fileInfo;
	unsigned long l_hFile;
	string l_strDir = strDir;

	listResltFiles.clear();

	//�淶Ŀ¼��ʽ
	if (!(strDir.find_last_of("\\") == strDir.size() - 1))
	{
		l_strDir += "\\";
	}
	l_strDir += "*.*";

	//���ҵ�һ���ļ�
	l_hFile = _findfirst(l_strDir.c_str() , &l_fileInfo);
	if (-1L == l_hFile)
	{
		//û���ҵ�
		return;
	}
	do 
	{
		string l_strFileName = string(l_fileInfo.name);
		if ((l_strFileName == ".") || (l_strFileName == ".."))
		{
			//�ϼ��ļ��У�������
		}
		else if (_A_SUBDIR == l_fileInfo.attrib)
		{
			//�ҵ��ļ��У�����
			listResltFiles.push_back(string(l_fileInfo.name));
		}
	} while (0 == _findnext(l_hFile , &l_fileInfo));
	_findclose(l_hFile);
}


/************************************************************************/
/* 
*
* @ �������� findFilesAndSubdirsInDir
* @ ������ ��ȡĿ¼strDir�µ������ļ��к��ļ�
* @ ���ߣ� ��־��
* @ ʱ�䣺 2011.11.11
*
*/
/************************************************************************/


void FindFiles::findFilesAndSubdirsInDir( FileList& listResltFiles , const string strDir /*= "." */, const string strFileType /*= "*.*"*/ )
{
	findSubdirsInDir(listResltFiles , strDir);
	findFilesInDir(listResltFiles , strDir , strFileType);

}
