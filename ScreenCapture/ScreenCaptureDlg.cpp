XAccessible

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
	, m_screenCaptureState(SCREEN_CAPTURE_STATE_NONE)
	, m_startLocation(0)
	, m_endLocation(0)
	, m_isMouseLButtonDown(FALSE)
	, m_resizeDirection(RESIZE_LOCATION_NONE)
	, m_selecteAreaLastPoint(0)
	, m_messageStr(_T(""))
	, m_selectAreaEditMode(0)
	, m_fontSize(12)
	, m_rectSize(2)
	, m_arrowSize(2)
	, m_circleSize(2)
{
	m_size = 1;
	m_color = RGB(0, 0, 0);
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
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
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

	if (m_screenCaptureState != SCREEN_CAPTURE_STATE_NONE) {
		PAINTSTRUCT ps;
		CDC *pCDC = BeginPaint(&ps);
		DrawScreenCaptureResult(pCDC);
		EndPaint(&ps);
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

		GetScreenCDCMem(&m_screenCDCMem, &m_screenBmpMem);

		// get screen size
		int width = GetSystemMetrics (SM_CXSCREEN);
		int height= GetSystemMetrics (SM_CYSCREEN);

		//GetSystemMetrics(SM_CXVIRTUALSCREEN);
		//GetSystemMetrics(SM_CYVIRTUALSCREEN);
		//GetSystemMetrics(SM_XVIRTUALSCREEN);
		//GetSystemMetrics(SM_YVIRTUALSCREEN);


		SetWindowPos(&wndTopMost, 0, 0, width, height, SWP_SHOWWINDOW);
		ShowWindow(SW_SHOW);

		// set screen capture state to 
		m_screenCaptureState = SCREEN_CAPTURE_STATE_START;
	}

	CDialogEx::OnHotKey(nHotKeyId, nKey1, nKey2);
}


void CScreenCaptureDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	BOOL bRet = FALSE;

	m_isMouseLButtonDown = TRUE;

	// handle user draw
	if (m_selectAreaEditMode != SELECT_AREA_EDIT_NONE) {
		bRet = CreateUserDraw(&point);
	}

	// hide tool bar and set the start point
	if (m_screenCaptureState == SCREEN_CAPTURE_STATE_START) {
		m_startLocation = point;
		m_toolBar.ShowWindow(SW_HIDE);
	}

	// set reseize direction, when mouse drag, this value will be used
	if (m_screenCaptureState == SCREEN_CAPTURE_STATE_RESIZE) {
		m_selecteAreaLastPoint = point;
		m_resizeDirection = GetReSizeDirection(&point);
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CScreenCaptureDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_selectAreaEditMode != SELECT_AREA_EDIT_NONE) {
		ModifyUserDrawEndPoint(&point);
	}

	if (m_screenCaptureState == SCREEN_CAPTURE_STATE_START && m_isMouseLButtonDown == TRUE) {
		// set program state
		m_screenCaptureState = SCREEN_CAPTURE_STATE_RESIZE;

		// show tool bar
		CRect toolBarRect;
		m_toolBar.GetWindowRect(&toolBarRect);
		CRect dlgRect;
		GetWindowRect(&dlgRect);

		CPoint toolBarPosition;
		toolBarPosition.SetPoint(point.x + 5, point.y + 5);
		if (toolBarPosition.x + toolBarRect.Width() > dlgRect.right) {
			toolBarPosition.x = dlgRect.right - toolBarRect.Width();
		}

		if (toolBarPosition.y + toolBarRect.Height() > dlgRect.bottom) {
			toolBarPosition.y = dlgRect.bottom - toolBarRect.Height();
		}

		m_toolBar.MoveWindow(toolBarPosition.x, toolBarPosition.y, toolBarRect.Width(), toolBarRect.Height());
		m_toolBar.ShowWindow(SW_SHOW);

		// set tool bar set location
		CRect toolBarSetRect;
		m_toolBarSet.GetWindowRect(&toolBarSetRect);

		CPoint toolBarSetPosition;
		toolBarSetPosition.SetPoint(toolBarPosition.x, toolBarPosition.y + toolBarRect.Height());
		if (toolBarSetPosition.x + toolBarRect.Width() > dlgRect.right) {
			toolBarSetPosition.x = dlgRect.right - toolBarSetRect.Width();
		}

		if (toolBarSetPosition.y + toolBarRect.Height() > dlgRect.bottom) {
			toolBarSetPosition.y = toolBarPosition.y - toolBarSetRect.Height();
		}

		//m_toolBarSet.MoveWindow(toolBarPosition.x, toolBarPosition.y, toolBarSetRect.Width(), toolBarSetRect.Height());
		m_toolBarSet.MoveWindow(toolBarSetPosition.x, toolBarSetPosition.y, toolBarSetRect.Width(), toolBarSetRect.Height());
	}

	if (m_screenCaptureState == SCREEN_CAPTURE_STATE_RESIZE) {
		m_selecteAreaLastPoint.SetPoint(-1, -1);
	}

	m_isMouseLButtonDown = FALSE;
	m_resizeDirection = RESIZE_LOCATION_NONE;

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CScreenCaptureDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	Reset();
	// reflash window
	Invalidate(TRUE);
	UpdateWindow();

	m_screenCaptureState = SCREEN_CAPTURE_STATE_START;

	CDialogEx::OnRButtonDown(nFlags, point);
}


void CScreenCaptureDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	CDialogEx::OnRButtonUp(nFlags, point);
}

// over ride and fix flicker issue
BOOL CScreenCaptureDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;

	// return CDialogEx::OnEraseBkgnd(pDC);
}

void CScreenCaptureDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// mouse is not parssed, when mouse get into resize area, change mouse icon
	
	if (!m_isMouseLButtonDown) {

		// set mouse icon
		UpdateCursorIcon(&point);

		return;
	}

	if (m_selectAreaEditMode == SELECT_AREA_EDIT_NONE) {

		// handle selecte area edit
		if (m_screenCaptureState == SCREEN_CAPTURE_STATE_START) {
			m_endLocation = point;
		} else if (m_screenCaptureState == SCREEN_CAPTURE_STATE_RESIZE) {
			// update selected area
			UpdateSelectedArea(m_resizeDirection, &point);
		}
		
	} else {
		// handle user draw action
		ModifyUserDrawEndPoint(&point);
	}

	// reflash window
	Invalidate(TRUE);
	UpdateWindow();

	CDialogEx::OnMouseMove(nFlags, point);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////

void CScreenCaptureDlg::UpdateSelectedArea(int direction, CPoint * pPoint)
{
	switch (direction)
	{
	case RESIZE_LOCATION_WEST:
		m_startLocation.x = pPoint->x;
		break;

	case RESIZE_LOCATION_EAST: 
		m_endLocation.x = pPoint->x;
		break;

	case RESIZE_LOCATION_NORTH:
		m_startLocation.y = pPoint->y;
		break;

	case RESIZE_LOCATION_SOUTH:
		m_endLocation.y = pPoint->y;
		break;

	case RESIZE_LOCATION_NORTHEAST:
		m_endLocation.x = pPoint->x;
		m_startLocation.y = pPoint->y;
		break;

	case RESIZE_LOCATION_SOUTHWEST:
		m_startLocation.x = pPoint->x;
		m_endLocation.y = pPoint->y;
		break;
	case RESIZE_LOCATION_NORTHWEST:
		m_startLocation.SetPoint(pPoint->x, pPoint->y);
		break;

	case RESIZE_LOCATION_SOUTHEAST:
		m_endLocation.SetPoint(pPoint->x, pPoint->y);
		break;
	case RESIZE_LOCATION_AREA:
		UpdateSelecteAreaLocation(pPoint, direction);
		break;
	default:

		break;
	}
}


void CScreenCaptureDlg::UpdateSelecteAreaLocation(CPoint * pPoint, int direction)
{
	INT mX = 0;
	INT mY = 0;

	if (m_selecteAreaLastPoint.x < 0 || m_selecteAreaLastPoint.y < 0) {
		return;
	}

	// get dialog size
	CRect dlgRect;
	GetClientRect(&dlgRect);

	mX = pPoint->x - m_selecteAreaLastPoint.x;
	mY = pPoint->y - m_selecteAreaLastPoint.y;

	// out of west area
	if (m_startLocation.x + mX < 0) {
		mX = -m_startLocation.x;
	}

	// out of east
	if (m_endLocation.x + mX > dlgRect.right) {
		mX = dlgRect.right - m_endLocation.x;
	}

	// out of north
	if (m_startLocation.y + mY < 0) {
		mY = -m_startLocation.y;
	}

	// out of south
	if (m_endLocation.y + mY > dlgRect.bottom) {
		mY = dlgRect.bottom - m_endLocation.y;
	}

	m_startLocation.x += mX;
	m_startLocation.y += mY;

	m_endLocation.x += mX;
	m_endLocation.y += mY;

	m_selecteAreaLastPoint.SetPoint(pPoint->x, pPoint->y);

}


