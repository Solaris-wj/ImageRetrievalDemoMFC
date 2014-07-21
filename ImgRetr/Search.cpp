// Search.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Search.h"
#include "ImageRetrieval.h"

#include "afxdialogex.h"

#include "ImgRetrDlg.h"

#include "../utilities/CvvImage.h"
typedef HRESULT (WINAPI *PFNETDT)(HWND, DWORD);



// CSearch �Ի���

IMPLEMENT_DYNAMIC(CSearch, CDialogEx)

CSearch::CSearch(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSearch::IDD, pParent)
	, m_simThreshold(0)
{
	m_inputPic = new CPicCtrl(this);
	//m_bigShowPic = new CPicCtrl(this);
}

CSearch::~CSearch()
{
	delete m_inputPic;
	//delete m_bigShowPic;
	
}

void CSearch::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB2, m_tab2);

	//DDX_Control(pDX, IDC_PICINPUT, m_inputPic);
	DDX_Text(pDX, IDC_EDIT_SIMTHRESHOLD, m_simThreshold);
	DDV_MinMaxInt(pDX, m_simThreshold, 0, 100);
	DDX_Control(pDX, IDC_IMGLISTBOX, m_imgListCtrl);
}


BEGIN_MESSAGE_MAP(CSearch, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB2, &CSearch::OnTcnSelchangeTab2)
	ON_BN_CLICKED(IDC_BTN_SEL_IMG, &CSearch::OnBnClickedBtnSelImg)
	ON_BN_CLICKED(IDC_BTN_SEARCH, &CSearch::OnBnClickedBtnSearch)
	ON_STN_CLICKED(IDC_PICINPUT, &CSearch::OnStnClickedPicinput)
	ON_WM_PAINT()
	ON_STN_DBLCLK(IDC_PICINPUT, &CSearch::OnStnDblclickPicinput)
	ON_EN_CHANGE(IDC_EDIT_SIMTHRESHOLD, &CSearch::OnEnChangeEditSimthreshold)
	//ON_CBN_DROPDOWN(IDC_COMBO_INPUTHISTORY, &CSearch::OnCbnDropdownComboInputhistory)
	//ON_CBN_SETFOCUS(IDC_COMBO_INPUTHISTORY, &CSearch::OnCbnSetfocusComboInputhistory)
	ON_LBN_DBLCLK(IDC_IMGLISTBOX, &CSearch::OnLbnDblclkImglistbox)
	ON_BN_CLICKED(IDC_BTN_SEL_IMG_DIR, &CSearch::OnBnClickedBtnSelImgDir)
	ON_LBN_SELCHANGE(IDC_IMGLISTBOX, &CSearch::OnLbnSelchangeImglistbox)
END_MESSAGE_MAP()


// CSearch ��Ϣ�������


BOOL CSearch::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��


	m_inputPic->m_picID=IDC_PICINPUT;
	//m_bigShowPic->m_picID=IDC_PICBIG;
	

	m_tab2.InsertItem(0,_T("Global"));      //��Ӳ���һѡ� 
	m_tab2.InsertItem(1,_T("Local"));    //��Ӳ�����ѡ� 
	m_tab2.SetCurSel(0);
	
	
	m_globalFeatDlg.Create(IDD_GLOBALFEAT,&m_tab2);
	m_localFeatDlg.Create(IDD_LOCALFEAT,&m_tab2);
	
	BOOL l_status = m_resultShowDlg.Create(IDD_RESULTSHOW,this);

	//m_imgListCtrl.SetHorizontalExtent(100);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


BOOL CSearch::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ����ר�ô����/����û���



	return CDialogEx::PreCreateWindow(cs);
}


