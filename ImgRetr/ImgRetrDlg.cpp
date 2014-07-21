
// ImgRetrDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImgRetr.h"
#include "ImgRetrDlg.h"
#include "afxdialogex.h"

#include "Search.h"
#include "DatabaseCtrl.h"

#include "../utilities/searchEngine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{

}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CImgRetrDlg �Ի���




CImgRetrDlg *g_pMainWnd;

CImgRetrDlg::CImgRetrDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImgRetrDlg::IDD, pParent)
{
	g_pMainWnd=this;

	//m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hIcon = AfxGetApp()->LoadIcon(IDI_MAIN);
	m_searchCtrl=new CSearch(&m_tab1);
	m_databaseCtrl = new CDatabaseCtrl(&m_tab1);
	searchEnine = new SearchEngine;
	
}

CImgRetrDlg::~CImgRetrDlg()
{
	delete m_searchCtrl;
	delete m_databaseCtrl;
	delete searchEnine;
}

void CImgRetrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab1);
}

BEGIN_MESSAGE_MAP(CImgRetrDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CImgRetrDlg::OnTcnSelchangeTab1)
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CImgRetrDlg ��Ϣ�������

BOOL CImgRetrDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	
	// TODO: �ڴ���Ӷ���ĳ�ʼ������


	MoveWindow(0,0,1080,768);


	//״̬�����
	m_statusBar.Create(WS_CHILD|WS_VISIBLE|SBT_OWNERDRAW,CRect(0,0,0,0),this,0);





	
	//TAB�ؼ����	
	

	m_tab1.InsertItem(0,_T("����ͼ��"),0);      //��Ӳ���һѡ� 
	m_tab1.InsertItem(1,_T("�������ݿ�"),1);    //��Ӳ�����ѡ� 
	m_tab1.SetCurSel(0);

	//��������ʼ����ǩ
	m_searchCtrl->Create(IDD_SEARCH,&m_tab1);
	m_databaseCtrl->Create(IDD_DATABASECTRL,&m_tab1);



	

	
	//CImageList* imgList = new CImageList;
	//imgList->Create(32,32,ILC_COLOR4,2,2);

	//imgList->SetBkColor(RGB(255,255,255));
//	imgList->Add(::AfxGetApp()->LoadIcon(IDI_ICON1));
	//imgList->Add(::AfxGetApp()->LoadIcon(IDI_ICON1));

	//m_tab1.SetImageList(imgList);

	//delete imgList;



	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CImgRetrDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CImgRetrDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CImgRetrDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CImgRetrDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int num=m_tab1.GetCurSel();
	switch(num)
	{
	case 0:
		m_searchCtrl->ShowWindow(SW_SHOW);
		m_databaseCtrl->ShowWindow(SW_HIDE);
		break;
	case 1:
		m_searchCtrl->ShowWindow(SW_HIDE);
		m_databaseCtrl->ShowWindow(SW_SHOW);
	}

	*pResult = 0;
}


void CImgRetrDlg::OnSize(UINT nType, int cx, int cy)
{

	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������

}


void CImgRetrDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialogEx::OnOK();
}


void CImgRetrDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: �ڴ˴������Ϣ����������

	//״̬�����
	//int strPartDim[3]={100,200,-1};

	//m_statusBar.SetParts(3,strPartDim);

	//m_statusBar.SetText(_T("����������������������������"),0,0);
	//m_statusBar.SetText(_T("������"),1,0);
	//m_statusBar.SetText(_T("������"),2,0);


	CRect statucRect;
	m_statusBar.GetWindowRect(&statucRect);

	//���� tab1�Ĵ�С��λ��
	CRect rect;
	// Get size of dialog window.
	GetClientRect(&rect);

	//m_tab1.AdjustRect(FALSE, &rect);
	m_tab1.SetItemSize(CSize(32,32));

	int offset =10;

	rect.top+=offset;
	rect.bottom -=statucRect.Height();
	m_tab1.MoveWindow(&rect, TRUE);  	

	m_tab1.ShowWindow(true);

	//�������� �Ի���Ĵ�С��λ��
	CRect rc;
	m_tab1.GetItemRect(0,&rc);

	CRect rc2;
	m_tab1.GetClientRect(rc2);
	rc2.top +=rc.Height();

	m_searchCtrl->MoveWindow(&rc2);
	m_databaseCtrl->MoveWindow(&rc2);

	m_databaseCtrl->ChangeSize(SIZE_MAXSHOW,rc2.Width(),rc2.Height());

	m_searchCtrl->ShowWindow(true);
	m_databaseCtrl->ShowWindow(false);

}


void CImgRetrDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	extern CMutex g_mutex;
	CSingleLock singleLock(&g_mutex);
	singleLock.Lock(1);
	if (!singleLock.IsLocked())
	{
		int flag =MessageBox(_T("��һ���������ڽ��У����ڹرճ������������𻵣�\n ������"),_T("����"),MB_OKCANCEL);
		if (flag ==IDOK)
		{
			int flag2=MessageBox(_T("�رճ������Ҫ��ԭ����ɾ����\n ������"),_T("����"),MB_OKCANCEL);
			if (flag2==IDOK)
			{
				CDialogEx::OnClose();
			}
		}
		
		
	}

	CDialogEx::OnClose();
	
}

