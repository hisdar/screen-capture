#pragma once

#include "tool/base/SCBaseToolView.h"

#define RESIZE_BUTTON_LEFT          1
#define RESIZE_BUTTON_LEFT_TOP      2
#define RESIZE_BUTTON_TOP           3
#define RESIZE_BUTTON_RIGHT_TOP     4
#define RESIZE_BUTTON_RIGHT         5
#define RESIZE_BUTTON_RIGHT_BOTTOM  6
#define RESIZE_BUTTON_BOTTOM        7
#define RESIZE_BUTTON_LEFT_BOTTON   8

class SCSelectToolView : public SCBaseToolView
{
public:
	SCSelectToolView(CWnd *wnd);
	virtual ~SCSelectToolView();

private:
	typedef CMap<unsigned, unsigned, CRect, CRect> ResizeButtonRects;

private:
	CRect m_rect;
	CRect m_selectedRect;
	ResizeButtonRects m_resizeButtonRects;
	CWnd *m_wnd;

public:
	virtual BOOL Draw(CDC &cdc);

	void SetRect(const CRect &rect);
	void GetRect(CRect &rect);
	void SetSelectedRect(const CRect &rect);
	void GetSelectedRect(CRect &rect);
	BOOL IsPointInSelectedRect(CPoint &point);
	int  IsPointInResizeButton(CPoint &point);

	BOOL SetCursorIcon(LONG cursorIcon);

	BOOL DrawMask(CDC &cdc);
	BOOL DrawSelectedArea(CDC * pCDC);


private:
	void GetResizeButtonRects(ResizeButtonRects &resizeButtonRects);
};