void CSearch::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: �ڴ˴������Ϣ����������


	//���� tab2�Ĵ�С��λ��
	CRect clientRect;
	// Get size of dialog window.
	GetClientRect(&clientRect);


	CRect rect= clientRect;
	int offset = 5;


	rect.top+=offset;
	rect.bottom *=0.35;
	rect.right *=0.35;

	m_tab2.MoveWindow(&rect, TRUE);  

	m_tab2.SetItemSize(CSize(32,32));

	m_tab2.ShowWindow(true);

	//����tab2�����Ի���Ĵ�С��λ��
	CRect rc;
	m_tab2.GetItemRect(0,rc);
	CRect rc1;
	m_tab2.GetClientRect(&rc1);
	rc1.top+=rc.Height();
	m_globalFeatDlg.MoveWindow(rc1);
	m_localFeatDlg.MoveWindow(rc1);
	m_globalFeatDlg.ChangeSize(SIZE_MAXSHOW,rc1.Width(),rc1.Height());

	m_globalFeatDlg.ShowWindow(true);
	m_localFeatDlg.ShowWindow(false);
	

	//���� search result �ַ�λ��
	CRect rect2=clientRect;
	rect2.left +=clientRect.Width()*0.35;
	rect2.right = clientRect.Width()*0.41;
	rect2.bottom = 30;
	rect2.top += 10;
	
	CStatic *pStatic = (CStatic *)GetDlgItem(IDC_CAPTION);
	pStatic->MoveWindow(&rect2);
	
	CRect rect3=clientRect;
	rect3.left =clientRect.Width()*0.42;
	rect3.right =clientRect.Width()*0.46;
	rect3.bottom = 32;
	rect3.top += 7;

	CEdit * pEdit = (CEdit*)GetDlgItem(IDC_EDIT_SIMTHRESHOLD);
	pEdit->MoveWindow(&rect3);

	CRect rectPer=clientRect;
	rectPer.left =clientRect.Width()*0.46;
	rectPer.right =clientRect.Width()*0.48;
	rectPer.bottom = 32;
	rectPer.top += 11;
	CStatic * pStaticPerSym=(CStatic*)GetDlgItem(IDC_STATIC_PER);
	pStaticPerSym->MoveWindow(&rectPer);


	//���� ��������Ĵ�С��λ��
	CRect rect4=clientRect;
	rect4.left += clientRect.Width() *0.35;		
	rect4.top +=33;
	m_resultShowDlg.MoveWindow(&rect4);
	m_resultShowDlg.ShowWindow(true);


	//  ��ʾ���˼����б�clist
	CRect rect5=clientRect;
	rect5.right *=0.35;
	rect5.top =rect5.bottom - rect5.bottom*0.4;

	//m_bigShowPic->m_pStaticPic=(CStatic*)GetDlgItem(IDC_PICBIG);
	//m_bigShowPic->m_pStaticPic->MoveWindow(rect5);
	m_imgListCtrl.MoveWindow(rect5);

	//���������� �ؼ�λ��
	CRect rect6=clientRect;
	
	rect6.top =  clientRect.bottom*0.36;
	rect6.bottom = clientRect.bottom *0.59;
	rect6.right =rect6.bottom-rect6.top;

	
	m_inputPic->m_pStaticPic= (CStatic *) GetDlgItem(IDC_PICINPUT);
	m_inputPic->m_pStaticPic->MoveWindow(rect6);
	

	//���� ������ťλ��

	CRect rect7;
	rect7.top = clientRect.bottom *0.40;
	rect7.left =clientRect.right*0.17;
	rect7.right=rect7.left+clientRect.right*0.08;
	rect7.bottom= rect7.top+clientRect.bottom*0.05;
	
	
	CComboBox *pCom=(CComboBox*)GetDlgItem(IDC_BTN_SEL_IMG);
	
	pCom->MoveWindow(rect7);

	CRect rect8;
	rect8.top = clientRect.bottom *0.45;
	rect8.left =clientRect.right*0.17;
	rect8.right=rect8.left+clientRect.right*0.08;
	rect8.bottom= rect8.top+clientRect.bottom*0.05;
	CButton * pSelectBtn = (CButton*) GetDlgItem(IDC_BTN_SEL_IMG_DIR);
	pSelectBtn->MoveWindow(rect8);
	

	CRect rect9=rect8;
	rect9.top = clientRect.bottom *0.40;
	rect9.left =clientRect.right*0.25;
	rect9.right=rect9.left+clientRect.right*0.08;
	rect9.bottom= rect9.top+clientRect.bottom*0.1;

	CButton *pSearchBtn = (CButton *) GetDlgItem(IDC_BTN_SEARCH);
	pSearchBtn->MoveWindow(rect9);

}


void CSearch::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialogEx::OnOK();
}


