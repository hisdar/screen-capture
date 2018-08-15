#pragma once


#include "base-def.h"
#include "UserDraw.h"
#include "SCToolBar.h"

// SCedit 对话框

class SCEdit : public CDialogEx
{
	DECLARE_DYNAMIC(SCEdit)

public:
	SCEdit(HMONITOR hMonitor, CWnd *pParent = NULL);   // 标准构造函数

	virtual ~SCEdit();

// 对话框数据
	enum { IDD = IDD_DIALOG_SC_EDIT };

private:
	BOOL GetMonitorCDCMem(HMONITOR hMonitor, CDC * pCDC, CBitmap *pBmp);
	BOOL CreateCompatibleCDC(CDC *pSrcCDC, CDC *pTagetCDC, CBitmap *pTargetBmp);
	BOOL CreateUserDraw(CPoint * pPoint);
	BOOL DrawScreenCaptureResult(CDC * pCDC);
	BOOL CopyCompatibleCDC(CDC *pSrcCDC, CDC *pTagetCDC, CBitmap *pTargetBmp);
	BOOL ModifyUserDrawEndPoint(CPoint * pPoint);
	BOOL DrawUserDraw(CDC * pCDC);
	BOOL DrawUserDrawArrow(CDC * pCDC, CUserDraw * pUserDraw);
	BOOL DrawUserDrawCircle(CDC * pCDC, CUserDraw * pUserDraw);
	BOOL DrawMetteRect(CDC *pCDC, int startX, int startY, int width, int height);
	BOOL DrawMette(CDC *pCDC);
	BOOL DrawSelectedArea(CDC * pCDC);
	BOOL DrawUserDrawRect(CDC * pCDC, CUserDraw * pUserDraw);
	BOOL DrawUserDrawText(CDC * pCDC, CUserDraw * pUserDraw);
	BOOL DrawInformation(CDC * pCDC);
	BOOL GetScreenCDCMem(CDC * pCDC, CBitmap *pBmp);
	void UpdateCursorIcon(CPoint *pPoint);
	void UpdateSelecteAreaLocation(CPoint * pPoint, int direction);
	void UpdateSelectedArea(int direction, CPoint * pPoint);

	BOOL GetEditModeCursorIcon(CPoint *pPoint, LONG *cursorIcon);
	BOOL GetSelecteModeCursorIcon(CPoint *pPoint, LONG *cursorIcon);
	BOOL GetSelectedAreaRect(CRect * pCrect);
	BOOL GetAdjustToolRectList(CRect * pCrect, int size);
	int  GetReSizeDirection(CPoint * point);
	BOOL IsPointInRect(CPoint * pPoint, CRect * pCRect);
	BOOL GetScreenCaptureBitmap(CBitmap *pSelectedBitmap);

	void SetWindowTextW(BOOL wParam);
	void SetDrawArrow(BOOL wParam);
	void SetDrawCircel(BOOL wParam);
	void SetDrawRect(BOOL wParam);
	void Reset(void);

private:
	HMONITOR m_hMonitor;
	CDC m_userDrawCDC;
	CDC m_screenCDCMem;

	CBitmap m_userDrawBmp;
	CBitmap m_screenBmpMem;

	int m_resizeDirection;
	int m_screenCaptureState;
	int m_selectAreaEditMode;

	CPoint m_startLocation;
	CPoint m_endLocation;

	COLORREF m_color;
	int m_size;
	int m_fontSize;
	int m_rectSize;
	int m_arrowSize;
	int m_circleSize;

	CPoint m_selecteAreaLastPoint;
	CPoint m_ptOrigin;
	CString m_messageStr;
	CString m_strLine;
	
	BOOL m_isMouseLButtonDown;
	
	CSCToolBar m_toolBar;
	CSCToolBarSet m_toolBarSet;

	CArray<CUserDraw> m_userDrawArray;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};
