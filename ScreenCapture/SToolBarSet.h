#pragma once
#include "hstatic.h"


// CSToobBarSet �Ի���

class CSToolBarSet : public CDialogEx
{
	DECLARE_DYNAMIC(CSToolBarSet)

public:
	CSToolBarSet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSToolBarSet();

// �Ի�������
	enum { IDD = IDD_DIALOG_TOOL_BAR_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	HStatic m_mainColorStatic;
};
