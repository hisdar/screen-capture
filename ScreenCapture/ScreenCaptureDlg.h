
// ScreenCaptureDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "atltypes.h"

#include "atltime.h"
#include "UserDraw.h"
#include "sctoolbarset.h"
#include "SCedit.h"


#define HORT_KEY_ID_CTRL_ALT_A	5402

#include "base-def.h"
#include "SCedit.h"

typedef struct MonitorInfor
{
	HMONITOR hMonitor;
	HDC hdcMonitor;
	RECT lprcMonitor;
	SCEdit *scEdit;
} MonitorInfor;

typedef CArray<MonitorInfor> Monitors;

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
	Monitors m_monitors;

private:
	BOOL GetScreenCDCMem(CDC * pCDC, CBitmap *pBmp);
	BOOL DrawMette(CDC *pCDC);
	BOOL DrawMetteRect(CDC *pCDC, int startX, int startY, int width, int height);
	
	BOOL DrawSelectedArea(CDC * pCDC);
	void Reset(void);
	BOOL GetScreenCaptureBitmap(CBitmap *pSelectedBitmap);
	BOOL GetScreenCaptureSavePath(CString *pSavePath);
	
	static BOOL CALLBACK SCMonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);

public:
	

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
	BOOL DrawUserDrawCircle(CDC * pCDC, CUserDraw * pUserDraw);
	BOOL DrawUserDrawArrow(CDC * pCDC, CUserDraw * pUserDraw);

	BOOL GetEditModeCursorIcon(CPoint *pPoint, LONG *cursorIcon);
	BOOL GetSelecteModeCursorIcon(CPoint *pPoint, LONG *cursorIcon);

	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	afx_msg LRESULT OnScreenCaptureColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnScreenCaptureSize(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
};
