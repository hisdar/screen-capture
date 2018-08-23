#include "stdafx.h"
#include "base/base-def.h"
#include "tool/select/SCSelectToolController.h"

#define MOVE_LEFT_TO(rect, x) \
	if ((x) > (rect).right) {\
		(rect).left = (rect).right;\
		(rect).right = (x);\
	} else {\
		(rect).left = (x);\
	}

#define MOVE_RIGHT_TO(rect, x) \
	if ((rect).left > (x)) {\
		(rect).right = (rect).left;\
		(rect).left = (x);\
	} else {\
		(rect).right = (x);\
	}

#define MOVE_TOP_TO(rect, y) \
	if ((rect).bottom < (y)) {\
		(rect).top = (rect).bottom;\
		(rect).bottom = (y);\
	} else {\
		(rect).top = (y);\
	}

#define MOVE_BOTTOM_TO(rect, y)\
	if ((rect).top > (y)) {\
		(rect).bottom = (rect).top; \
		(rect).top = (y); \
	} else {	\
		(rect).bottom = (y); \
	}

SCSelectToolController::SCSelectToolController(SCSelectToolView *view)
: m_view(view)
, m_state(SC_SELECT_STATE_NONE)
{
	UpdateCursorIcon(CPoint(0, 0));
}

SCSelectToolController::~SCSelectToolController()
{

}

int SCSelectToolController::GetState()
{
	return m_state;
}

void SCSelectToolController::UpdateCursorIcon(CPoint &point)
{
	int ret = FALSE;
	LONG cursorIcon = 0;

	cursorIcon = (LONG)LoadCursor(NULL, IDC_ARROW);

	int resizeLocation = m_view->IsPointInResizeButton(point);
	if (resizeLocation > 0) {
		
		ret = GetResizeStateCursorIcon(resizeLocation, &cursorIcon);
		if (!ret) {
			SCErr("GetResizeStateCursorIcon fail, resizeLocation=%d\n", resizeLocation);
		}

	} else if (m_view->IsPointInSelectedRect(point)) {
		cursorIcon = (LONG)LoadCursor(NULL, IDC_SIZEALL);
	}

	m_view->SetCursorIcon(cursorIcon);
}

BOOL SCSelectToolController::GetResizeStateCursorIcon(int resizeLocation, LONG *cursorIcon)
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

void SCSelectToolController::MoveSelectedRect(CPoint &point)
{
	CRect rect;
	CRect viewRect;

	m_view->GetRect(viewRect);
	m_view->GetSelectedRect(rect);
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
	m_view->SetSelectedRect(rect);
}


void SCSelectToolController::ResizeSelectedRect(int location, CPoint &point)
{
	CRect rect;
	rect = m_resizeBeginRect;

	switch (location)
	{
	case RESIZE_BUTTON_LEFT:
		
		MOVE_LEFT_TO(rect, point.x);
		m_view->SetSelectedRect(rect);
		break;

	case RESIZE_BUTTON_RIGHT:

		MOVE_RIGHT_TO(rect, point.x)
		m_view->SetSelectedRect(rect);
		break;

	case RESIZE_BUTTON_TOP:
		
		MOVE_TOP_TO(rect, point.y);		
		m_view->SetSelectedRect(rect);
		break;

	case RESIZE_BUTTON_BOTTOM:
		
		MOVE_BOTTOM_TO(rect, point.y);
		m_view->SetSelectedRect(rect);
		break;

	case RESIZE_BUTTON_RIGHT_TOP:

		MOVE_RIGHT_TO(rect, point.x);
		MOVE_TOP_TO(rect, point.y);
		m_view->SetSelectedRect(rect);
		break;

	case RESIZE_BUTTON_LEFT_BOTTON:

		MOVE_LEFT_TO(rect, point.x);
		MOVE_BOTTOM_TO(rect, point.y);
		m_view->SetSelectedRect(rect);
		break;

	case RESIZE_BUTTON_LEFT_TOP:
		MOVE_LEFT_TO(rect, point.x);
		MOVE_TOP_TO(rect, point.y);
		m_view->SetSelectedRect(rect);
		break;

	case RESIZE_BUTTON_RIGHT_BOTTOM:
		MOVE_RIGHT_TO(rect, point.x);
		MOVE_BOTTOM_TO(rect, point.y);
		m_view->SetSelectedRect(rect);
		break;
	default:

		break;
	}
}

