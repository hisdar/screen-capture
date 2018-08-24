#include "stdafx.h"
#include "base/base-def.h"
#include "tool/SCMaskTool.h"
#include "tool/SCSelectTool.h"

#define MOVE_LEFT_TO(rect, x) \
	if ((x) > (rect).right) {\
		(rect).left = (rect).right; \
		(rect).right = (x); \
	}else {	\
		(rect).left = (x); \
	}

#define MOVE_RIGHT_TO(rect, x) \
	if ((rect).left > (x)) {\
		(rect).right = (rect).left; \
		(rect).left = (x); \
	} else {\
		(rect).right = (x); \
	}

#define MOVE_TOP_TO(rect, y) \
	if ((rect).bottom < (y)) {\
		(rect).top = (rect).bottom; \
		(rect).bottom = (y); \
	} else {\
		(rect).top = (y); \
	}

#define MOVE_BOTTOM_TO(rect, y)\
	if ((rect).top > (y)) {\
		(rect).bottom = (rect).top; \
		(rect).top = (y); \
	} else {\
		(rect).bottom = (y); \
	}


SCSelectTool::SCSelectTool()
: m_selectedRect(0, 0, 0, 0)
, m_state(SC_SELECT_STATE_NONE)
{
}

SCSelectTool::~SCSelectTool()
{
}

int SCSelectTool::GetToolType()
{
	return SC_TOOL_TYPE_SELECT;
}

void SCSelectTool::SetRect(const CRect &rect)
{
	m_rect = rect;
}

void SCSelectTool::GetRect(CRect &rect)
{
	rect = m_rect;
}

void SCSelectTool::SetSelectedRect(const CRect &rect)
{
	if (rect.left < m_rect.left
		|| rect.right > m_rect.right
		|| rect.top < m_rect.top
		|| rect.bottom > m_rect.bottom)
	{
		SCErr("rect out of range, range is : [%d,%d,%d,%d], param is : [%d,%d,%d,%d]\n",
			m_rect.left, m_rect.top, m_rect.right, m_rect.bottom,
			rect.left, rect.top, rect.right, rect.bottom);
		return;
	}

	m_selectedRect = rect;
	NotifySelectArea(rect);
}

void SCSelectTool::GetSelectedRect(CRect &rect)
{
	rect = m_selectedRect;
}

BOOL SCSelectTool::IsPointInSelectedRect(CPoint &point)
{
	if (m_selectedRect.IsRectEmpty()) {
		return FALSE;
	}

	return m_selectedRect.PtInRect(point);
}

int  SCSelectTool::IsPointInResizeButton(CPoint &point)
{
	// 没有选择区的时候，不应该有调整框
	if (m_selectedRect.IsRectEmpty()) {
		return 0;
	}

	ResizeButtonRects rbRects;
	GetResizeButtonRects(rbRects);

	for (int i = 0; i < rbRects.GetSize(); i++) {
		if (rbRects[i + 1].PtInRect(point)) {
			return i + 1;
		}
	}

	return 0;
}
//
//BOOL SCSelectTool::SetCursorIcon(LONG cursorIcon)
//{
//	int ret = 0;
//
//	// not change
//	if (GetClassLong(m_wnd->GetSafeHwnd(), GCL_HCURSOR) == cursorIcon) {
//		return TRUE;
//	}
//
//	ret = SetClassLong(m_wnd->GetSafeHwnd(), GCL_HCURSOR, cursorIcon);
//	if (ret) {
//		return TRUE;
//	}
//
//	SCErr("SetCursorIcon fail\n");
//	return FALSE;
//}

