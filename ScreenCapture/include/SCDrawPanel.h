#pragma once

#include "resource.h"

#include "base/SCObject.h"
#include "UserDraw.h"

#include "tool/base/SCBaseTool.h"
#include "base/SCDC.h"

class SCDrawPanel;

class SCDrawPanelListener
{
public:
	SCDrawPanelListener() {};
	virtual ~SCDrawPanelListener() {};

	virtual void SelectedAreaChangeEvent(const SCDrawPanel *scDrawPanel, CRect &rect) {};
};

// SCEditor 对话框
class SCDrawPanel : public CDialogEx, public SCObject
{
	DECLARE_DYNAMIC(SCDrawPanel)

public:
	SCDrawPanel(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SCDrawPanel();

// 对话框数据
	enum { IDD = IDD_DIALOG_SC_DRAW_PANEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	typedef CArray<SCDrawPanelListener *> SCDrawPanelListenerArray;

public:
	BOOL UpdateBaseImage(SCDC &scDC);
	void SetState(int state);
	void SetEditMode(int editMode);
	int  GetRectSize();
	void AddListener(SCDrawPanelListener *listener);
	void RemoveListener(SCDrawPanelListener *listener);

private:
	SCDC m_scDC;

	int  m_resizeDirection;
	int  m_screenCaptureState;
	int  m_selectAreaEditMode;
	BOOL m_isMouseLButtonDown;

	COLORREF m_color;
	int m_size;
	int m_fontSize;
	int m_rectSize;
	int m_arrowSize;
	int m_circleSize;

	CPoint m_startLocation;
	CPoint m_endLocation;

	CPoint m_selecteAreaLastPoint;
	CPoint m_ptOrigin;
	CString m_messageStr;
	CString m_strLine;
	CArray<CUserDraw> m_userDrawArray;
	SCDrawPanelListenerArray m_scDrawPanelListenerArray;

	SCBaseTool *m_currentTool;
	CRect m_oldSelectedRect;

private:
	BOOL CreateUserDraw(CPoint * pPoint);
	BOOL IsPointInRect(CPoint * pPoint, CRect * pCRect);
	BOOL GetSelectedAreaRect(CRect * pCrect);
	BOOL ModifyUserDrawEndPoint(CPoint * pPoint);
	void Reset(void);
	void UpdateCursorIcon(CPoint *pPoint);
	BOOL GetEditModeCursorIcon(CPoint *pPoint, LONG *cursorIcon);
	BOOL DrawInformation(CDC * pCDC);
	BOOL DrawScreenCaptureResult(CDC * pCDC);
	BOOL DrawUserDraw(CDC * pCDC);

public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};
