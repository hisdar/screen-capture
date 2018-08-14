
// ScreenCaptureDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "atltypes.h"
#include "SCToolBar.h"
#include "atltime.h"
#include "UserDraw.h"
#include "sctoolbarset.h"

#define ADJUST_CRECT_ARRAY_LEN	8
#define HORT_KEY_ID_CTRL_ALT_A	5402

#define RESIZE_LOCATION_NONE		0
#define RESIZE_LOCATION_NORTHWEST	1
#define RESIZE_LOCATION_SOUTHEAST	2
#define RESIZE_LOCATION_NORTHEAST	3
#define RESIZE_LOCATION_SOUTHWEST	4
#define RESIZE_LOCATION_NORTH		5
#define RESIZE_LOCATION_SOUTH		6
#define RESIZE_LOCATION_WEST		7
#define RESIZE_LOCATION_EAST		8
#define RESIZE_LOCATION_AREA		9

#define SCREEN_CAPTURE_STATE_NONE		0
#define SCREEN_CAPTURE_STATE_START		1
#define SCREEN_CAPTURE_STATE_RESIZE		2

#define SELECT_AREA_EDIT_NONE			0
#define SELECT_AREA_EDIT_DRAW_RECT		1
#define SELECT_AREA_EDIT_DRAW_CIRCLE	2
#define SELECT_AREA_EDIT_DRAW_ARROW		3
#define SELECT_AREA_EDIT_DRAW_TEXT		4

#define FONT_ADJUST						8
#define DEFAULT_FONT					_T("微软雅黑")

// CScreenCaptureDlg 对话框
class CScreenCaptureDlg : public CDialogEx
{
// 构造
public:
	CScreenCaptureDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SCREENCAPTURE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
private:
	CBitmap m_screenBmpMem;
	
	CBitmap m_userDrawBmp;

	CDC m_screenCDCMem;

	CDC m_userDrawCDC;

	int m_resizeDirection;
	int m_screenCaptureState;
	int m_selectAreaEditMode;
	BOOL m_isMouseLButtonDown;
	
	CPoint m_startLocation;
	CPoint m_endLocation;

	CSCToolBar m_toolBar;

	CPoint m_selecteAreaLastPoint;
	CPoint m_ptOrigin;
	CString m_messageStr;
	CString m_strLine;

	COLORREF m_color;
	int m_size;
	int m_fontSize;
	int m_rectSize;
	int m_arrowSize;
	int m_circleSize;

	CArray<CUserDraw> m_userDrawArray;

private:
	BOOL GetScreenCDCMem(CDC * pCDC, CBitmap *pBmp);
	BOOL DrawMette(CDC *pCDC);
	BOOL DrawMetteRect(CDC *pCDC, int startX, int startY, int width, int height);
	BOOL DrawScreenCaptureResult(CDC * pCDC);
	BOOL DrawSelectedArea(CDC * pCDC);
	void Reset(void);
	BOOL GetScreenCaptureBitmap(CBitmap *pSelectedBitmap);
	BOOL GetScreenCaptureSavePath(CString *pSavePath);

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

protected:
	afx_msg LRESULT OnScreenCaptureCancel(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnScreenCaptureOk(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnScreenCaptureSave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnScreenCaptureDrawRect(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnScreenCaptureDrawCircel(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnScreenCaptureDrawArrow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnScreenCaptureText(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnScreenCaptureUndo(WPARAM wParam, LPARAM lParam);

public:
	BOOL GetAdjustToolRectList(CRect * pCrect, int size);
	BOOL GetSelectedAreaRect(CRect * pCrect);
	int GetReSizeDirection(CPoint * point);
	void UpdateCursorIcon(CPoint *pPoint);
	void UpdateSelectedArea(int direction, CPoint * pPoint);
	BOOL DrawInformation(CDC * pCDC);
	void UpdateSelecteAreaLocation(CPoint * pPoint, int direction);
	static BOOL IsPointInRect(CPoint * pPoint, CRect * pCRect);

	BOOL CreateCompatibleCDC(CDC *pSrcCDC, CDC *pTagetCDC, CBitmap *pTargetBmp);
	BOOL CopyCompatibleCDC(CDC *pSrcCDC, CDC *pTagetCDC, CBitmap *pTargetBmp);
	BOOL CreateUserDraw(CPoint * pPoint);
	BOOL ModifyUserDrawEndPoint(CPoint * pPoint);
	
	BOOL DrawUserDraw(CDC * pCDC);
	BOOL DrawUserDrawRect(CDC * pCDC, CUserDraw * pUserDraw);
	BOOL DrawUserDrawCircle(CDC * pCDC, CUserDraw * pUserDraw);
	BOOL DrawUserDrawArrow(CDC * pCDC, CUserDraw * pUserDraw);
	BOOL DrawUserDrawText(CDC * pCDC, CUserDraw * pUserDraw);

	BOOL GetEditModeCursorIcon(CPoint *pPoint, LONG *cursorIcon);
	BOOL GetSelecteModeCursorIcon(CPoint *pPoint, LONG *cursorIcon);

	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
private:
	CSCToolBarSet m_toolBarSet;
protected:
	afx_msg LRESULT OnScreenCaptureColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnScreenCaptureSize(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};
