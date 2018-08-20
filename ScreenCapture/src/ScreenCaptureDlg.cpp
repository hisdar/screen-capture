// ScreenCaptureDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ScreenCaptureDlg.h"
#include "afxdialogex.h"
#include "SCToolBar.h"
#include "tool/SCMaskTool.h"
#include "tool/SCRectangleTool.h"
#include "tool/SCCircleTool.h"
#include "tool/SCArrowTool.h"
#include "SCTextTool.h"
#include "base/base-def.h"
#include "SCMonitorManager.h"

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
	ON_WM_CHAR()
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
	int ret = 0;

	if (nHotKeyId == HORT_KEY_ID_CTRL_ALT_A) {

		for (; m_dpArray.GetSize() > 0;) {
			SCDrawPanel *m_dp = m_dpArray.GetAt(0);
			m_dpArray.RemoveAt(0);
			CHECK_DELETE(m_dp);
		}

		SCMonitorArray mArray;
		SCMonitorManager mMgr;
		ret = mMgr.GetMonitors(mArray);
		if (!ret) {
			SCErr("GetMonitors fail\n");
		}

		SCMonitor m(mArray.GetAt(0));

		SCDC screenSCDC;
		ret = m.GetScreenImage(screenSCDC);
		if (!ret) {
			SCErr("GetScreenCDC fail\n");
			return;
		}

		SCDrawPanel *m_dp = new SCDrawPanel();
		m_dp->Create(IDD_DIALOG_SC_DRAW_PANEL);
		m_dp->UpdateBaseImage(screenSCDC);
		m_dp->SetState(SCREEN_CAPTURE_STATE_START);
		m_dp->AddListener(this);

		m_dp->SetWindowPos(&wndTopMost, 0, 0, screenSCDC.GetWidth(), screenSCDC.GetHeight(), SWP_SHOWWINDOW);
		m_dp->ShowWindow(SW_SHOW);

		//m_dpArray.Add(m_dp);
		
	}

	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CScreenCaptureDlg::GetScreenCDCMem(CDC * pCDC, CBitmap *pBmp)
{
	if (pCDC == NULL || pBmp == NULL) {
		return FALSE;
	}

	// create screen cdc
	CDC screenCDC;
	screenCDC.CreateDCW(_T("DISPLAY"), NULL, NULL, NULL);   // 创建屏幕设备 dc

	BOOL bRet = CreateCompatibleCDC(&screenCDC, pCDC, pBmp);
	if (!bRet) {
		return FALSE;
	}

	int width  = screenCDC.GetDeviceCaps(HORZRES);
	int height = screenCDC.GetDeviceCaps(VERTRES);
	pCDC->BitBlt(0, 0, width, height, &screenCDC, 0, 0, SRCCOPY);

	return TRUE;
}

BOOL CScreenCaptureDlg::GetScreenCaptureBitmap(CBitmap *pSelectedBitmap)
{
	if (pSelectedBitmap == NULL) {
		return FALSE;
	}

	CRect cRect;
	
	//CDC *pDlgCDC = &m_userDrawCDC;
	CDC *pDlgCDC = NULL;

	CDC pSelectedCDC;
	pSelectedCDC.CreateCompatibleDC(pDlgCDC);

	pSelectedBitmap->DeleteObject();
	pSelectedBitmap->CreateCompatibleBitmap(pDlgCDC, cRect.Width(), cRect.Height());
	pSelectedCDC.SelectObject(pSelectedBitmap);

	pSelectedCDC.StretchBlt(0,          0,         cRect.Width(), cRect.Height(), pDlgCDC,
							cRect.left, cRect.top, cRect.Width(), cRect.Height(), SRCCOPY);

	pSelectedCDC.DeleteDC();
	
	return TRUE;
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
	//Reset();

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

//	Reset();
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
//	Reset();

	return 0;
}


