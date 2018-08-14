#pragma once
#include "hstatic.h"


// CSToobBarSet 对话框

class CSToolBarSet : public CDialogEx
{
	DECLARE_DYNAMIC(CSToolBarSet)

public:
	CSToolBarSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSToolBarSet();

// 对话框数据
	enum { IDD = IDD_DIALOG_TOOL_BAR_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	HStatic m_mainColorStatic;
};
