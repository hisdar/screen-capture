#pragma once

#include "base/hstatic.h"
#include "resource.h"
#include "afxwin.h"

#define DEFAULT_SIZE	2
#define DEFAULT_COLOR	RGB(255, 0, 0)
// CSCToolBarSet 对话框

class CSCToolBarSet : public CDialogEx
{
	DECLARE_DYNAMIC(CSCToolBarSet)

public:
	CSCToolBarSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSCToolBarSet();

// 对话框数据
	enum { IDD = IDD_DIALOG_TOOL_BAR_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	HStatic m_mainColorStatic;
	afx_msg void OnPaint();
	afx_msg void OnStnClickedStaticMainColor();
	CStatic m_sizeStatic;

	HStatic m_color2Static;
	HStatic m_color3Static;
	HStatic m_color4Static;
	HStatic m_color5Static;
	HStatic m_color6Static;
	HStatic m_color7Static;
	HStatic m_color8Static;
	HStatic m_color9Static;
	HStatic m_color10Static;
	HStatic m_color11Static;
	HStatic m_color12Static;
	HStatic m_color13Static;
	HStatic m_color14Static;
	HStatic m_color15Static;
	HStatic m_color16Static;
	HStatic m_color17Static;

	afx_msg void OnStnClickedStaticMainColor2();
	afx_msg void OnStnClickedStaticMainColor3();
	afx_msg void OnStnClickedStaticMainColor4();
	afx_msg void OnStnClickedStaticMainColor5();
	afx_msg void OnStnClickedStaticMainColor6();
	afx_msg void OnStnClickedStaticMainColor7();
	afx_msg void OnStnClickedStaticMainColor8();
	afx_msg void OnStnClickedStaticMainColor9();
	afx_msg void OnStnClickedStaticMainColor10();
	afx_msg void OnStnClickedStaticMainColor11();
	afx_msg void OnStnClickedStaticMainColor12();
	afx_msg void OnStnClickedStaticMainColor13();
	afx_msg void OnStnClickedStaticMainColor14();
	afx_msg void OnStnClickedStaticMainColor15();
	afx_msg void OnStnClickedStaticMainColor16();
	afx_msg void OnStnClickedStaticMainColor17();
	virtual BOOL OnInitDialog();

	COLORREF GetSelectedColor();
	int GetFontSize();
	void SetFontSize(int size);

	CEdit m_fontSizeEdit;
	afx_msg void OnEnChangeEditSize();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
