XAccessible
// SCToolBar.cpp : 实现文件
//

#include "stdafx.h"
#include "ScreenCapture.h"
#include "SCToolBar.h"
#include "afxdialogex.h"
#include "ScreenCaptureDlg.h"

#define SET_ALL_STATE(item) \
	{\
		if (item) {\
			Reset();\
			item = TRUE;\
		} else {\
			Reset();\
			item = FALSE;\
		}\
	}

// CSCToolBar 对话框

IMPLEMENT_DYNAMIC(CSCToolBar, CDialogEx)

CSCToolBar::CSCToolBar(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSCToolBar::IDD, pParent)
{

	m_isDrawRectClicked = FALSE;
	m_isDrawCircle = FALSE;
	m_isDrawArrow = FALSE;
	m_isDrawText = FALSE;
}

CSCToolBar::~CSCToolBar()
{
}

void CSCToolBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_OK, m_okBtn);
	DDX_Control(pDX, IDC_BUTTON_CANCLE, m_cancleBtn);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_saveBtn);
	DDX_Control(pDX, IDC_BUTTON_UNDO, m_undoBtn);
	DDX_Control(pDX, IDC_BUTTON_DRAW_RECT, m_drawRectBtn);
	DDX_Control(pDX, IDC_BUTTON_DRAW_CIRCLE, m_drawCircleBtn);
	DDX_Control(pDX, IDC_BUTTON_DRAW_ARROW, m_drawArrowBtn);
	DDX_Control(pDX, IDC_BUTTON_TEXT, m_textBtn);
	DDX_Control(pDX, IDC_BUTTON_OK2, m_okBnt2);
}


BEGIN_MESSAGE_MAP(CSCToolBar, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_OK, &CSCToolBar::OnBnClickedButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CSCToolBar::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_CANCLE, &CSCToolBar::OnBnClickedButtonCancle)
//	ON_WM_MOUSELEAVE()
//	ON_WM_MOUSEHOVER()
//	ON_BN_SETFOCUS(IDC_BUTTON_OK, &CSCToolBar::OnSetfocusButtonOk)
ON_BN_CLICKED(IDC_BUTTON_UNDO, &CSCToolBar::OnBnClickedButtonUndo)
ON_BN_CLICKED(IDC_BUTTON_DRAW_RECT, &CSCToolBar::OnBnClickedButtonDrawRect)
ON_BN_CLICKED(IDC_BUTTON_DRAW_CIRCLE, &CSCToolBar::OnBnClickedButtonDrawCircle)
ON_BN_CLICKED(IDC_BUTTON_DRAW_ARROW, &CSCToolBar::OnBnClickedButtonDrawArrow)
ON_BN_CLICKED(IDC_BUTTON_TEXT, &CSCToolBar::OnBnClickedButtonText)
//ON_WM_MOUSEMOVE()
ON_BN_CLICKED(IDC_BUTTON_OK2, &CSCToolBar::OnBnClickedButtonOk2)
END_MESSAGE_MAP()


// CSCToolBar 消息处理程序

BOOL CSCToolBar::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetBackgroundColor(RGB(234, 238, 245));

	m_okBtn.HLoadBitMap(IDB_BITMAP_OK_DEF, IDB_BITMAP_OK_IN, IDB_BITMAP_OK_CLICKED);
	m_okBnt2.HLoadBitMap(IDB_BITMAP_OK_DEF, IDB_BITMAP_OK_IN, IDB_BITMAP_OK_CLICKED);
	m_cancleBtn.HLoadBitMap(IDB_BITMAP_CANCEL_DEF, IDB_BITMAP_CANCEL_IN, IDB_BITMAP_CANCEL_CLICKED);
	m_saveBtn.HLoadBitMap(IDB_BITMAP_SAVE_DEF, IDB_BITMAP_SAVE_IN, IDB_BITMAP_SAVE_CLICKED);
	m_undoBtn.HLoadBitMap(IDB_BITMAP_UNDO_DEF, IDB_BITMAP_UNDO_IN, IDB_BITMAP_UNDO_CLICKED);
	m_drawRectBtn.HLoadBitMap(IDB_BITMAP_RECT_DEF, IDB_BITMAP_RECT_IN, IDB_BITMAP_RECT_CLICKED);
	m_drawCircleBtn.HLoadBitMap(IDB_BITMAP_CIRCLE_DEF, IDB_BITMAP_CIRCLE_IN, IDB_BITMAP_CIRCLE_CLICKED);
	m_drawArrowBtn.HLoadBitMap(IDB_BITMAP_ARROW_DEF, IDB_BITMAP_ARROW_IN, IDB_BITMAP_ARROW_CLICKED);
	m_textBtn.HLoadBitMap(IDB_BITMAP_TEXT_DEF, IDB_BITMAP_TEXT_IN, IDB_BITMAP_TEXT_CLICKED);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CSCToolBar::OnBnClickedButtonOk()
{

}