BOOL CScreenCaptureDlg::GetEditModeCursorIcon(CPoint *pPoint, LONG *cursorIcon)
{
	CRect selectedArea;
	BOOL bRet = GetSelectedAreaRect(&selectedArea);

	// mouse is not in selecte area
	if (!IsPointInRect(pPoint, &selectedArea)) {
		*cursorIcon = (LONG)LoadCursor(NULL, IDC_ARROW);
		return TRUE;
	}

	switch(m_selectAreaEditMode) {

	case SELECT_AREA_EDIT_DRAW_RECT:
	case SELECT_AREA_EDIT_DRAW_CIRCLE:
		*cursorIcon = (LONG)LoadCursor(NULL, IDC_CROSS);
		break;

	case SELECT_AREA_EDIT_DRAW_TEXT:
		*cursorIcon = (LONG)LoadCursor(NULL, IDC_IBEAM);
		break;

	default:
		*cursorIcon = (LONG)LoadCursor(NULL, IDC_ARROW);
	}

	return TRUE;
}

BOOL CScreenCaptureDlg::GetSelecteModeCursorIcon(CPoint *pPoint, LONG *cursorIcon)
{
	// selected area is not in edit
	int direction = GetReSizeDirection(pPoint);
	switch (direction)
	{
	case RESIZE_LOCATION_WEST:
	case RESIZE_LOCATION_EAST:
		*cursorIcon = (LONG)LoadCursor(NULL, IDC_SIZEWE);
		break;

	case RESIZE_LOCATION_NORTH:
	case RESIZE_LOCATION_SOUTH:
		*cursorIcon = (LONG)LoadCursor(NULL, IDC_SIZENS);
		break;

	case RESIZE_LOCATION_NORTHEAST:
	case RESIZE_LOCATION_SOUTHWEST:
		*cursorIcon = (LONG)LoadCursor(NULL, IDC_SIZENESW);
		break;

	case RESIZE_LOCATION_NORTHWEST:
	case RESIZE_LOCATION_SOUTHEAST:
		*cursorIcon = (LONG)LoadCursor(NULL, IDC_SIZENWSE);
		break;

	case RESIZE_LOCATION_AREA:
		*cursorIcon = (LONG)LoadCursor(NULL, IDC_SIZEALL);
		break;

	default:
		*cursorIcon = (LONG)LoadCursor(NULL, IDC_ARROW);
		break;
	}

	return TRUE;
}

void CScreenCaptureDlg::UpdateCursorIcon(CPoint *pPoint)
{
	LONG cursorIcon = (LONG)LoadCursor(NULL, IDC_ARROW);

	// selected area is in edit
	if (m_selectAreaEditMode != SELECT_AREA_EDIT_NONE) {
		GetEditModeCursorIcon(pPoint, &cursorIcon);
	} else {
		GetSelecteModeCursorIcon(pPoint, &cursorIcon);
	}

	SetClassLong(this->GetSafeHwnd(), GCL_HCURSOR, cursorIcon);
}

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


BOOL CScreenCaptureDlg::DrawScreenCaptureResult(CDC * pCDC)
{
	BOOL bRet = FALSE;

	// 1. draw user draw
	// 1.1 create user draw cdc based on screen cdc
	bRet = CopyCompatibleCDC(&m_screenCDCMem, &m_userDrawCDC, &m_userDrawBmp);

	// 1.2 draw user darw
	bRet = DrawUserDraw(&m_userDrawCDC);

	// 2. copy user draw to wnndow cdc
	CDC wndCDCMem;
	CBitmap wndBmpMem;
	bRet = CopyCompatibleCDC(&m_userDrawCDC, &wndCDCMem, &wndBmpMem);

	// 3. draw rect on window cdc
	int cx = pCDC->GetDeviceCaps(HORZRES);
	int cy = pCDC->GetDeviceCaps(VERTRES);

	if (m_startLocation.x >= 0 && m_startLocation.y >= 0 && m_endLocation.x >= 0 && m_endLocation.y >= 0) {
		bRet = DrawMette(&wndCDCMem);
		bRet = DrawSelectedArea(&wndCDCMem);
	} else {
		bRet = DrawMetteRect(&wndCDCMem, 0, 0, cx, cy);
	}

	// 4. draw information
	// bRet = DrawInformation(&wndCDCMem);

	// 5. copy window dc to window dc
	bRet = pCDC->BitBlt(0, 0, cx, cy, &wndCDCMem, 0, 0, SRCCOPY);
	if (!bRet) {
		MessageBox(_T("cDC->BitBlt fail"), _T("message"), MB_OK);
	}

	wndBmpMem.DeleteObject();
	wndCDCMem.DeleteDC();

	return TRUE;
}


