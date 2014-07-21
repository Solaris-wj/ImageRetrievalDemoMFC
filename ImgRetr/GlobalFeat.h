#pragma once
#include <vector>

#include "resource.h"

#include "../utilities/DatabaseInfo.h"
using namespace std;
// CGlobalFeat �Ի���

class CGlobalFeat : public CDialogEx
{
	DECLARE_DYNAMIC(CGlobalFeat)

public:
	CGlobalFeat(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGlobalFeat();

// �Ի�������
	enum { IDD = IDD_GLOBALFEAT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit3();
	DECLARE_MESSAGE_MAP()
	virtual void OnOK();

public:
	virtual BOOL OnInitDialog();
	void ChangeSize(UINT nType, int cx, int cy);

public:
	CRect m_rect;
	vector<int> m_vecCtrlID;
	vector<CRect> m_vecCtrlRect;

	int m_textureWeight;
	int m_shapeWeight;
	int m_colorWeight;
	afx_msg void OnEnChangeEdit1();
	
};
