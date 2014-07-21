// MyTabCrtl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImgRetr.h"
#include "MyTabCrtl.h"


// CTabCrtl

IMPLEMENT_DYNAMIC(CMyTabCtrl, CTabCtrl)

CMyTabCtrl::CMyTabCtrl()
{

}

CMyTabCtrl::~CMyTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyTabCtrl, CTabCtrl)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CTabCrtl ��Ϣ�������




void CMyTabCtrl::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CMyTabCrtl::OnPaint()


	CDC* pDC = fatherDialog->GetDC();
	COLORREF color= pDC->GetBkColor();
	
ReleaseDC(pDC);

	CRect		rectClient, rectItem;
	CBitmap		bitmapTemp, *pOldBitmap;
	CDC*		pMemDC=new CDC;
	CPaintDC	dc(this);

	//��ȡ���ڴ�С
	GetClientRect(rectClient);

	//����λͼ�ڴ�
	bitmapTemp.CreateCompatibleBitmap(&dc, rectClient.Width(), rectClient.Height());
	pMemDC->CreateCompatibleDC(&dc);
	pOldBitmap=pMemDC->SelectObject(&bitmapTemp);

	pMemDC->FillSolidRect(rectClient, color);


	for(int i=0; i< this->GetItemCount(); i++)
	{
		GetItemRect(i, rectItem);
		pMemDC->FillSolidRect(rectItem, color);
		pMemDC->FrameRect(rectItem, &CBrush(color));
		//DrawItemName(pMemDC, i);
	}

	dc.BitBlt(0, 0, rectClient.Width(), rectClient.Height(), pMemDC, 0, 0, SRCCOPY);

	//�ͷ��ڴ�
	pMemDC->SelectObject(pOldBitmap);
	ReleaseDC(pMemDC);

	delete	pMemDC;
}