void CSearch::OnTcnSelchangeTab2(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	int num=m_tab2.GetCurSel();
	switch(num)
	{
	case 0:
		m_globalFeatDlg.ShowWindow(SW_SHOW);
		m_localFeatDlg.ShowWindow(SW_HIDE);
		break;
	case 1:
		m_globalFeatDlg.ShowWindow(SW_HIDE);
		m_localFeatDlg.ShowWindow(SW_SHOW);
	}

}


void CSearch::OnBnClickedBtnSelImg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CImgRetrDlg * pMainWnd = (CImgRetrDlg*)AfxGetMainWnd();

	//if (pMainWnd->searchEnine.databaseInfo==NULL)
	//{
	//	MessageBox(_T("����ѡ�����ݿ�!"),_T("����"),MB_ICONERROR);
	//	return;
	//}



	CFileDialog fileDlg(TRUE,NULL,NULL,OFN_EXPLORER|OFN_ENABLESIZING|OFN_HIDEREADONLY |OFN_FILEMUSTEXIST,_T("jpg files(*.jpg;*.jpeg)|*.jpg;*.jpeg|png files (*.png)|*.png|bmp files(*.bmp)|*.bmp|All Files (*.*)|*.*||"),this);
	fileDlg.DoModal();


	CString l_fileName = fileDlg.GetPathName();
	if (l_fileName.GetLength()!=0)
	{
		m_imgFullName = l_fileName;
		m_inputPic->ShowImage(m_imgFullName.GetBuffer());
	}
	

}

UINT ThreadSingleSeach(LPVOID pParam)
{
	extern CImgRetrDlg * g_pMainWnd;

	CSearch * l_searchCtrl = g_pMainWnd->m_searchCtrl;
	CString *l_imgFullname=(CString*)pParam;
	string imgPath = string(l_imgFullname->GetBuffer());

	vector<int> featIndex;
	vector<float> featWeights;
	int distanceType;
	l_searchCtrl->GetSearchParam(featIndex,featWeights,distanceType);
	vector<ImgInfo> &result = g_pMainWnd->searchEnine->m_searchResult;

	bool flag = g_pMainWnd->searchEnine->singleSearch(imgPath,result,featIndex,featWeights,distanceType);

	if(flag)
	{	//��ʾ���
		//l_searchCtrl->UpdateData();
		l_searchCtrl->m_resultShowDlg.SetImageList(&g_pMainWnd->searchEnine->m_searchResult,l_searchCtrl->m_simThreshold);
		l_searchCtrl->m_resultShowDlg.ShowImageList();
	}

	return 0;
}


void CSearch::OnBnClickedBtnSearch()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	extern CImgRetrDlg * g_pMainWnd;
	if (g_pMainWnd->searchEnine->databaseInfo==NULL)
	{
		MessageBox(_T("����ѡ�����ݿ�!"),_T("����"),MB_ICONERROR);
		return;
	}

	if (m_imgFullName.GetLength()==0)
	{
		MessageBox(_T("��ѡ�����ͼ��!"),_T("����"),MB_ICONERROR);
		return ;
	}
	
	if (g_pMainWnd->searchEnine->databaseInfo->imgFullNames.size()==0)
	{
		MessageBox(_T("���ݿ���û��ͼ�����������ݿ�!"),_T("����"),MB_ICONERROR);
		return ;
	}



	//string imgPath = string(m_imgFullName.GetBuffer());
	//vector<int> featIndex;
	//vector<float> featWeights;
	//int distanceType;
	//GetSearchParam(featIndex,featWeights,distanceType);
	//vector<ImgInfo> &result = g_pMainWnd->searchEnine->m_searchResult;
	//
	//bool flag = g_pMainWnd->searchEnine->singleSearch(imgPath,result,featIndex,featWeights,distanceType);


	AfxBeginThread(ThreadSingleSeach,&m_imgFullName,0,0,0);

}


