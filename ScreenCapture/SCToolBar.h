#pragma once
#include "afxwin.h"
#include "HButton.h"

// CSCToolBar 对话框

class CSCToolBar : public CDialogEx
{
	DECLARE_DYNAMIC(CSCToolBar)

public:
	CSCToolBar(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSCToolBar();

// 对话框数据
	enum { IDD = IDD_DIALOG_TOOL_BAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:

	HButton m_okBtn;
	HButton m_cancleBtn;
	HButton m_saveBtn;
	HButton m_undoBtn;
	HButton m_drawRectBtn;
	HButton m_drawCircleBtn;
	HButton m_drawArrowBtn;
	HButton m_textBtn;

	CBitmap m_okBmpIn;
	CBitmap m_okBmpDef;
	CBitmap m_okBmpClked;

	CBitmap m_cancelBmpIn;
	CBitmap m_cancelBmpDef;
	CBitmap m_cancelBmpClked;

	CBitmap m_saveBmpIn;
	CBitmap m_saveBmpDef;
	CBitmap m_saveBmpClked;

	CBitmap m_rectBmpIn;
	CBitmap m_rectBmpDef;
	CBitmap m_rectBmpClked;

	CBitmap m_circleBmpIn;
	CBitmap m_circleBmpDef;
	CBitmap m_circleBmpClked;

	CBitmap m_undoBmpIn;
	CBitmap m_undoBmpDef;
	CBitmap m_undoBmpClked;

	CBitmap m_arrowBmpIn;
	CBitmap m_arrowBmpDef;
	CBitmap m_arrowBmpClked;

	CBitmap m_textBmpIn;
	CBitmap m_textBmpDef;
	CBitmap m_textBmpClked;

	BOOL m_isDrawRectClicked;
	BOOL m_isDrawCircle;
	BOOL m_isDrawArrow;
	BOOL m_isDrawText;

public:
	virtual BOOL OnInitDialog();

	void Reset();

	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonCancle();
	afx_msg void OnBnClickedButtonOk();
	
//	afx_msg void OnMouseLeave();
//	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
//	afx_msg void OnSetfocusButtonOk();
	afx_msg void OnBnClickedButtonUndo();
	afx_msg void OnBnClickedButtonDrawRect();
	afx_msg void OnBnClickedButtonDrawCircle();
	afx_msg void OnBnClickedButtonDrawArrow();
	afx_msg void OnBnClickedButtonText();
//	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	HButton m_okBnt2;
	afx_msg void OnBnClickedButtonOk2();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
