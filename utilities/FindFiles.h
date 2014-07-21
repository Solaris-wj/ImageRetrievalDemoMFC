
/************************************************************************/
/* 
 *
 * @ �ļ����� FindFiles.h
 * @ ������ ��ȡ�ļ�����
 * @ ���ߣ� ��־��
 * @ ʱ�䣺 2011.11.11
 * @ �汾�� 1.0
 *
*/
/************************************************************************/

#pragma once

#include "allInclude.h"


#include <io.h>
#include <string>
#include <vector>
#include <list>

#include <direct.h>

using namespace std;

typedef std::vector<string> FileList;
//typedef std::string string;
//typedef std::list<std::string> FileList;

class FindFiles
{

protected:

	static void normalizeDir(string& strDir); //�淶��Ŀ¼

public:
	FindFiles(void);
	~FindFiles(void);

	//�������ļ�·��
	static void findFilesInDir(FileList& listResltFiles , const string strDir = "." , const string strFileType = "*.*"); //��ȡ·���µ�ָ�������ļ�
	//�����ļ�·��
	static void findFilesInDirAndSubdirs(FileList& listResltFiles , const string strDir = "." , const string strFileType = "*.*"); //��ȡ·�������ļ���������ָ�������ļ�
	static void findSubdirsInDir(FileList& listResltFiles , const string strDir = ".");
	static void findFilesAndSubdirsInDir(FileList& listResltFiles , const string strDir = "." , const string strFileType = "*.*"); //��ȡ

	static string getFatherPath(string strDir,string &folderName=string());
};