void CSCToolBar::OnBnClickedButtonOk2()
{	
	ShowWindow(SW_HIDE);
	GetParent()->PostMessage(SCREEN_CAPTURE_OK, 0, 0);
	Reset();
}


void CSCToolBar::OnBnClickedButtonSave()
{
	ShowWindow(SW_HIDE);
	GetParent()->PostMessage(SCREEN_CAPTURE_SAVE, 0, 0);
	Reset();
}

void CSCToolBar::OnBnClickedButtonCancle()
{
	ShowWindow(SW_HIDE);
	GetParent()->PostMessage(SCREEN_CAPTURE_CANCEL, 0, 0);
	Reset();
}


void CSCToolBar::OnBnClickedButtonUndo()
{
	GetParent()->PostMessage(SCREEN_CAPTURE_UNDO, 0, 0);
}


void CSCToolBar::OnBnClickedButtonDrawRect()
{
	m_isDrawRectClicked = !m_isDrawRectClicked;
	SET_ALL_STATE(m_isDrawRectClicked);
	

	m_drawRectBtn.HSetBtnClicked(m_isDrawRectClicked);
	GetParent()->PostMessage(SCREEN_CAPTURE_DRAW_RECT, m_isDrawRectClicked, 0);
}


void CSCToolBar::OnBnClickedButtonDrawCircle()
{
	m_isDrawCircle = !m_isDrawCircle;
	SET_ALL_STATE(m_isDrawCircle);

	m_drawCircleBtn.HSetBtnClicked(m_isDrawCircle);

	GetParent()->PostMessage(SCREEN_CAPTURE_DRAW_CIRCEL, m_isDrawCircle, 0);
}


void CSCToolBar::OnBnClickedButtonDrawArrow()
{
	m_isDrawArrow = !m_isDrawArrow;
	SET_ALL_STATE(m_isDrawArrow);
	m_drawArrowBtn.HSetBtnClicked(m_isDrawArrow);
	GetParent()->PostMessage(SCREEN_CAPTURE_DRAW_ARROW, m_isDrawArrow, 0);
}


void CSCToolBar::OnBnClickedButtonText()
{
	m_isDrawText = !m_isDrawText;
	SET_ALL_STATE(m_isDrawText);
	m_textBtn.HSetBtnClicked(m_isDrawText);
	GetParent()->PostMessage(SCREEN_CAPTURE_TEXT, m_isDrawText, 0);
}

void CSCToolBar::Reset()
{
	m_isDrawText = FALSE;
	m_isDrawRectClicked = FALSE;
	m_isDrawCircle = FALSE;
	m_isDrawArrow = FALSE;

	m_textBtn.HSetBtnClicked(m_isDrawText);
	m_drawRectBtn.HSetBtnClicked(m_isDrawRectClicked);
	m_drawCircleBtn.HSetBtnClicked(m_isDrawCircle);
	m_drawArrowBtn.HSetBtnClicked(m_isDrawArrow);
}


BOOL CSCToolBar::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN) {
		
		// 屏蔽回车和 ESC 
		if (pMsg->wParam==VK_ESCAPE || pMsg->wParam==VK_RETURN) {     
			return TRUE;
		}
	}

	// 屏蔽ALT+F4
	if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4 ) {
		
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
