// DatabaseCtrl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DatabaseCtrl.h"

#include "ImgRetrDlg.h"
#include "afxdialogex.h"
#include <io.h>
#include "../utilities/searchEngine.h"


// CDatabaseCtrl �Ի���

IMPLEMENT_DYNAMIC(CDatabaseCtrl, CDialogEx)

CDatabaseCtrl::CDatabaseCtrl(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDatabaseCtrl::IDD, pParent)
{

}

CDatabaseCtrl::~CDatabaseCtrl()
{
}

void CDatabaseCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDatabaseCtrl, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_NEW_DB, &CDatabaseCtrl::OnBnClickedBtnNewDb)
	ON_BN_CLICKED(IDC_BTN_OPEN_DB, &CDatabaseCtrl::OnBnClickedBtnOpenDb)
	ON_BN_CLICKED(IDC_BTN_SEL_IMG, &CDatabaseCtrl::OnBnClickedBtnSelImg)
	ON_BN_CLICKED(IDC_ADD_IMG, &CDatabaseCtrl::OnBnClickedAddImg)
END_MESSAGE_MAP()


// CDatabaseCtrl ��Ϣ�������


BOOL CDatabaseCtrl::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	GetClientRect(&m_rect);

	CRect rc;
	CWnd* pFirstCtrl = GetNextDlgTabItem(NULL);
	if(pFirstCtrl !=NULL)
	{
				
		m_vecCtrlID.push_back(pFirstCtrl->GetDlgCtrlID());
		pFirstCtrl->GetWindowRect(&rc);
		ScreenToClient(&rc);
		m_vecCtrlRect.push_back(rc);
		CWnd* pCtrl=GetNextDlgTabItem(pFirstCtrl);

		while(pCtrl!=NULL && pCtrl != pFirstCtrl)
		{        
			m_vecCtrlID.push_back(pCtrl->GetDlgCtrlID()); 
			pCtrl->GetWindowRect(&rc);
			ScreenToClient(&rc);
			m_vecCtrlRect.push_back(rc);

			pCtrl = GetNextDlgTabItem(pCtrl);
		}
	}



	//����progress bar
	CProgressCtrl *pProg=(CProgressCtrl*)GetDlgItem(IDC_PROGRESS);
	pProg->SetRange(0,10000);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CDatabaseCtrl::ChangeSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	Invalidate();
	if(nType!=SIZE_MINIMIZED ) 
	{
		if(cx>m_rect.Width() || cy>m_rect.Height())
		{
			for (int i = 0 ; i!=m_vecCtrlID.size();++i)
			{
				CRect &rec = m_vecCtrlRect[i]; 
				//pWnd->GetWindowRect(&rec);  //��ȡ�ؼ��仯ǰ�Ĵ�С
				//ScreenToClient(&rec);   //���ؼ���Сװ��λ�ڶԻ����е���������
				CRect new_rec=rec;
				new_rec.left=ceil((float)rec.left*cx/m_rect.Width());  //���ձ��������ռ����λ��
				new_rec.top=ceil((float)rec.top*cy/m_rect.Height());

				new_rec.bottom=ceil((float)rec.bottom*cy/m_rect.Height());
				new_rec.right=ceil((float)rec.right*cx/m_rect.Width());
				GetDlgItem(m_vecCtrlID[i])->MoveWindow(new_rec);   //�����ؼ�
			}
		}
		else
		{
			for (int i = 0 ; i!=m_vecCtrlID.size();++i)
			{
				CRect &rec = m_vecCtrlRect[i]; 
				//pWnd->GetWindowRect(&rec);  //��ȡ�ؼ��仯ǰ�Ĵ�С
				//ScreenToClient(&rec);   //���ؼ���Сװ��λ�ڶԻ����е���������
				CRect new_rec=rec;
				new_rec.left=floor((float)rec.left*cx/m_rect.Width());  //���ձ��������ռ����λ��
				new_rec.top=floor((float)rec.top*cy/m_rect.Height());

				new_rec.bottom=floor((float)rec.bottom*cy/m_rect.Height());
				new_rec.right=floor((float)rec.right*cx/m_rect.Width());
				GetDlgItem(m_vecCtrlID[i])->MoveWindow(new_rec);   //�����ؼ�
			}

		}
	}
}


//��ѡ���ļ��жԻ��������½��ļ��й���

void CDatabaseCtrl::OnBnClickedBtnNewDb()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	TCHAR szPath[MAX_PATH];     //���ѡ���Ŀ¼·�� 
	CString str;

	ZeroMemory(szPath, sizeof(szPath));   

	BROWSEINFO bi;   
	bi.hwndOwner = m_hWnd;   
	bi.pidlRoot = NULL;   
	bi.pszDisplayName = szPath;   
	bi.lpszTitle = _T("��ѡ���Ŀ¼��������ļ���");   
	bi.ulFlags = BIF_NEWDIALOGSTYLE|BIF_DONTGOBELOWDOMAIN|BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS;   
	bi.lpfn = NULL;   
	bi.lParam = 0;   
	bi.iImage = 0;   
	//����ѡ��Ŀ¼�Ի���
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);   

	if(SHGetPathFromIDList(lp, szPath))   
	{
		
		CFileFind finder;

		// build a string with wildcards
		CString strWildcard(szPath);
		strWildcard += _T("\\*.*");

		

		BOOL bWorking = finder.FindFile(strWildcard);

		if (bWorking ==0 )
		{
			MessageBox(_T("���벻��·��!"),_T("����"),MB_ICONERROR);
			return;
		}

		int cnt=2;
		while (bWorking )
		{
			bWorking = finder.FindNextFile();
			if (finder.IsDots())
			{
				continue;
			}			
			else if (finder.IsDirectory())
			{				
				cnt++;
				break;
			}
			else
			{
				cnt++;
				break;
			}
		}
		finder.Close();	

		if (cnt > 2  )
		{
			MessageBox(_T("����·���ǿ�!"),_T("����"),MB_ICONERROR);
			return;
		}
	}	
	else if (!szPath[0])
	{
		return;
	}
	else
	{
		//SHGetPathFromIDList �������ش���
		MessageBox(_T("ѡ���·���Ƿ�!"),_T("����"),MB_ICONERROR);
		return;
	}
	
	ShowDBNameAndLocation(szPath);

	CImgRetrDlg * pMainWnd=(CImgRetrDlg*)AfxGetMainWnd();
	string databasePath(szPath);
	DatabaseInfo *databaseInfo;

	CMutex g_mutex;

	CSingleLock singleLock(&g_mutex);
	singleLock.Lock(1);
	if (!singleLock.IsLocked())
	{
		::MessageBox(NULL,_T("��ȴ���һ��������ɣ�"),_T("����"),MB_ICONERROR);
		return ;
	}

	databaseInfo = new DatabaseInfo(databasePath,pMainWnd->searchEnine->m_featureExactor);


	pMainWnd->searchEnine->setDatabase(databaseInfo);

	singleLock.Unlock();
}




UINT ThreadNewDatabaseInfo( LPVOID pParam )
{
	extern CMutex g_mutex;	
	CSingleLock singleLock(&g_mutex);

	singleLock.Lock(1);
	if (!singleLock.IsLocked())
	{
		::MessageBox(NULL,_T("��ȴ���һ��������ɣ�"),_T("����"),MB_ICONERROR);
		return 0;
	}

	char * strDBPath = (char*)pParam;
	extern CImgRetrDlg* g_pMainWnd;
	DatabaseInfo *databaseInfo = new DatabaseInfo(string(strDBPath),g_pMainWnd->searchEnine->m_featureExactor);

	g_pMainWnd->searchEnine->setDatabase(databaseInfo);

	delete strDBPath;

	singleLock.Unlock();
	return 0;
}


void CDatabaseCtrl::OnBnClickedBtnOpenDb()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������


	TCHAR szPath[MAX_PATH];     //���ѡ���Ŀ¼·�� 
	CString str;

	ZeroMemory(szPath, sizeof(szPath));   

	BROWSEINFO bi;   
	bi.hwndOwner = m_hWnd;   
	bi.pidlRoot = NULL;   
	bi.pszDisplayName = szPath;   
	bi.lpszTitle = _T("��ѡ���Ѵ��ڵ������ļ��У�");   
	bi.ulFlags = BIF_NEWDIALOGSTYLE|BIF_DONTGOBELOWDOMAIN|BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS;   
	bi.lpfn = NULL;   
	bi.lParam = 0;   
	bi.iImage = 0;   
	//����ѡ��Ŀ¼�Ի���
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);   

	if(lp && SHGetPathFromIDList(lp, szPath))   
	{

		CFileFind finder;

		// build a string with wildcards
		CString strWildcard(szPath);
		strWildcard += _T("\\*.*");



		BOOL bWorking = finder.FindFile(strWildcard);

		if (bWorking ==0 )
		{
			MessageBox(_T("���벻��·��!"),_T("����"),MB_ICONERROR);
			return;
		}

		int cnt=2;
		while (bWorking )
		{
			bWorking = finder.FindNextFile();
			if (finder.IsDots())
			{
				continue;
			}			
			else if (finder.IsDirectory())
			{				
				cnt++;
				break;
			}
			else
			{
				cnt++;
				break;
			}
		}
		finder.Close();	

		if (cnt == 2  )
		{
			MessageBox(_T("����·��Ϊ�գ�����������!"),_T("����"),MB_ICONERROR);
			return;
		}
	}	
	else if (!szPath[0])
	{
		return;
	}
	else
	{
		MessageBox(_T("ѡ���·���Ƿ�!"),_T("����"),MB_ICONERROR);
		return;
	}


	ShowDBNameAndLocation(szPath);



	CImgRetrDlg * pMainWnd=(CImgRetrDlg*)AfxGetMainWnd();
	string databasePath(szPath);




	//�̲߳��������̺߳������ͷ�
	char * strDBPath = new char[MAXBUFSIZE];
	strcpy(strDBPath,databasePath.c_str());
	AfxBeginThread(ThreadNewDatabaseInfo,strDBPath);

}



