// ScreenCaptureDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ScreenCapture.h"
#include "ScreenCaptureDlg.h"
#include "afxdialogex.h"
#include "SCToolBar.h"

#include <GdiPlus.h>
#include <Windows.h>

using namespace Gdiplus;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CScreenCaptureDlg 对话框

CScreenCaptureDlg::CScreenCaptureDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CScreenCaptureDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CScreenCaptureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CScreenCaptureDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_HOTKEY()
	ON_MESSAGE(SCREEN_CAPTURE_CANCEL, &CScreenCaptureDlg::OnScreenCaptureCancel)
	ON_MESSAGE(SCREEN_CAPTURE_OK, &CScreenCaptureDlg::OnScreenCaptureOk)
	ON_MESSAGE(SCREEN_CAPTURE_SAVE, &CScreenCaptureDlg::OnScreenCaptureSave)
	ON_WM_LBUTTONDBLCLK()
	ON_MESSAGE(SCREEN_CAPTURE_DRAW_RECT, &CScreenCaptureDlg::OnScreenCaptureDrawRect)
	ON_MESSAGE(SCREEN_CAPTURE_DRAW_CIRCEL, &CScreenCaptureDlg::OnScreenCaptureDrawCircel)
	ON_MESSAGE(SCREEN_CAPTURE_DRAW_ARROW, &CScreenCaptureDlg::OnScreenCaptureDrawArrow)
	ON_MESSAGE(SCREEN_CAPTURE_TEXT, &CScreenCaptureDlg::OnScreenCaptureText)
	ON_MESSAGE(SCREEN_CAPTURE_UNDO, &CScreenCaptureDlg::OnScreenCaptureUndo)
	ON_MESSAGE(SCREEN_CAPTURE_COLOR, &CScreenCaptureDlg::OnScreenCaptureColor)
	ON_MESSAGE(SCREEN_CAPTURE_SIZE, &CScreenCaptureDlg::OnScreenCaptureSize)
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()


// CScreenCaptureDlg 消息处理程序

BOOL CScreenCaptureDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//HShortCut shortCut;
	//shortCut.CreateShortCut(_T(""), _T(""));

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	BOOL bRet = TRUE;

	bRet = RegisterHotKey(m_hWnd, HORT_KEY_ID_CTRL_ALT_A, MOD_CONTROL | MOD_ALT, 'A');
	if (!bRet) {
		MessageBox(_T("ALT registe fail"), _T("Message"), MB_OK);
	}

	// hide window when start
	ModifyStyleEx(WS_EX_APPWINDOW,WS_EX_TOOLWINDOW); //从任务栏中去掉.
	SetWindowPos(&wndTop, 0, 0, 0, 0, NULL);

	m_toolBar.Create(IDD_DIALOG_TOOL_BAR, this);
	m_toolBarSet.Create(IDD_DIALOG_TOOL_BAR_SET, this);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。


