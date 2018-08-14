XAccessible
// HButton.cpp : 实现文件
//

#include "stdafx.h"
#include "ScreenCapture.h"
#include "HButton.h"


// HButton

IMPLEMENT_DYNAMIC(HButton, CWnd)

HButton::HButton() :
m_isMouseIn(FALSE),
	m_isClicked(FALSE)
{

}

HButton::~HButton()
{
}


BEGIN_MESSAGE_MAP(HButton, CWnd)
	ON_WM_MOUSELEAVE()
//	ON_WM_MOUSEHOVER()
//	ON_WM_MOUSEWHEEL()
//	ON_WM_MOUSEACTIVATE()
//	ON_WM_MOUSEHWHEEL()
	ON_WM_MOUSEMOVE()
//	ON_WM_NCMOUSEHOVER()
//	ON_WM_NCMOUSELEAVE()
//	ON_WM_NCMOUSEMOVE()
END_MESSAGE_MAP()



// HButton 消息处理程序
void HButton::OnMouseLeave()
{
	m_isMouseIn = FALSE;

	HSetBtnBitMap();

	CButton::OnMouseLeave();
}

void HButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_isMouseIn) {
		m_isMouseIn = TRUE;
		HSetBtnBitMap();
	}

	CButton::OnMouseMove(nFlags, point);
}

BOOL HButton::HLoadBitMap(UINT deffaultBmpId, UINT mouseInBmpId, UINT clickedBmpId)
{
	m_bmpMouseIn.LoadBitmapW(mouseInBmpId);
	m_bmpDefault.LoadBitmapW(deffaultBmpId);
	m_bmpClicked.LoadBitmapW(clickedBmpId);
	
	return HSetBtnBitMap();
}

BOOL HButton::HSetBtnClicked(BOOL isClicked)
{
	m_isClicked = isClicked;

	return HSetBtnBitMap();
}

BOOL HButton::HSetBtnBitMap()
{
	SetBitmap((HBITMAP)m_bmpDefault.GetSafeHandle());

	if (m_isClicked) {
		SetBitmap((HBITMAP)m_bmpClicked.GetSafeHandle());
		return TRUE;
	}

	if (m_isMouseIn) {
		SetBitmap((HBITMAP)m_bmpMouseIn.GetSafeHandle());
		return TRUE;
	}

	SetBitmap((HBITMAP)m_bmpDefault.GetSafeHandle());
	return TRUE;
}