afx_msg LRESULT CScreenCaptureDlg::OnScreenCaptureDrawRect(WPARAM wParam, LPARAM lParam)
{
	if (wParam) {
		//m_dp->SetEditMode(SELECT_AREA_EDIT_DRAW_RECT);
		m_toolBarSet.SetFontSize(m_rectSize);
		m_toolBarSet.ShowWindow(SW_SHOW);
	} else {
		m_toolBarSet.ShowWindow(SW_HIDE);
		//m_dp->SetEditMode(SELECT_AREA_EDIT_NONE);
	}

	return 0;
}

afx_msg LRESULT CScreenCaptureDlg::OnScreenCaptureDrawCircel(WPARAM wParam, LPARAM lParam)
{
	if (wParam) {
		m_selectAreaEditMode = SELECT_AREA_EDIT_DRAW_CIRCLE;
		m_toolBarSet.SetFontSize(m_circleSize);
		m_toolBarSet.ShowWindow(SW_SHOW);
	} else {
		m_selectAreaEditMode = SELECT_AREA_EDIT_NONE;
		m_toolBarSet.ShowWindow(SW_HIDE);
	}

	return 0;
}


afx_msg LRESULT CScreenCaptureDlg::OnScreenCaptureDrawArrow(WPARAM wParam, LPARAM lParam)
{
	if (wParam) {
		m_selectAreaEditMode = SELECT_AREA_EDIT_DRAW_ARROW;
		m_toolBarSet.SetFontSize(m_arrowSize);
		m_toolBarSet.ShowWindow(SW_SHOW);
	} else {
		m_selectAreaEditMode = SELECT_AREA_EDIT_NONE;
		m_toolBarSet.ShowWindow(SW_HIDE);
	}

	return 0;
}


afx_msg LRESULT CScreenCaptureDlg::OnScreenCaptureText(WPARAM wParam, LPARAM lParam)
{
	if (wParam) {
		m_selectAreaEditMode = SELECT_AREA_EDIT_DRAW_TEXT;

		m_toolBarSet.SetFontSize(m_fontSize);
		m_toolBarSet.ShowWindow(SW_SHOW);
	} else {
		m_selectAreaEditMode = SELECT_AREA_EDIT_NONE;
		m_toolBarSet.ShowWindow(SW_HIDE);
	}

	return 0;
}

afx_msg LRESULT CScreenCaptureDlg::OnScreenCaptureUndo(WPARAM wParam, LPARAM lParam)
{
	//if (m_userDrawArray.GetSize() <= 0) {
	//	return 0;
	//}

	//m_userDrawArray.RemoveAt(m_userDrawArray.GetSize() - 1);

	//Invalidate(TRUE);
	//UpdateWindow();

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
		// set to draw panel
		break;
	case SELECT_AREA_EDIT_DRAW_TEXT:
		m_fontSize = size;
		break;
	default:
		break;
	}

	return 0;
}



BOOL CScreenCaptureDlg::DrawUserDrawArrow(CDC * pCDC, CUserDraw * pUserDraw)
{
	SCArrowTool at;

	at.SetColor(pUserDraw->m_color);
	at.SetSize(pUserDraw->m_size);
	at.SetRect(pUserDraw->m_startPoint, pUserDraw->m_endPoint);

	return at.Draw(*pCDC);
}

BOOL CScreenCaptureDlg::DrawUserDrawCircle(CDC * pCDC, CUserDraw * pUserDraw)
{
	SCCircleTool ct;

	ct.SetColor(pUserDraw->m_color);
	ct.SetSize(pUserDraw->m_shape);
	ct.SetRect(pUserDraw->m_startPoint, pUserDraw->m_endPoint);

	return ct.Draw(*pCDC);
}

BOOL CScreenCaptureDlg::DrawUserDrawRect(CDC * pCDC, CUserDraw * pUserDraw)
{
	SCRectangleTool rt;
	rt.SetColor(pUserDraw->m_color);
	rt.SetSize(pUserDraw->m_shape);
	rt.SetRect(pUserDraw->m_startPoint, pUserDraw->m_endPoint);

	return rt.Draw(*pCDC);
}