////////////////////////////////////////////////////////////////////////////////////////////////////////
void CScreenCaptureDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		// CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CScreenCaptureDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CScreenCaptureDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	if (nHotKeyId == HORT_KEY_ID_CTRL_ALT_A) {
		// MessageBox(_T("Ctrl + Alt + A pressed"), _T("message"), MB_OK);

		// get screen size
		int width = GetSystemMetrics(SM_CXSCREEN);
		int height = GetSystemMetrics(SM_CYSCREEN);
		TRACE("%s-%d:SM_CXSCREEN=%d, SM_CYSCREEN=%d\n", __FUNCTION__, __LINE__, width, height);

		// 获取虚拟屏幕的长宽，虚拟屏幕就是所有屏幕拼起来的大小
		int cxWidth = GetSystemMetrics(SM_CXVIRTUALSCREEN);
		int cyHeight = GetSystemMetrics(SM_CYVIRTUALSCREEN);
		TRACE("%s-%d:SM_CXVIRTUALSCREEN=%d, SM_CYVIRTUALSCREEN=%d\n", __FUNCTION__, __LINE__, cxWidth, cyHeight);
		
		int xv = GetSystemMetrics(SM_XVIRTUALSCREEN);
		int yv = GetSystemMetrics(SM_YVIRTUALSCREEN);
		TRACE("%s-%d:SM_XVIRTUALSCREEN=%d, SM_YVIRTUALSCREEN=%d\n", __FUNCTION__, __LINE__, xv, yv);

		for (int i = 0; i < m_monitors.GetSize(); i++) {
			MonitorInfor mi = m_monitors.GetAt(i);
			delete mi.scEdit;
		}

		m_monitors.RemoveAll();
		EnumDisplayMonitors(NULL, NULL, SCMonitorEnumProc, (LPARAM)this);
		TRACE("monitor size:%d\n", m_monitors.GetSize());

		for (int i = 0; i < m_monitors.GetSize(); i++) {
			MonitorInfor mi = m_monitors.GetAt(i);
			RECT rect = mi.lprcMonitor;
			mi.scEdit->SetWindowPos(&wndTopMost, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);
		}

		/*GetScreenCDCMem(&m_screenCDCMem, &m_screenBmpMem);
		 
		int monitorCount = GetSystemMetrics(SM_CMONITORS);
		TRACE("%s-%d:monitorCount=%d\n", __FUNCTION__, __LINE__, monitorCount);

		SetWindowPos(&wndTopMost, 0, 0, width, height, SWP_SHOWWINDOW);
		ShowWindow(SW_SHOW);*/

		// set screen capture state to 
		m_screenCaptureState = SCREEN_CAPTURE_STATE_START;
	}

	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}


BOOL CScreenCaptureDlg::GetScreenCaptureSavePath(CString *pSavePath)
{
	BOOL bRet = FALSE;
	BOOL isOpen = FALSE;		//是否打开(否则为保存)  
 
	CString fileDir;			//默认打开的文件路径 
	CString fileName;			//默认打开的文件名  
	CString filePath;
	CString filter;				//文件过虑的类型  
	TCHAR desktopDir[_MAX_PATH] = {0};

	// set filter
	filter = _T("PNG(*.png)|*.png|JPEG(*.jpg)|*.jpg|GIF(*.gif)|*.gif|BMP(*.bmp)|*.bmp;||");
	
	// get currate time
	CTime tm = CTime::GetCurrentTime();
	fileName.Format(_T("ScreenCapture_%04d-%02d-%02d_%02d-%02d-%02d.png"),
					tm.GetYear(), tm.GetMonth(), tm.GetDay(),
					tm.GetHour(), tm.GetMinute(), tm.GetSecond());

	// get default save path - desktop
	bRet = SHGetSpecialFolderPath(GetSafeHwnd(), desktopDir, CSIDL_DESKTOP, FALSE);
	if (bRet) {
		fileDir.Format(_T("%s"), desktopDir);
	}

	// init file save dialog
	CFileDialog openFileDlg(isOpen, fileDir, fileName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, NULL);  
	openFileDlg.GetOFN().lpstrInitialDir = filePath;  
	openFileDlg.m_ofn.lpstrTitle = _T("保存");
	filePath = fileDir + "\\" + fileName;

	INT_PTR result = openFileDlg.DoModal();
	if (result == IDOK) {  
		filePath = openFileDlg.GetPathName();  
	} else {
		return FALSE;
	}

	pSavePath->SetString(filePath);
	return TRUE;
}

/////////////////////////////////////自定义消息/////////////////////////////////////////////////////////////
afx_msg LRESULT CScreenCaptureDlg::OnScreenCaptureCancel(WPARAM wParam, LPARAM lParam)
{
	ShowWindow(SW_HIDE);
	Reset();

	return 0;
}


afx_msg LRESULT CScreenCaptureDlg::OnScreenCaptureOk(WPARAM wParam, LPARAM lParam)
{
	// get selected bit map
	CBitmap selectedBitmap;
	BOOL bRet = GetScreenCaptureBitmap(&selectedBitmap);

	// copy bitmap to clip board
	if (OpenClipboard()) {
		EmptyClipboard();
		SetClipboardData(CF_BITMAP, selectedBitmap.GetSafeHandle());
		CloseClipboard();
	}

	selectedBitmap.DeleteObject();

	Reset();
	ShowWindow(SW_HIDE);

	return 0;
}