BOOL CScreenCaptureDlg::DrawInformation(CDC * pCDC)
{
	CRect cRect;
	BOOL bRet = GetSelectedAreaRect(&cRect);

	CString message;
	message.Format(_T("selectedAreaRect:[%d, %d, %d, %d]"), cRect.top, cRect.right, cRect.bottom, cRect.left);
	pCDC->TextOutW(100, 50, message);

	message.Format(_T("m_selecteAreaLastPoint:[%d, %d]"), m_selecteAreaLastPoint.x, m_selecteAreaLastPoint.y);
	pCDC->TextOutW(100, 65, message);

	message.Format(_T("m_screenCaptureState:[%d]"), m_screenCaptureState);
	pCDC->TextOutW(100, 80, message);

	message.Format(_T("m_isMouseLButtonDown:[%d]"), m_isMouseLButtonDown);
	pCDC->TextOutW(100, 95, message);

	message.Format(_T("m_resizeDirection:[%d]"), m_resizeDirection);
	pCDC->TextOutW(100, 110, message);

	message.Format(_T("m_startLocation:[%d, %d]"), m_startLocation.x, m_startLocation.y);
	pCDC->TextOutW(100, 125, message);

	message.Format(_T("m_endLocation:[%d, %d]"), m_endLocation.x, m_endLocation.y);
	pCDC->TextOutW(100, 140, message);

	message.Format(_T("m_selectAreaEditMode:[%d], m_userDrawArraySize[%d]"), m_selectAreaEditMode, m_userDrawArray.GetSize());
	pCDC->TextOutW(100, 155, message);


	pCDC->TextOutW(100, 170, m_messageStr);

	return 0;
}


BOOL CScreenCaptureDlg::DrawSelectedArea(CDC * pCDC)
{
	BOOL bRet = TRUE;

	// get image dc size
	int width  = pCDC->GetDeviceCaps(HORZRES);
	int height = pCDC->GetDeviceCaps(VERTRES);

	// set pen
	CPen boarderPen;
	COLORREF boarderColor = RGB(100, 100, 255);
	boarderPen.CreatePen(PS_SOLID, 1, boarderColor);
	CPen *oldPen = pCDC->SelectObject(&boarderPen);

	// draw boarder
	pCDC->MoveTo(m_startLocation);
	pCDC->LineTo(m_endLocation.x, m_startLocation.y);

	pCDC->LineTo(m_endLocation);
	pCDC->LineTo(m_startLocation.x, m_endLocation.y);
	bRet = pCDC->LineTo(m_startLocation);

	// draw adjust tool
	CRect cRectArray[ADJUST_CRECT_ARRAY_LEN];
	bRet = GetAdjustToolRectList(cRectArray, ADJUST_CRECT_ARRAY_LEN);

	LOGBRUSH brush;
	brush.lbColor = boarderColor;
	brush.lbStyle = BS_SOLID;
	brush.lbHatch = HS_CROSS;
	CBrush boarderBrush;
	bRet = boarderBrush.CreateBrushIndirect(&brush);
	for (int i = 0; i < ADJUST_CRECT_ARRAY_LEN; i++)
	{
		pCDC->FillRect(cRectArray[i], &boarderBrush);
	}

	// 
	pCDC->SelectObject(oldPen);
	boarderPen.DeleteObject();

	return TRUE;
}

