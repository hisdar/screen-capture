#include "stdafx.h"
#include "tool/select/SCSelectToolView.h"
#include "base/base-def.h"
#include "tool/SCMaskTool.h"

SCSelectToolView::SCSelectToolView(CWnd *wnd)
	: m_selectedRect(0, 0, 0, 0)
	, m_wnd(wnd)
{
}

SCSelectToolView::~SCSelectToolView()
{
}

void SCSelectToolView::SetRect(const CRect &rect)
{
	m_rect = rect;
}

void SCSelectToolView::GetRect(CRect &rect)
{
	rect = m_rect;
}

void SCSelectToolView::SetSelectedRect(const CRect &rect)
{
	if (rect.left < m_rect.left
		|| rect.right > m_rect.right
		|| rect.top < m_rect.top
		|| rect.bottom > m_rect.bottom)
	{
		return;
	}

	m_selectedRect = rect;

	m_wnd->Invalidate();
}

void SCSelectToolView::GetSelectedRect(CRect &rect)
{
	rect = m_selectedRect;
}

BOOL SCSelectToolView::IsPointInSelectedRect(CPoint &point)
{
	if (m_selectedRect.IsRectEmpty()) {
		return FALSE;
	}

	return m_selectedRect.PtInRect(point);
}

int  SCSelectToolView::IsPointInResizeButton(CPoint &point)
{
	// 没有选择区的时候，不应该有调整框
	if (m_selectedRect.IsRectEmpty()) {
		return 0;
	}

	ResizeButtonRects rbRects;
	GetResizeButtonRects(rbRects);

	for (unsigned i = 0; i < rbRects.GetSize(); i++) {
		if (rbRects[i + 1].PtInRect(point)) {
			return i + 1;
		}
	}

	return 0;
}

BOOL SCSelectToolView::SetCursorIcon(LONG cursorIcon)
{
	int ret = 0;

	// not change
	if (GetClassLong(m_wnd->GetSafeHwnd(), GCL_HCURSOR) == cursorIcon) {
		return TRUE;
	}

	ret = SetClassLong(m_wnd->GetSafeHwnd(), GCL_HCURSOR, cursorIcon);
	if (ret) {
		return TRUE;
	}

	SCErr("SetCursorIcon fail\n");
	return FALSE;
}

BOOL SCSelectToolView::Draw(CDC &cdc)
{
	int ret = FALSE;

	m_wnd->GetWindowRect(m_rect);

	ret = DrawMask(cdc);
	if (!ret) {
		SCErr("DrawMette fail\n");
		return ret;
	}

	if (!m_selectedRect.IsRectEmpty()) {
		return DrawSelectedArea(&cdc);
	}

	return TRUE;
}

BOOL SCSelectToolView::DrawMask(CDC &cdc)
{
	BOOL bRet = FALSE;
	CRect cRect = m_selectedRect;
	
	// get dc size
	int dcWidth = cdc.GetDeviceCaps(HORZRES);
	int dcHeight = cdc.GetDeviceCaps(VERTRES);

	SCMaskTool mt;

	// draw top mette
	mt.SetRect(0, 0, dcWidth, cRect.top);
	bRet = mt.Draw(cdc);
	if (!bRet) {
		SCErr("draw top mette fail\n");
		return bRet;
	}

	// draw right mette
	mt.SetRect(cRect.right, cRect.top, dcWidth, cRect.bottom);
	bRet = mt.Draw(cdc);
	if (!bRet) {
		SCErr("draw right mette fail\n");
		return bRet;
	}

	// draw bottom mette
	mt.SetRect(0, cRect.bottom, dcWidth, dcHeight);
	bRet = mt.Draw(cdc);
	if (!bRet) {
		SCErr("draw bottom mette fail\n");
		return bRet;
	}

	// draw left mette
	mt.SetRect(0, cRect.top, cRect.left, cRect.bottom);
	bRet = mt.Draw(cdc);

	return bRet;
}

BOOL SCSelectToolView::DrawSelectedArea(CDC * pCDC)
{
	BOOL bRet = TRUE;

	// get image dc size
	int width = pCDC->GetDeviceCaps(HORZRES);
	int height = pCDC->GetDeviceCaps(VERTRES);

	// set pen
	CPen boarderPen;
	COLORREF boarderColor = RGB(100, 100, 255);
	boarderPen.CreatePen(PS_SOLID, 1, boarderColor);
	CPen *oldPen = pCDC->SelectObject(&boarderPen);

	// draw boarder
	CPoint m_startLocation(m_selectedRect.left, m_selectedRect.top);
	CPoint m_endLocation(m_selectedRect.right, m_selectedRect.bottom);
	pCDC->MoveTo(m_startLocation);
	pCDC->LineTo(m_endLocation.x, m_startLocation.y);

	pCDC->LineTo(m_endLocation);
	pCDC->LineTo(m_startLocation.x, m_endLocation.y);
	bRet = pCDC->LineTo(m_startLocation);

	// draw adjust tool
	ResizeButtonRects rbRects;
	GetResizeButtonRects(rbRects);

	LOGBRUSH brush;
	brush.lbColor = boarderColor;
	brush.lbStyle = BS_SOLID;
	brush.lbHatch = HS_CROSS;
	CBrush boarderBrush;
	bRet = boarderBrush.CreateBrushIndirect(&brush);
	for (int i = 0; i < rbRects.GetSize(); i++) {
		pCDC->FillRect(rbRects[i + 1], &boarderBrush);
	}

	// 
	pCDC->SelectObject(oldPen);
	boarderPen.DeleteObject();

	return TRUE;
}

void SCSelectToolView::GetResizeButtonRects(ResizeButtonRects &resizeButtonRects)
{
	int offset = 2;
	CRect rect(0, 0, 0, 0);
	CRect cRect = m_selectedRect;

	rect.SetRect(cRect.left - offset, cRect.top - offset, cRect.left + offset, cRect.top + offset);
	resizeButtonRects[RESIZE_BUTTON_LEFT_TOP] = rect;

	rect.SetRect(cRect.right - offset, cRect.top - offset, cRect.right + offset, cRect.top + offset);
	resizeButtonRects[RESIZE_BUTTON_RIGHT_TOP] = rect;

	rect.SetRect(cRect.right - offset, cRect.bottom - offset, cRect.right + offset, cRect.bottom + offset);
	resizeButtonRects[RESIZE_BUTTON_RIGHT_BOTTOM] = rect;

	rect.SetRect(cRect.left - offset, cRect.bottom - offset, cRect.left + offset, cRect.bottom + offset);
	resizeButtonRects[RESIZE_BUTTON_LEFT_BOTTON] = rect;

	int xM = (cRect.right + cRect.left) / 2;
	int yM = (cRect.bottom + cRect.top) / 2;
	rect.SetRect(xM - offset, cRect.top - offset, xM + offset, cRect.top + offset);
	resizeButtonRects[RESIZE_BUTTON_TOP] = rect;

	rect.SetRect(cRect.right - offset, yM - offset, cRect.right + offset, yM + offset);
	resizeButtonRects[RESIZE_BUTTON_RIGHT] = rect;

	rect.SetRect(xM - offset, cRect.bottom - offset, xM + offset, cRect.bottom + offset);
	resizeButtonRects[RESIZE_BUTTON_BOTTOM] = rect;

	rect.SetRect(cRect.left - offset, yM - offset, cRect.left + offset, yM + offset);
	resizeButtonRects[RESIZE_BUTTON_LEFT] = rect;
}