void SCSelectToolController::OnLButtonDown(UINT nFlags, CPoint point)
{
	BOOL bRet = FALSE;

	if (m_view == NULL) {
		return;
	}

	CRect selectedRect;
	m_view->GetSelectedRect(selectedRect);

	// ��ǰ��û��ѡ�����򣬰�����������ʼѡ������״̬Ϊѡ��ͬʱ����ѡ��Ŀ�ʼλ��
	if (m_state == SC_SELECT_STATE_NONE) {
		// ���õ�ǰ��״̬Ϊѡ��
		m_state = SC_SELECT_STATE_SELECT;
		m_selectStartPoint = point;
		return;
	}

	// ����Ѿ����ѡ�񣬲�������ڵ���������£���ô�л����ı��С��״̬
	m_resizeLocation = m_view->IsPointInResizeButton(point);
	SCDbg("m_resizeLocation=%d\n", m_resizeLocation);
	if (m_state == SC_SELECT_STATE_FINISHED && m_resizeLocation) {
		m_state = SC_SELECT_STATE_RESIZE;
		m_view->GetSelectedRect(m_resizeBeginRect);
		return;
	}

	// ����Ѿ����ѡ�񣬲��������ѡ��������£���ô�л����ƶ���״̬
	if (m_state == SC_SELECT_STATE_FINISHED && selectedRect.PtInRect(point)) {
		m_state = SC_SELECT_STATE_MOVE;
		m_moveStartPoint = point;
		return;
	}
}

void SCSelectToolController::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_view == NULL) {
		return;
	}

	CRect selectedRect;
	m_view->GetSelectedRect(selectedRect);

	switch (m_state)
	{
	case SC_SELECT_STATE_SELECT:
		// ѡ�����
		if (!selectedRect.IsRectEmpty()) {
			m_state = SC_SELECT_STATE_FINISHED;
		}
		else {
			// û�л���ѡ����������˵�û��ѡ���״̬
			m_state = SC_SELECT_STATE_NONE;
		}

		break;
	case SC_SELECT_STATE_RESIZE:
		m_state = SC_SELECT_STATE_FINISHED;
		break;

	case SC_SELECT_STATE_MOVE:
		m_state = SC_SELECT_STATE_FINISHED;
		break;
	default:
		break;
	}
}

void SCSelectToolController::OnMouseMove(UINT nFlags, CPoint point)
{
	// �����ǰ������ѡ��״̬����ô�ı�ѡ���Ĵ�С
	switch (m_state)
	{
	case SC_SELECT_STATE_SELECT:
	{
		CRect selectedRect;
		selectedRect.left = m_selectStartPoint.x > point.x ? point.x : m_selectStartPoint.x;
		selectedRect.right = m_selectStartPoint.x < point.x ? point.x : m_selectStartPoint.x;

		selectedRect.top = m_selectStartPoint.y > point.y ? point.y : m_selectStartPoint.y;
		selectedRect.bottom = m_selectStartPoint.y < point.y ? point.y : m_selectStartPoint.y;
		
		m_view->SetSelectedRect(selectedRect);
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
		UpdateCursorIcon(point);
		break;
	default:
		break;
	}
}

void SCSelectToolController::OnRButtonDown(UINT nFlags, CPoint point)
{

}

void SCSelectToolController::OnRButtonUp(UINT nFlags, CPoint point)
{

}

void SCSelectToolController::OnLButtonDblClk(UINT nFlags, CPoint point)
{

}