afx_msg LRESULT CScreenCaptureDlg::OnScreenCaptureSave(WPARAM wParam, LPARAM lParam)
{

	// get the file save path
	CString savePath;
	BOOL bRet = GetScreenCaptureSavePath(&savePath);
	if (!bRet) {
		m_toolBar.ShowWindow(SW_SHOW);
		return 0;
	}

	// get selected bit map
	CBitmap selectedBitmap;
	bRet = GetScreenCaptureBitmap(&selectedBitmap);

	// exchange bitmap to image
	CImage selectedImage;
	selectedImage.Attach((HBITMAP)selectedBitmap.GetSafeHandle());

	selectedImage.Save(savePath);

	selectedBitmap.DeleteObject();
	selectedImage.Destroy();

	ShowWindow(SW_HIDE);
	Reset();

	return 0;
}


afx_msg LRESULT CScreenCaptureDlg::OnScreenCaptureDrawRect(WPARAM wParam, LPARAM lParam)
{
	// rename to SetDrawRect

	return 0;
}


afx_msg LRESULT CScreenCaptureDlg::OnScreenCaptureDrawCircel(WPARAM wParam, LPARAM lParam)
{
	// rename to SetDrawCircel

	return 0;
}


afx_msg LRESULT CScreenCaptureDlg::OnScreenCaptureDrawArrow(WPARAM wParam, LPARAM lParam)
{
	// rename to SetDrawArrow

	return 0;
}


afx_msg LRESULT CScreenCaptureDlg::OnScreenCaptureText(WPARAM wParam, LPARAM lParam)
{
	// rename to SetText

	return 0;
}


afx_msg LRESULT CScreenCaptureDlg::OnScreenCaptureUndo(WPARAM wParam, LPARAM lParam)
{
	if (m_userDrawArray.GetSize() <= 0) {
		return 0;
	}

	m_userDrawArray.RemoveAt(m_userDrawArray.GetSize() - 1);

	Invalidate(TRUE);
	UpdateWindow();

	return 0;
}

afx_msg LRESULT CScreenCaptureDlg::OnScreenCaptureColor(WPARAM wParam, LPARAM lParam)
{
	m_color = m_toolBarSet.GetSelectedColor();

	return 0;
}


afx_msg LRESULT CScreenCaptureDlg::OnScreenCaptureSize(WPARAM wParam, LPARAM lParam)
{
	int size = m_toolBarSet.GetFontSize();

	switch (m_selectAreaEditMode)
	{
	case SELECT_AREA_EDIT_DRAW_ARROW:
		m_arrowSize = size;
		break;
	case SELECT_AREA_EDIT_DRAW_CIRCLE:
		m_circleSize = size;
		break;
	case SELECT_AREA_EDIT_DRAW_RECT:
		m_rectSize = size;
		break;
	case SELECT_AREA_EDIT_DRAW_TEXT:
		m_fontSize = size;
		break;
	default:
		break;
	}

	return 0;
}




BOOL CScreenCaptureDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYUP)  
	{  
		SendMessage(pMsg->message, pMsg->wParam, pMsg->lParam);  
	}  

	return FALSE;  
	//return CDialogEx::PreTranslateMessage(pMsg);
}

void CScreenCaptureDlg::OnSetFocus(CWnd* pOldWnd)
{
	CDialogEx::OnSetFocus(pOldWnd);

	// CreateSolidCaret(2, 20);
}


void CScreenCaptureDlg::OnKillFocus(CWnd* pNewWnd)
{
	CDialogEx::OnKillFocus(pNewWnd);

	DestroyCaret();
}

BOOL CScreenCaptureDlg::SCMonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	CScreenCaptureDlg *self = (CScreenCaptureDlg*)dwData;

	MonitorInfor mi;

	mi.hMonitor = hMonitor;
	mi.hdcMonitor = hdcMonitor;
	memcpy(&mi.lprcMonitor, lprcMonitor, sizeof(RECT));

	mi.scEdit = new SCEdit(mi.hMonitor);
	mi.scEdit->Create(IDD_DIALOG_SC_EDIT, self);
	
	self->m_monitors.Add(mi);

	return TRUE;
}

