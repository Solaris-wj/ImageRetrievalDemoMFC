#pragma once

#include "resource.h"

// CLocalFeat �Ի���

class CLocalFeat : public CDialogEx
{
	DECLARE_DYNAMIC(CLocalFeat)

public:
	CLocalFeat(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLocalFeat();

// �Ի�������
	enum { IDD = IDD_LOCALFEAT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
};
