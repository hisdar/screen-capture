#pragma once

#include "tool/base/SCBaseToolController.h"
#include "tool/select/SCSelectToolView.h"

class SCSelectToolController : public SCBaseToolController
{
public:
	SCSelectToolController(SCSelectToolView *view);
	virtual ~SCSelectToolController();

	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnRButtonDown(UINT nFlags, CPoint point);
	virtual void OnRButtonUp(UINT nFlags, CPoint point);
	virtual void OnLButtonDblClk(UINT nFlags, CPoint point);

private:
	void MoveSelectedRect(CPoint &point);
	void ResizeSelectedRect(int location, CPoint &point);
	void UpdateCursorIcon(CPoint &point);
	BOOL GetResizeStateCursorIcon(int resizeLocation, LONG *cursorIcon);

private:
	BOOL m_isMouseLButtonDown;
	int m_state;
	int m_resizeLocation;
	SCSelectToolView *m_view;

	CPoint m_moveStartPoint;
	CPoint m_selectStartPoint;
	CRect m_resizeBeginRect;
};