BOOL CScreenCaptureDlg::DrawUserDrawText(CDC * pCDC, CUserDraw * pUserDraw)
{
// 	Graphics g(pCDC->GetSafeHdc());
// 	g.SetSmoothingMode(SmoothingModeAntiAlias);
// 
// 	FontFamily gFontFamily(DEFAULT_FONT);
// 	Gdiplus::Font gTextFont(&gFontFamily, pUserDraw->m_size, FontStyleRegular, UnitPoint);
// 	PointF gPoint(pUserDraw->m_startPoint.x, pUserDraw->m_startPoint.x);
// 	Color gColor(GetRValue(pUserDraw->m_color), GetGValue(pUserDraw->m_color), GetBValue(pUserDraw->m_color));
// 	g.DrawString(pUserDraw->m_str, pUserDraw->m_str.GetLength(), &gTextFont, gPoint, &SolidBrush(gColor));

	// set font size and color
	SCTextTool tt;

	tt.SetColor(pUserDraw->m_color);
	tt.SetPoint(pUserDraw->m_startPoint);
	tt.SetSize(pUserDraw->m_size);
	tt.SetString(pUserDraw->m_str);

	tt.Draw(*pCDC);

	return TRUE;
}

void CScreenCaptureDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	//CDC *pCDC = &m_userDrawCDC;//GetWindowDC();
	CDC *pCDC = NULL;
	pCDC->SetBkMode(TRANSPARENT);

	TEXTMETRIC tm;
	pCDC->GetTextMetrics(&tm);
	/*if (0x0d == nChar) {
		m_strLine.Empty();
		m_ptOrigin.y += tm.tmHeight;
	} else if ( 0x08 == nChar) {
		COLORREF clr = pCDC->SetTextColor(pCDC->GetBkColor());
		pCDC->TextOut(m_ptOrigin.x, m_ptOrigin.y, m_strLine);
		m_strLine = m_strLine.Left(m_strLine.GetLength() - 1);
		pCDC->SetTextColor(clr);
	} else {
		m_strLine += (TCHAR)nChar;
	}

	CSize sz = pCDC->GetTextExtent(m_strLine);
	CPoint pt;
	pt.x = m_ptOrigin.x + sz.cx;
	pt.y = m_ptOrigin.y;

	SetCaretPos(pt);

	if (m_userDrawArray.GetSize() > 0) {
		CUserDraw &userDraw = m_userDrawArray.ElementAt(m_userDrawArray.GetSize() - 1);
		userDraw.m_str.SetString(m_strLine);

		CFont font;
		font.CreatePointFont(userDraw.m_size * FONT_ADJUST, DEFAULT_FONT);

		CFont *oldFont = pCDC->SelectObject(&font);
		COLORREF oldColor = pCDC->SetTextColor(userDraw.m_color);
		//pCDC->TextOut(m_ptOrigin.x, m_ptOrigin.y, m_strLine);

		font.DeleteObject();
		pCDC->SetTextColor(oldColor);
		pCDC->SelectObject(oldFont);
	} else {
		//pCDC->TextOut(m_ptOrigin.x, m_ptOrigin.y, m_strLine);
	}

	Invalidate();*/

	CDialogEx::OnChar(nChar, nRepCnt, nFlags);
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

void CScreenCaptureDlg::SelectedAreaChangeEvent(const SCDrawPanel *scDrawPanel, CRect &rect)
{
	CRect toolBarRect;
	m_toolBar.GetWindowRect(toolBarRect);

	CRect windowRect;
	scDrawPanel->GetWindowRect(windowRect);

	CPoint toolBarNewPoint;
	toolBarNewPoint.x = rect.right - toolBarRect.Width();
	toolBarNewPoint.y = rect.bottom + 5;

	if (toolBarNewPoint.x < windowRect.left) {
		toolBarNewPoint.x = windowRect.left;
	}

	if (toolBarNewPoint.y + toolBarRect.Height() > windowRect.bottom) {
		toolBarNewPoint.y = windowRect.bottom - toolBarRect.Height() - 10;
	}

	m_toolBar.SetWindowPos(&wndTopMost, toolBarNewPoint.x, toolBarNewPoint.y, toolBarRect.Width(), toolBarRect.Height(), SWP_SHOWWINDOW);
	m_toolBar.ShowWindow(SW_SHOW);
}
