#pragma once

#include "tool/SCBaseTool.h"

typedef enum SCSelectState_ {
	SC_SELECT_STATE_NONE,
	SC_SELECT_STATE_SELECT,
	SC_SELECT_STATE_RESIZE,
	SC_SELECT_STATE_MOVE,
	SC_SELECT_STATE_FINISHED,
} SCSelectState;

#define RESIZE_BUTTON_LEFT          1
#define RESIZE_BUTTON_LEFT_TOP      2
#define RESIZE_BUTTON_TOP           3
#define RESIZE_BUTTON_RIGHT_TOP     4
#define RESIZE_BUTTON_RIGHT         5
#define RESIZE_BUTTON_RIGHT_BOTTOM  6
#define RESIZE_BUTTON_BOTTOM        7
#define RESIZE_BUTTON_LEFT_BOTTON   8

class SCSelectToolEventListener
{
public:
	SCSelectToolEventListener(){};
	virtual ~SCSelectToolEventListener(){};
	virtual void SelectAreaChangeEvent(const CRect &selectArea) = 0;
	virtual void SelectStateChangeEvent(SCSelectState state) = 0;
};

class SCSelectTool : public SCBaseTool
{
public:
	SCSelectTool();
	virtual ~SCSelectTool();
	virtual int GetToolType();

	int  GetState();

	virtual void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnRButtonDown(UINT nFlags, CPoint point);
	virtual void OnRButtonUp(UINT nFlags, CPoint point);
	virtual void OnLButtonDblClk(UINT nFlags, CPoint point);

	virtual BOOL Draw(CDC &cdc);

	void SetRect(const CRect &rect);
	void GetRect(CRect &rect);
	void SetSelectedRect(const CRect &rect);
	void GetSelectedRect(CRect &rect);
	BOOL IsPointInSelectedRect(CPoint &point);
	int  IsPointInResizeButton(CPoint &point);

	//BOOL SetCursorIcon(LONG cursorIcon);

	BOOL DrawMask(CDC &cdc);
	BOOL DrawSelectedArea(CDC * pCDC);

	BOOL AddListener(SCSelectToolEventListener *listener);
	void RemoveListener(SCSelectToolEventListener *listener);

private:
	typedef CMap<unsigned, unsigned, CRect, CRect> ResizeButtonRects;
	typedef CArray<SCSelectToolEventListener *> SCSelectToolEventListenerArray;

private:
	void MoveSelectedRect(CPoint &point);
	void ResizeSelectedRect(int location, CPoint &point);
	void UpdateCursorIcon(CPoint &point);
	BOOL GetResizeStateCursorIcon(int resizeLocation, LONG *cursorIcon);
	void GetResizeButtonRects(ResizeButtonRects &resizeButtonRects);
	void SetState(int state);

	void NotifyState(SCSelectState state);
	void NotifySelectArea(const CRect &selectArea);

private:
	BOOL m_isMouseLButtonDown;
	int m_state;
	int m_resizeLocation;

	CPoint m_moveStartPoint;
	CPoint m_selectStartPoint;
	CRect m_resizeBeginRect;

	CRect m_rect;
	CRect m_selectedRect;

	ResizeButtonRects m_resizeButtonRects;
	SCSelectToolEventListenerArray m_selectToolEventListenerArray;
};

