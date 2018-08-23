// HStatic.cpp : 实现文件
//

#include "stdafx.h"
#include "base/SCStatic.h"


// HStatic

IMPLEMENT_DYNAMIC(SCStatic, CStatic)

SCStatic::SCStatic():
	m_isMouseIn(FALSE)
{

}

SCStatic::~SCStatic()
{
}


BEGIN_MESSAGE_MAP(SCStatic, CStatic)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_DRAWITEM()
	ON_WM_MOUSEHOVER()
END_MESSAGE_MAP()



// HStatic 消息处理程序




void SCStatic::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CStatic::OnPaint()

	CRect rect;
	GetWindowRect(&rect);

	// draw border
	CPen borderPen(PS_SOLID, 1, RGB(100, 100, 100));
	dc.SelectObject(&borderPen);

	dc.MoveTo(0, 0);
	dc.LineTo(0, rect.Height());
	dc.LineTo(rect.Width(), rect.Height());
	dc.LineTo(rect.Width(), 0);
	dc.LineTo(0, 0);

	// draw bg
	LOGBRUSH logBrush;
	logBrush.lbColor = m_backgroundColor;
	logBrush.lbStyle = BS_SOLID;
	logBrush.lbHatch = HS_CROSS;

	CBrush cBrush;
	cBrush.CreateBrushIndirect(&logBrush);
	dc.SelectObject(&cBrush);

	CRect colorRect;
	colorRect.left = 2;
	colorRect.top = 2;
	colorRect.right = rect.Width();
	colorRect.bottom = rect.Height();
	dc.Rectangle(&colorRect);

	// draw selected borader
	if (m_isMouseIn) {
		CPen pen(PS_SOLID, 1, RGB(255, 255, 255));
		dc.SelectObject(&pen);

		dc.MoveTo(1, 1);
		dc.LineTo(1, rect.Height() - 1);
		dc.LineTo(rect.Width() - 1, rect.Height() - 1);
		dc.LineTo(rect.Width() - 1, 1);
		dc.LineTo(1, 1);
	}
}

void SCStatic::SetBgColor(COLORREF color)
{
	m_backgroundColor = color;
	Invalidate(TRUE);
}


COLORREF SCStatic::GetBgColor()
{
	return m_backgroundColor;
}

void SCStatic::SetIsMouseIn(BOOL isMouseIn)
{
	m_isMouseIn = isMouseIn;
	Invalidate(TRUE);
}


void SCStatic::OnMouseMove(UINT nFlags, CPoint point)
{
	
// 	if (!m_isMouseIn) {
// 		m_isMouseIn = TRUE;
// 		Invalidate(TRUE);
// 		UpdateWindow();
// 	}

	//CStatic::OnMouseMove(nFlags, point);

	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);
	tme.hwndTrack = m_hWnd;
	tme.dwFlags = TME_LEAVE | TME_HOVER;
	tme.dwHoverTime = 1;
	_TrackMouseEvent(&tme);
}


void SCStatic::OnMouseLeave()
{
	m_isMouseIn = FALSE;
	//MessageBox(_T("leag"), _T(""), MB_OK);
	Invalidate(TRUE);
	UpdateWindow();
	CStatic::OnMouseLeave();
}


void SCStatic::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//OnPaint();
	CStatic::OnDrawItem(nIDCtl, lpDrawItemStruct);
	//OnPaint();
}


void SCStatic::DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/)
{
	//OnPaint();
	// TODO:  添加您的代码以绘制指定项

	//LPDRAWITEMSTRUCT->hDC
}


void SCStatic::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_isMouseIn = TRUE;
	//MessageBox(_T("leag"), _T(""), MB_OK);
	Invalidate(TRUE);
	UpdateWindow();
	CStatic::OnMouseHover(nFlags, point);
}