BOOL CScreenCaptureDlg::GetAdjustToolRectList(CRect * pCrect, int size)
{
	if (size < ADJUST_CRECT_ARRAY_LEN) {
		return FALSE;
	}

	int offset = 2;
	CRect cRect;
	BOOL bRet = GetSelectedAreaRect(&cRect);

	pCrect[0].SetRect(cRect.left - offset, cRect.top - offset, cRect.left + offset, cRect.top + offset);
	pCrect[1].SetRect(cRect.right - offset, cRect.top - offset, cRect.right + offset, cRect.top + offset);
	pCrect[2].SetRect(cRect.right - offset, cRect.bottom - offset, cRect.right + offset, cRect.bottom + offset);
	pCrect[3].SetRect(cRect.left - offset, cRect.bottom - offset, cRect.left + offset, cRect.bottom + offset);

	int xM = (cRect.right + cRect.left) / 2;
	int yM = (cRect.bottom + cRect.top) / 2;
	pCrect[4].SetRect(xM - offset, cRect.top - offset, xM + offset, cRect.top + offset);
	pCrect[5].SetRect(cRect.right - offset, yM - offset, cRect.right + offset, yM + offset);
	pCrect[6].SetRect(xM - offset, cRect.bottom - offset, xM + offset, cRect.bottom + offset);
	pCrect[7].SetRect(cRect.left - offset, yM - offset, cRect.left + offset, yM + offset);

	return TRUE;
}


BOOL CScreenCaptureDlg::DrawMette(CDC *pCDC)
{
	BOOL bRet = FALSE;

	CRect cRect;
	bRet = GetSelectedAreaRect(&cRect);

	// get dc size
	int dcWidth  = pCDC->GetDeviceCaps(HORZRES);
	int dcHeight = pCDC->GetDeviceCaps(VERTRES);

	// draw top mette
	bRet = DrawMetteRect(pCDC, 0, 0, dcWidth, cRect.top);
	if (!bRet) {
		MessageBox(_T("draw top mette fail"), _T("message"), MB_OK);
		return bRet;
	}

	// draw right mette
	bRet = DrawMetteRect(pCDC, cRect.right, cRect.top, dcWidth - cRect.right, cRect.bottom - cRect.top);
	if (!bRet) {
		MessageBox(_T("draw right mette fail"), _T("message"), MB_OK);
		return bRet;
	}

	// draw bottom mette
	bRet = DrawMetteRect(pCDC, 0, cRect.bottom, dcWidth, dcHeight - cRect.bottom);
	if (!bRet) {
		MessageBox(_T("draw bottom mette fail"), _T("message"), MB_OK);
		return bRet;
	}

	// draw left mette
	bRet = DrawMetteRect(pCDC, 0, cRect.top, cRect.left, cRect.bottom - cRect.top);

	return bRet;
}

BOOL CScreenCaptureDlg::DrawMetteRect(CDC *pCDC, int startX, int startY, int width, int height)
{
	BOOL bRet = FALSE;

	// mette hdc
	CDC metteCDC;
	CBitmap metteBmp;

	bRet = CreateCompatibleCDC(pCDC, &metteCDC, &metteBmp);

	COLORREF colorREF = RGB(0, 0, 0);
	metteCDC.FillSolidRect(0, 0, width, height, colorREF);

	// 
	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = 0;
	bf.SourceConstantAlpha = 90;
	bRet = AlphaBlend(pCDC->GetSafeHdc(), startX, startY, width, height, metteCDC, 0, 0, width, height, bf);

	metteBmp.DeleteObject();
	metteCDC.DeleteDC();

	return bRet;
}


void CScreenCaptureDlg::Reset(void)
{
	m_isMouseLButtonDown = FALSE;
	m_screenCaptureState = SCREEN_CAPTURE_STATE_NONE;
	m_resizeDirection = RESIZE_LOCATION_NONE;
	m_selectAreaEditMode = SELECT_AREA_EDIT_NONE;

	m_startLocation.SetPoint(-1, -1);
	m_endLocation.SetPoint(-1, -1);
	m_selecteAreaLastPoint.SetPoint(-1, -1);

	m_toolBar.ShowWindow(SW_HIDE);
	m_toolBarSet.ShowWindow(SW_HIDE);

	m_userDrawArray.RemoveAll();

}


