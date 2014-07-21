
// ImgRetrDlg.h : ͷ�ļ�
//


#ifndef IMGRETRDLG_HHH
#define IMGRETRDLG_HHH
//#pragma once
#include "afxcmn.h"

#include "resource.h"

//#include "Search.h"
//#include "DatabaseCtrl.h"





class CSearch;
class CDatabaseCtrl;
class SearchEngine;

// CImgRetrDlg �Ի���
class CImgRetrDlg : public CDialogEx
{
// ����
public:
	CImgRetrDlg(CWnd* pParent = NULL);	// ��׼���캯��
	CImgRetrDlg::~CImgRetrDlg();

// �Ի�������
	enum { IDD = IDD_IMGRETR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	virtual void OnOK();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

public:
	CTabCtrl m_tab1;
	CStatusBarCtrl m_statusBar;

	CSearch *m_searchCtrl;
	CDatabaseCtrl *m_databaseCtrl;

	SearchEngine *searchEnine;
	afx_msg void OnClose();
	
};

#endif