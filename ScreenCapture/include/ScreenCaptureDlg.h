
// ScreenCaptureDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "atltypes.h"
#include "SCToolBar.h"
#include "atltime.h"
#include "UserDraw.h"
#include "sctoolbarset.h"
#include "base/SCObject.h"
#include "SCDrawPanel.h"

// CScreenCaptureDlg 对话框
class CScreenCaptureDlg : public CDialogEx, public SCObject, public SCDrawPanelListener
{
// 构造
public:
	CScreenCaptureDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SCREENCAPTURE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	virtual void SelectedAreaChangeEvent(const SCDrawPanel *scDrawPanel, CRect &rect);

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	
private:
	typedef CArray<SCDrawPanel *> SCDrawPanalArray;
private:
	CBitmap m_screenBmpMem;
	
	CBitmap m_userDrawBmp;

	CSCToolBar m_toolBar;
	CSCToolBarSet m_toolBarSet;

	SCDrawPanalArray m_dpArray;

	COLORREF m_color;

	int m_rectSize;
	int m_circleSize;
	int m_arrowSize;
	int m_fontSize;

	int m_selectAreaEditMode;

private:
	BOOL GetScreenCDCMem(CDC * pCDC, CBitmap *pBmp);
	BOOL GetScreenCaptureBitmap(CBitmap *pSelectedBitmap);
	BOOL GetScreenCaptureSavePath(CString *pSavePath);

public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);

protected:
	afx_msg LRESULT OnScreenCaptureCancel(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnScreenCaptureOk(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnScreenCaptureSave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnScreenCaptureDrawRect(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnScreenCaptureDrawCircel(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnScreenCaptureDrawArrow(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnScreenCaptureText(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnScreenCaptureUndo(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnScreenCaptureColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnScreenCaptureSize(WPARAM wParam, LPARAM lParam);

public:
	BOOL DrawUserDrawRect(CDC * pCDC, CUserDraw * pUserDraw);
	BOOL DrawUserDrawCircle(CDC * pCDC, CUserDraw * pUserDraw);
	BOOL DrawUserDrawArrow(CDC * pCDC, CUserDraw * pUserDraw);
	BOOL DrawUserDrawText(CDC * pCDC, CUserDraw * pUserDraw);


	virtual BOOL PreTranslateMessage(MSG* pMsg);


public:

};