BOOL SCSelectTool::Draw(CDC &cdc)
{
	int ret = FALSE;

	int cx = cdc.GetDeviceCaps(HORZRES);
	int cy = cdc.GetDeviceCaps(VERTRES);

	m_rect.left = 0;
	m_rect.top = 0;
	m_rect.right = cx;
	m_rect.bottom = cy;

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

BOOL SCSelectTool::DrawMask(CDC &cdc)
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

BOOL SCSelectTool::DrawSelectedArea(CDC * pCDC)
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

void SCSelectTool::GetResizeButtonRects(ResizeButtonRects &resizeButtonRects)
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

BOOL SCSelectTool::AddListener(SCSelectToolEventListener *listener)
{
	for (int i = 0; i < m_selectToolEventListenerArray.GetSize(); i++) {
		if (listener == m_selectToolEventListenerArray.GetAt(i)) {
			SCErr("listener is exist\n");
			return FALSE;
		}
	}

	m_selectToolEventListenerArray.Add(listener);
	return TRUE;
}

void SCSelectTool::RemoveListener(SCSelectToolEventListener *listener)
{
	for (int i = 0; i < m_selectToolEventListenerArray.GetSize(); i++) {
		if (listener == m_selectToolEventListenerArray.GetAt(i)) {
			m_selectToolEventListenerArray.RemoveAt(i);
			return ;
		}
	}
}

void SCSelectTool::NotifyState(SCSelectState state)
{
	for (int i = 0; i < m_selectToolEventListenerArray.GetSize(); i++) {
		SCSelectToolEventListener *listener = m_selectToolEventListenerArray.GetAt(i);
		listener->SelectStateChangeEvent(state);
	}
}

void SCSelectTool::NotifySelectArea(const CRect &selectArea)
{
	for (int i = 0; i < m_selectToolEventListenerArray.GetSize(); i++) {
		SCSelectToolEventListener *listener = m_selectToolEventListenerArray.GetAt(i);
		listener->SelectAreaChangeEvent(selectArea);
	}
}

////////////////// 消息处理函数 /////////////////////
int SCSelectTool::GetState()
{
	return m_state;
}

void SCSelectTool::UpdateCursorIcon(CPoint &point)
{
	int ret = FALSE;
	LONG cursorIcon = 0;

	cursorIcon = (LONG)LoadCursor(NULL, IDC_ARROW);

	int resizeLocation = IsPointInResizeButton(point);
	if (resizeLocation > 0) {

		ret = GetResizeStateCursorIcon(resizeLocation, &cursorIcon);
		if (!ret) {
			SCErr("GetResizeStateCursorIcon fail, resizeLocation=%d\n", resizeLocation);
		}

	}
	else if (IsPointInSelectedRect(point)) {
		cursorIcon = (LONG)LoadCursor(NULL, IDC_SIZEALL);
	}

	//SetCursorIcon(cursorIcon);
}

BOOL SCSelectTool::GetResizeStateCursorIcon(int resizeLocation, LONG *cursorIcon)
{
	switch (resizeLocation)
	{
	case RESIZE_BUTTON_LEFT:
	case RESIZE_BUTTON_RIGHT:
		*cursorIcon = (LONG)LoadCursor(NULL, IDC_SIZEWE);
		return TRUE;

	case RESIZE_BUTTON_TOP:
	case RESIZE_BUTTON_BOTTOM:
		*cursorIcon = (LONG)LoadCursor(NULL, IDC_SIZENS);
		return TRUE;

	case RESIZE_BUTTON_RIGHT_TOP:
	case RESIZE_BUTTON_LEFT_BOTTON:
		*cursorIcon = (LONG)LoadCursor(NULL, IDC_SIZENESW);
		return TRUE;

	case RESIZE_BUTTON_LEFT_TOP:
	case RESIZE_BUTTON_RIGHT_BOTTOM:
		*cursorIcon = (LONG)LoadCursor(NULL, IDC_SIZENWSE);
		return TRUE;

	default:
		return FALSE;
	}
}

void SCSelectTool::MoveSelectedRect(CPoint &point)
{
	CRect rect;
	CRect viewRect;

	GetRect(viewRect);
	GetSelectedRect(rect);
	CPoint movePoint(point);

	if (rect.left + movePoint.x < viewRect.left) {
		movePoint.x = viewRect.left - rect.left;
	}

	if (rect.right + movePoint.x > viewRect.right) {
		movePoint.x = viewRect.right - rect.right;
	}

	if (rect.top + movePoint.y < viewRect.top) {
		movePoint.y = viewRect.top - rect.top;
	}

	if (rect.bottom + movePoint.y > viewRect.bottom) {
		movePoint.y = viewRect.bottom - rect.bottom;
	}

	rect.OffsetRect(movePoint);
	SetSelectedRect(rect);
}

void SCSelectTool::ResizeSelectedRect(int location, CPoint &point)
{
	CRect rect;
	rect = m_resizeBeginRect;

	switch (location)
	{
	case RESIZE_BUTTON_LEFT:

		MOVE_LEFT_TO(rect, point.x);
		SetSelectedRect(rect);
		break;

	case RESIZE_BUTTON_RIGHT:

		MOVE_RIGHT_TO(rect, point.x)
			SetSelectedRect(rect);
		break;

	case RESIZE_BUTTON_TOP:

		MOVE_TOP_TO(rect, point.y);
		SetSelectedRect(rect);
		break;

	case RESIZE_BUTTON_BOTTOM:

		MOVE_BOTTOM_TO(rect, point.y);
		SetSelectedRect(rect);
		break;

	case RESIZE_BUTTON_RIGHT_TOP:

		MOVE_RIGHT_TO(rect, point.x);
		MOVE_TOP_TO(rect, point.y);
		SetSelectedRect(rect);
		break;

	case RESIZE_BUTTON_LEFT_BOTTON:

		MOVE_LEFT_TO(rect, point.x);
		MOVE_BOTTOM_TO(rect, point.y);
		SetSelectedRect(rect);
		break;

	case RESIZE_BUTTON_LEFT_TOP:
		MOVE_LEFT_TO(rect, point.x);
		MOVE_TOP_TO(rect, point.y);
		SetSelectedRect(rect);
		break;

	case RESIZE_BUTTON_RIGHT_BOTTOM:
		MOVE_RIGHT_TO(rect, point.x);
		MOVE_BOTTOM_TO(rect, point.y);
		SetSelectedRect(rect);
		break;
	default:

		break;
	}
}

void SCSelectTool::SetState(int state)
{
	if (m_state == state) {
		return;
	}

	m_state = state;

	// nodify listeners
	NotifyState((SCSelectState)m_state);
}

void SCSelectTool::OnLButtonDown(UINT nFlags, CPoint point)
{
	BOOL bRet = FALSE;

	CRect selectedRect;
	GetSelectedRect(selectedRect);

	// 当前还没有选定区域，按下鼠标左键开始选择，设置状态为选择，同时设置选择的开始位置
	if (m_state == SC_SELECT_STATE_NONE) {
		// 设置当前的状态为选择
		SetState(SC_SELECT_STATE_SELECT);
		m_selectStartPoint = point;
		return;
	}

	// 如果已经完成选择，并且鼠标在调整区域殿下，那么切换到改变大小的状态
	m_resizeLocation = IsPointInResizeButton(point);
	SCDbg("m_resizeLocation=%d\n", m_resizeLocation);
	if (m_state == SC_SELECT_STATE_FINISHED && m_resizeLocation) {
		SetState(SC_SELECT_STATE_RESIZE);
		GetSelectedRect(m_resizeBeginRect);
		return;
	}

	// 如果已经完成选择，并且鼠标在选择区域点下，那么切换到移动的状态
	if (m_state == SC_SELECT_STATE_FINISHED && selectedRect.PtInRect(point)) {
		SetState(SC_SELECT_STATE_MOVE);
		m_moveStartPoint = point;
		return;
	}
}

void SCSelectTool::OnLButtonUp(UINT nFlags, CPoint point)
{
	CRect selectedRect;
	GetSelectedRect(selectedRect);

	switch (m_state)
	{
	case SC_SELECT_STATE_SELECT:
		// 选择完成
		if (!selectedRect.IsRectEmpty()) {
			SetState(SC_SELECT_STATE_FINISHED);
		}
		else {
			// 没有画出选择区域，则回退到没有选择的状态
			SetState(SC_SELECT_STATE_NONE);
		}

		break;
	case SC_SELECT_STATE_RESIZE:
		SetState(SC_SELECT_STATE_FINISHED);
		break;

	case SC_SELECT_STATE_MOVE:
		SetState(SC_SELECT_STATE_FINISHED);
		break;
	default:
		break;
	}
}

void SCSelectTool::OnMouseMove(UINT nFlags, CPoint point)
{
	// 如果当前正处于选择状态，那么改变选区的大小
	switch (m_state)
	{
	case SC_SELECT_STATE_SELECT:
	{
		CRect selectedRect;
		selectedRect.left = m_selectStartPoint.x > point.x ? point.x : m_selectStartPoint.x;
		selectedRect.right = m_selectStartPoint.x < point.x ? point.x : m_selectStartPoint.x;

		selectedRect.top = m_selectStartPoint.y > point.y ? point.y : m_selectStartPoint.y;
		selectedRect.bottom = m_selectStartPoint.y < point.y ? point.y : m_selectStartPoint.y;

		SetSelectedRect(selectedRect);
		break;
	}
	case SC_SELECT_STATE_MOVE:
	{
		CPoint offSet = point - m_moveStartPoint;
		MoveSelectedRect(offSet);
		m_moveStartPoint = point;
		break;
	}
	case SC_SELECT_STATE_RESIZE:
		ResizeSelectedRect(m_resizeLocation, point);
		break;

	case SC_SELECT_STATE_NONE:
	case SC_SELECT_STATE_FINISHED:
		//UpdateCursorIcon(point);
		break;
	default:
		break;
	}
}

void SCSelectTool::OnRButtonDown(UINT nFlags, CPoint point)
{

}

void SCSelectTool::OnRButtonUp(UINT nFlags, CPoint point)
{

}

void SCSelectTool::OnLButtonDblClk(UINT nFlags, CPoint point)
{

}