void CDatabaseCtrl::ShowDBNameAndLocation(string _path)
{
	string folderName;
	FindFiles::getFatherPath(_path,folderName);

	CStatic * pDBNameStatic =(CStatic*) GetDlgItem(IDC_DB_NAME_SHOW);
	pDBNameStatic->SetWindowTextA(folderName.c_str());
	CStatic * pDBLocationStatic = (CStatic*)GetDlgItem(IDC_DB_LOCATION_SHOW);
	pDBLocationStatic->SetWindowTextA(_path.c_str());

}

void CDatabaseCtrl::OnBnClickedBtnSelImg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CFolderPickerDialog folderDlg;
	//folderDlg.DoModal();

	TCHAR szPath[MAX_PATH];     //���ѡ���Ŀ¼·�� 
	CString str;

	ZeroMemory(szPath, sizeof(szPath));   

	BROWSEINFO bi;   
	bi.hwndOwner = m_hWnd;   
	bi.pidlRoot = NULL;   
	bi.pszDisplayName = szPath;   
	bi.lpszTitle = _T("��ѡ���Ѵ��ڵ������ļ��У�");   
	bi.ulFlags = BIF_NEWDIALOGSTYLE|BIF_DONTGOBELOWDOMAIN|BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS;   
	bi.lpfn = NULL;   
	bi.lParam = 0;   
	bi.iImage = 0;   
	//����ѡ��Ŀ¼�Ի���
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);   

	if(lp && SHGetPathFromIDList(lp, szPath))   
	{

		CFileFind finder;

		// build a string with wildcards
		CString strWildcard(szPath);
		strWildcard += _T("\\*.*");



		BOOL bWorking = finder.FindFile(strWildcard);

		if (bWorking ==0 )
		{
			MessageBox(_T("���벻��·��!"),_T("����"),MB_ICONERROR);
			return;
		}

		int cnt=2;
		while (bWorking )
		{
			bWorking = finder.FindNextFile();
			if (finder.IsDots())
			{
				continue;
			}			
			else if (finder.IsDirectory())
			{				
				cnt++;
				break;
			}
			else
			{
				cnt++;
				break;
			}
		}
		finder.Close();	

		if (cnt == 2  )
		{
			MessageBox(_T("����·��Ϊ�գ�������ͼ��!"),_T("����"),MB_ICONERROR);
			return;
		}
	}	
	else if(!szPath[0])
	{
		return ;
	}
	else
	{
		MessageBox(_T("ѡ���·���Ƿ�!"),_T("����"),MB_ICONERROR);
		return;
	}

	m_toAddImgPath=string(szPath);


}


UINT ThreadAddImages( LPVOID pParam )
{
	//CDatabaseCtrl * pCDBctrl = (CDatabaseCtrl*)pParam;

	CImgRetrDlg  * pMainWnd=(CImgRetrDlg*)pParam;

	pMainWnd->searchEnine->AddImageToDatabase(pMainWnd->m_databaseCtrl->m_toAddImgPath);
	//pMainWnd->searchEnine.databaseInfo->AddImages(pMainWnd->m_databaseCtrl.m_toAddImgPath);
	return 0;
}


void CDatabaseCtrl::OnBnClickedAddImg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������	

	//CImgRetrDlg * pMainWnd=(CImgRetrDlg*)AfxGetMainWnd();
	extern CImgRetrDlg * g_pMainWnd;
	//pMainWnd->searchEnine.setDatabase(databaseInfo);

	if (g_pMainWnd->searchEnine->databaseInfo==NULL)
	{
		MessageBox(_T("���½�����ѡ���������ݿ⣡"),_T("����"),MB_ICONERROR);
		return;
	}


	//pMainWnd->searchEnine.databaseInfo->AddImages(m_toAddImgPath);

	AfxBeginThread(ThreadAddImages,g_pMainWnd,0,0,0,NULL);

}