BOOL CScreenCaptureDlg::GetScreenCaptureBitmap(CBitmap *pSelectedBitmap)
{
	if (pSelectedBitmap == NULL) {
		return FALSE;
	}

	CRect cRect;
	BOOL bRet = GetSelectedAreaRect(&cRect);
	
	CDC *pDlgCDC = &m_userDrawCDC;
	
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


BOOL CScreenCaptureDlg::GetSelectedAreaRect(CRect * pCrect)
{
	if (pCrect == NULL) {
		return FALSE;
	}

	int selectedAreaStartX = m_startLocation.x < m_endLocation.x ? m_startLocation.x : m_endLocation.x;
	int selectedAreaStartY = m_startLocation.y < m_endLocation.y ? m_startLocation.y : m_endLocation.y;

	int selectedAreaEndX = m_startLocation.x > m_endLocation.x ? m_startLocation.x : m_endLocation.x;
	int selectedAreaEndY = m_startLocation.y > m_endLocation.y ? m_startLocation.y : m_endLocation.y;

	pCrect->SetRect(selectedAreaStartX, selectedAreaStartY, selectedAreaEndX, selectedAreaEndY);

	return TRUE;
}


int CScreenCaptureDlg::GetReSizeDirection(CPoint * point)
{
	if (point == NULL) {
		return RESIZE_LOCATION_NONE;
	}

	CRect cRectArray[ADJUST_CRECT_ARRAY_LEN];
	BOOL bRet = GetAdjustToolRectList(cRectArray, ADJUST_CRECT_ARRAY_LEN);

	// check reset area
	for (int i = 0; i < ADJUST_CRECT_ARRAY_LEN; i++) {
		if (IsPointInRect(point, &cRectArray[i])) {

				if (i == 0) {
					return RESIZE_LOCATION_NORTHWEST;
				} else if (i == 1) {
					return RESIZE_LOCATION_NORTHEAST;
				} else if (i == 2) {
					return RESIZE_LOCATION_SOUTHEAST;
				} else if (i == 3) {
					return RESIZE_LOCATION_SOUTHWEST;
				} else if (i == 4) {
					return RESIZE_LOCATION_NORTH;
				} else if (i == 5) {
					return RESIZE_LOCATION_EAST;
				} else if (i == 6) {
					return RESIZE_LOCATION_SOUTH;
				} else if (i == 7) {
					return RESIZE_LOCATION_WEST;
				}
		}
	}

	// check selecte area
	CRect selectedArea;
	bRet = GetSelectedAreaRect(&selectedArea);

	if (point->x > selectedArea.left && point->x < selectedArea.right
		&& point->y > selectedArea.top && point->y < selectedArea.bottom) {
			return RESIZE_LOCATION_AREA;
	}

	return RESIZE_LOCATION_NONE;
}


BOOL CScreenCaptureDlg::IsPointInRect(CPoint * pPoint, CRect * pCRect)
{
	if (pPoint->x < pCRect->left) {
		return FALSE;
	}

	if (pPoint->x > pCRect->right) {
		return FALSE;
	}

	if (pPoint->y < pCRect->top) {
		return FALSE;
	}

	if (pPoint->y > pCRect->bottom) {
		return FALSE;
	}

	return TRUE;
}


void CScreenCaptureDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (m_screenCaptureState == SCREEN_CAPTURE_STATE_RESIZE) {

		// double click in selected area, means ok
		CRect cRect;
		BOOL bRet = GetSelectedAreaRect(&cRect);
		if (IsPointInRect(&point, &cRect) == TRUE) {
			PostMessage(SCREEN_CAPTURE_OK, 0, 0);
		}
	}

	CDialogEx::OnLButtonDblClk(nFlags, point);
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
	if (wParam) {
		m_selectAreaEditMode = SELECT_AREA_EDIT_DRAW_RECT;
		m_toolBarSet.SetFontSize(m_rectSize);
		m_toolBarSet.ShowWindow(SW_SHOW);
	} else {
		m_toolBarSet.ShowWindow(SW_HIDE);
		m_selectAreaEditMode = SELECT_AREA_EDIT_NONE;
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

BOOL CScreenCaptureDlg::CreateCompatibleCDC(CDC *pSrcCDC, CDC *pTagetCDC, CBitmap *pTargetBmp)
{
	BOOL bRet = FALSE;

	if (pSrcCDC == NULL || pTargetBmp == NULL || pTagetCDC == NULL) {
		return FALSE;
	}

	// get src cdc size
	int cx = pSrcCDC->GetDeviceCaps(HORZRES);
	int cy = pSrcCDC->GetDeviceCaps(VERTRES);

	// create window dc memory copy
	pTagetCDC->DeleteDC();
	bRet = pTagetCDC->CreateCompatibleDC(pSrcCDC);
	if (!bRet) {
		MessageBox(_T("wndCDCMem.CreateCompatibleDC fail"), _T("message"), MB_OK);
		return FALSE;
	}

	pTargetBmp->DeleteObject();
	bRet = pTargetBmp->CreateCompatibleBitmap(pSrcCDC, cx, cy);
	if (!bRet) {
		MessageBox(_T("wndBitmap.CreateCompatibleBitmap fail"), _T("message"), MB_OK);
		return FALSE;
	}

	pTagetCDC->SelectObject(pTargetBmp);

	return TRUE;
}

BOOL CScreenCaptureDlg::CopyCompatibleCDC(CDC *pSrcCDC, CDC *pTagetCDC, CBitmap *pTargetBmp)
{
	BOOL bRet = FALSE;

	if (pSrcCDC == NULL || pTargetBmp == NULL || pTagetCDC == NULL) {
		return FALSE;
	}

	// get src cdc size
	int cx = pSrcCDC->GetDeviceCaps(HORZRES);
	int cy = pSrcCDC->GetDeviceCaps(VERTRES);

	bRet = CreateCompatibleCDC(pSrcCDC, pTagetCDC, pTargetBmp);

	// copy src image to buffer dc
	bRet = pTagetCDC->BitBlt(0, 0, cx, cy, pSrcCDC, 0, 0, SRCCOPY);
	if (!bRet) {
		MessageBox(_T("wndCDCMem.BitBlt fail"), _T("message"), MB_OK);
		return FALSE;
	}

	return TRUE;
}


BOOL CScreenCaptureDlg::CreateUserDraw(CPoint * pPoint)
{
	if (m_selectAreaEditMode == SELECT_AREA_EDIT_NONE) {
		return FALSE;
	}

	// create a CUserDraw and add it to the array
	CUserDraw userDraw;
	userDraw.m_startPoint.SetPoint(pPoint->x, pPoint->y);
	userDraw.m_endPoint.SetPoint(pPoint->x, pPoint->y);
	userDraw.m_color = m_color;

	switch (m_selectAreaEditMode)
	{
	case SELECT_AREA_EDIT_DRAW_RECT:
		userDraw.m_shape = USER_DRAW_SHAPE_RECT;
		userDraw.m_size = m_rectSize;
		break;
	case SELECT_AREA_EDIT_DRAW_CIRCLE:
		userDraw.m_shape = USER_DRAW_SHAPE_CIRCLE;
		userDraw.m_size = m_circleSize;
		break;
	case SELECT_AREA_EDIT_DRAW_ARROW:
		userDraw.m_shape = USER_DRAW_SHAPE_ARROW;
		userDraw.m_size = m_arrowSize;
		break;
	case SELECT_AREA_EDIT_DRAW_TEXT:
		userDraw.m_shape = USER_DRAW_SHAPE_TEXT;
		userDraw.m_size = m_fontSize;

		// save point
		m_ptOrigin.x = pPoint->x;
		m_ptOrigin.y = pPoint->y;

		// reset string
		m_strLine = _T("");

		DestroyCaret();
		CreateSolidCaret(2, 20);		
		SetCaretPos(m_ptOrigin);
		ShowCaret();
		break;
	default:
		break;
	}

	m_userDrawArray.Add(userDraw);

	return TRUE;
}


BOOL CScreenCaptureDlg::ModifyUserDrawEndPoint(CPoint * pPoint)
{
	if (m_userDrawArray.GetSize() <= 0) {
		return TRUE;
	}

	CUserDraw &userDraw = m_userDrawArray.ElementAt(m_userDrawArray.GetSize() - 1);
	userDraw.m_endPoint.SetPoint(pPoint->x, pPoint->y);

	return TRUE;
}


BOOL CScreenCaptureDlg::DrawUserDraw(CDC * pCDC)
{
	if (m_userDrawArray.GetSize() <= 0) {
		return TRUE;
	}

	for (int i = 0; i < m_userDrawArray.GetSize(); i++)
	{
		CUserDraw userDraw = m_userDrawArray.GetAt(i);
		m_messageStr.Format(_T("userDraw:%d, %d"), userDraw.m_size, userDraw.m_color);
		Invalidate();
		switch(userDraw.m_shape) {
		case USER_DRAW_SHAPE_RECT:
			DrawUserDrawRect(pCDC, &userDraw);
			break;

		case USER_DRAW_SHAPE_CIRCLE:
			DrawUserDrawCircle(pCDC, &userDraw);
			break;

		case USER_DRAW_SHAPE_ARROW:

			DrawUserDrawArrow(pCDC, &userDraw);
			break;

		case USER_DRAW_SHAPE_TEXT:
			DrawUserDrawText(pCDC, &userDraw);
			break;

		default:
			break;
		}
	}

	return TRUE;
}

BOOL CScreenCaptureDlg::DrawUserDrawArrow(CDC * pCDC, CUserDraw * pUserDraw)
{
	Graphics g(pCDC->GetSafeHdc());
	g.SetSmoothingMode(SmoothingModeAntiAlias);

	Color gColor(GetRValue(pUserDraw->m_color), GetGValue(pUserDraw->m_color), GetBValue(pUserDraw->m_color));
	Pen gArrowPen(gColor, pUserDraw->m_size);
	g.DrawLine(&gArrowPen, pUserDraw->m_startPoint.x, pUserDraw->m_startPoint.y, pUserDraw->m_endPoint.x, pUserDraw->m_endPoint.y);

	// draw arrow
	int x1 = pUserDraw->m_endPoint.x - 10;
	if (pUserDraw->m_endPoint.x < pUserDraw->m_startPoint.x) {
		x1 = pUserDraw->m_endPoint.x + 10;
	}

	int y1 = pUserDraw->m_endPoint.y - 10;
	if (pUserDraw->m_endPoint.y < pUserDraw->m_startPoint.y) {
		y1 = pUserDraw->m_endPoint.y + 10;
	}
	
	return TRUE;
}

BOOL CScreenCaptureDlg::DrawUserDrawCircle(CDC * pCDC, CUserDraw * pUserDraw)
{
	Graphics g(pCDC->GetSafeHdc());
	g.SetSmoothingMode(SmoothingModeAntiAlias);

	Color gColor(GetRValue(pUserDraw->m_color), GetGValue(pUserDraw->m_color), GetBValue(pUserDraw->m_color));
	Pen gCirclePen(gColor, pUserDraw->m_size);
	CRect cRect(pUserDraw->m_startPoint, pUserDraw->m_endPoint);
	g.DrawEllipse(&gCirclePen, cRect.left, cRect.top, cRect.Width(), cRect.Height());

	return TRUE;
}

BOOL CScreenCaptureDlg::DrawUserDrawRect(CDC * pCDC, CUserDraw * pUserDraw)
{
	Graphics g(pCDC->GetSafeHdc());
	g.SetSmoothingMode(SmoothingModeAntiAlias);

	Color gColor(GetRValue(pUserDraw->m_color), GetGValue(pUserDraw->m_color), GetBValue(pUserDraw->m_color));
	Pen gRectPen(gColor, pUserDraw->m_size);
	CRect cRect(pUserDraw->m_startPoint, pUserDraw->m_endPoint);
	
	g.DrawRectangle(&gRectPen, cRect.left, cRect.top, cRect.Width(), cRect.Height());

	return TRUE;
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
	CFont font;
	font.CreatePointFont(pUserDraw->m_size * FONT_ADJUST, DEFAULT_FONT);

	CFont *oldFont = pCDC->SelectObject(&font);
	COLORREF oldColor = pCDC->SetTextColor(pUserDraw->m_color);
	pCDC->SetBkMode(TRANSPARENT);
	pCDC->TextOut(pUserDraw->m_startPoint.x, pUserDraw->m_startPoint.y, pUserDraw->m_str);

	// reset parameters
	pCDC->SetTextColor(oldColor);
	pCDC->SelectObject(oldFont);
	font.DeleteObject();

	return TRUE;
}

void CScreenCaptureDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CDC *pCDC = &m_userDrawCDC;//GetWindowDC();
	pCDC->SetBkMode(TRANSPARENT);

	TEXTMETRIC tm;
	pCDC->GetTextMetrics(&tm);
	if (0x0d == nChar) {
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

	Invalidate();

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
