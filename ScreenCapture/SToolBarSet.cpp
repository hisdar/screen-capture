// SToobBarSet.cpp : 实现文件
//

#include "stdafx.h"
#include "ScreenCapture.h"
#include "SToolBarSet.h"
#include "afxdialogex.h"


// CSToobBarSet 对话框

IMPLEMENT_DYNAMIC(CSToolBarSet, CDialogEx)

CSToolBarSet::CSToolBarSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSToolBarSet::IDD, pParent)
{

}

CSToolBarSet::~CSToolBarSet()
{
}

void CSToolBarSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_MAIN_COLOR, m_mainColorStatic);
}


BEGIN_MESSAGE_MAP(CSToolBarSet, CDialogEx)
END_MESSAGE_MAP()


// CSToobBarSet 消息处理程序