void CSearch::GetSearchParam(vector<int> &featIndex,vector<float> & featWeights,int &distanceType)
{
	//CGlobalFeat * pGlobalFeatWnd = (CGlobalFeat *) m_tab2.GetDlgItem(IDD_GLOBALFEAT);

	CComboBox * colorComboBox =(CComboBox*)m_globalFeatDlg.GetDlgItem(IDC_COMBO_COLORFEAT);
	CComboBox * textureComboBox =(CComboBox*)m_globalFeatDlg.GetDlgItem(IDC_COMBO_TEXTUREFEAT);
	CComboBox * shapeComboBox =(CComboBox*)m_globalFeatDlg.GetDlgItem(IDC_COMBO_SHAPEFEAT);
	
	
	featIndex.push_back(colorComboBox->GetCurSel());
	featIndex.push_back(textureComboBox->GetCurSel()+colorComboBox->GetCount());
	featIndex.push_back(shapeComboBox->GetCurSel() + colorComboBox->GetCount()+textureComboBox->GetCount());

	//���̻߳��� ���ڲ���ֻ�������߳��н��У��������߳���Ӧ�޸ĵĺ������������
	//m_globalFeatDlg.UpdateData(TRUE);

	int sum = m_globalFeatDlg.m_colorWeight+m_globalFeatDlg.m_textureWeight+m_globalFeatDlg.m_shapeWeight;
	
	if (sum ==0)
	{
		featWeights.push_back(1.0/3);
		featWeights.push_back(1.0/3);
		featWeights.push_back(1.0/3);
	}
	else
	{	
		featWeights.push_back((float)m_globalFeatDlg.m_colorWeight/sum);
		featWeights.push_back((float)m_globalFeatDlg.m_textureWeight/sum);
		featWeights.push_back((float)m_globalFeatDlg.m_shapeWeight/sum);
	}

	CComboBox * distComBox = (CComboBox*)m_globalFeatDlg.GetDlgItem(IDC_COMBO_SIM);
	distanceType=distComBox->GetCurSel();
}



void CSearch::OnStnDblclickPicinput()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//m_bigShowPic->ShowImage(m_inputPic->m_imgToShow);

	//m_inputPic->ShowImage(string(m_imgFullName));

	string imgName = string(m_imgFullName);
	Mat  img = imread(imgName);
	if (img.empty())
	{
		return;
	}
	cvNamedWindow(imgName.c_str(),CV_WINDOW_KEEPRATIO);
	cv::imshow(imgName,img);
}


void CSearch::OnStnClickedPicinput()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	extern CImgRetrDlg * g_pMainWnd;
	//m_bigShowPic->ShowImage(m_inputPic->m_imgToShow);
	g_pMainWnd->m_statusBar.SetWindowText(m_imgFullName.GetBuffer());

}


void CSearch::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()

	m_inputPic->OnPaint();
	//m_bigShowPic->OnPaint();
}




void CSearch::OnEnChangeEditSimthreshold()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	if(!UpdateData()) 
		return ;

	extern CImgRetrDlg* g_pMainWnd;
	if (g_pMainWnd->searchEnine->m_searchResult.size() !=0)
	{
		m_resultShowDlg.SetImageList(&g_pMainWnd->searchEnine->m_searchResult,m_simThreshold);
		m_resultShowDlg.ShowImageList();
	}
	

}


void CSearch::OnBnClickedBtnSelImgDir()
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
			MessageBox(_T("����·��Ϊ�գ�������ͼ��!"),_T("����"),MB_ICONERROR);
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


	m_imgListPath=CString(szPath);

	m_imgNameList.clear();
	m_imgListCtrl.ResetContent();
	FindFiles::findFilesInDirAndSubdirs(m_imgNameList,m_imgListPath.GetBuffer());


	m_imgListCtrl.InitStorage(m_imgNameList.size(),MAX_PATH);
	int l_maxLenPixel=0;
	CDC *pDC=m_imgListCtrl.GetDC();
	for (int i = 0 ; i !=m_imgNameList.size();++i)
	{
		int len=pDC->GetTextExtent(CString(m_imgNameList[i].c_str())).cx ;
		if (len> l_maxLenPixel)
		{
			l_maxLenPixel=len;
		}
		
		m_imgListCtrl.AddString(m_imgNameList[i].c_str());
	}
	ReleaseDC(pDC);
	m_imgListCtrl.SetItemHeight(0,20);
	
	m_imgListCtrl.SetHorizontalExtent(l_maxLenPixel);
}


void CSearch::OnLbnDblclkImglistbox()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������


	int index=m_imgListCtrl.GetCurSel();
	m_imgFullName=CString(m_imgNameList[index].c_str());
	OnBnClickedBtnSearch();
}





void CSearch::OnLbnSelchangeImglistbox()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	int index = m_imgListCtrl.GetCurSel();
	m_inputPic->ShowImage(m_imgNameList[index]);
	m_imgFullName=CString(m_imgNameList[index].c_str());
}
