// LocalFeat.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImgRetr.h"
#include "LocalFeat.h"
#include "afxdialogex.h"


// CLocalFeat �Ի���

IMPLEMENT_DYNAMIC(CLocalFeat, CDialogEx)

CLocalFeat::CLocalFeat(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLocalFeat::IDD, pParent)
{

}

CLocalFeat::~CLocalFeat()
{
}

void CLocalFeat::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLocalFeat, CDialogEx)
END_MESSAGE_MAP()


// CLocalFeat ��Ϣ�������


void CLocalFeat::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialogEx::OnOK();
}
