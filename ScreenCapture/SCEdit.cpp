// SCedit.cpp : 实现文件
//

#include "stdafx.h"
#include "ScreenCapture.h"
#include "SCedit.h"
#include "afxdialogex.h"
#include <GdiPlus.h>
#include <Windows.h>

// SCedit 对话框

IMPLEMENT_DYNAMIC(SCEdit, CDialogEx)

SCEdit::SCEdit(HMONITOR hMonitor, CWnd* pParent /*=NULL*/)
	: CDialogEx(SCEdit::IDD, pParent)
	, m_screenCaptureState(SCREEN_CAPTURE_STATE_NONE)
	, m_startLocation(0)
	, m_endLocation(0)
	, m_resizeDirection(RESIZE_LOCATION_NONE)
	, m_selecteAreaLastPoint(0)
	, m_messageStr(_T(""))
	, m_selectAreaEditMode(0)
	, m_fontSize(12)
	, m_rectSize(2)
	, m_arrowSize(2)
	, m_circleSize(2)
	, m_isMouseLButtonDown(FALSE)
{
	m_size = 1;
	m_color = RGB(0, 0, 0);

	m_hMonitor = hMonitor;
	GetMonitorCDCMem(hMonitor, &m_screenCDCMem, &m_screenBmpMem);
}

SCEdit::~SCEdit()
{
}

void SCEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SCEdit, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CHAR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL SCEdit::CreateCompatibleCDC(CDC *pSrcCDC, CDC *pTagetCDC, CBitmap *pTargetBmp)
{
	BOOL bRet = FALSE;

	if (pSrcCDC == NULL || pTargetBmp == NULL || pTagetCDC == NULL) {
		return FALSE;
	}

	// get src cdc size
	int cx = pSrcCDC->GetDeviceCaps(HORZRES);
	int cy = pSrcCDC->GetDeviceCaps(VERTRES);
	TRACE("%s-%d:SM_CXSCREEN=%d, SM_CYSCREEN=%d\n", __FUNCTION__, __LINE__, cx, cy);

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

BOOL SCEdit::CopyCompatibleCDC(CDC *pSrcCDC, CDC *pTagetCDC, CBitmap *pTargetBmp)
{
	BOOL bRet = FALSE;

	if (pSrcCDC == NULL || pTargetBmp == NULL || pTagetCDC == NULL) {
		return FALSE;
	}

	// get src cdc size
	int cx = pSrcCDC->GetDeviceCaps(HORZRES);
	int cy = pSrcCDC->GetDeviceCaps(VERTRES);

	TRACE("%s-%d:cx=%d, cy=%d\n", __FUNCTION__, __LINE__, cx, cy);

	bRet = CreateCompatibleCDC(pSrcCDC, pTagetCDC, pTargetBmp);

	// copy src image to buffer dc
	bRet = pTagetCDC->BitBlt(0, 0, cx, cy, pSrcCDC, 0, 0, SRCCOPY);
	if (!bRet) {
		MessageBox(_T("wndCDCMem.BitBlt fail"), _T("message"), MB_OK);
		return FALSE;
	}

	return TRUE;
}

BOOL SCEdit::GetMonitorCDCMem(HMONITOR hMonitor, CDC * pCDC, CBitmap *pBmp)
{
	int ret = 0;

	if (pCDC == NULL || pBmp == NULL) {
		TRACE("%s-%d:param check fail\n", __FUNCTION__, __LINE__);
		return FALSE;
	}

	if (hMonitor == NULL) {
		TRACE("%s-%d:monitor infor check fail\n", __FUNCTION__, __LINE__);
		return FALSE;
	}

	MONITORINFOEX mis;
	memset(&mis, 0x00, sizeof(MONITORINFOEX));
	mis.cbSize = sizeof(MONITORINFOEX);

	ret = GetMonitorInfo(hMonitor, &mis);
	if (!ret) {
		TRACE("%s-%d:GetMonitorInfo fail\n", __FUNCTION__, __LINE__);
		return FALSE;
	}
	
	CDC monitorDc;
	ret = monitorDc.CreateDCW(CString(mis.szDevice), NULL, NULL, NULL);

	CDC *pDC = &monitorDc;
	BOOL bRet = CreateCompatibleCDC(pDC, pCDC, pBmp);
	if (!bRet) {
		TRACE("%s-%d:CreateCompatibleCDC fail\n", __FUNCTION__, __LINE__);
		return FALSE;
	}

	int width = pDC->GetDeviceCaps(HORZRES);
	int height = pDC->GetDeviceCaps(VERTRES);

	pCDC->BitBlt(0, 0, width, height, pDC, 0, 0, SRCCOPY);

	return TRUE;
}

// SCedit 消息处理程序


void SCEdit::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	//PAINTSTRUCT ps;
	//CDC *pCDC = BeginPaint(&ps);
	CDC *pCDC = &dc;

	if (m_screenCaptureState != SCREEN_CAPTURE_STATE_NONE) {
		PAINTSTRUCT ps;
		CDC *pCDC = BeginPaint(&ps);
		DrawScreenCaptureResult(pCDC);
		EndPaint(&ps);
	}

	// 3. draw rect on window cdc
	int cx = pCDC->GetDeviceCaps(HORZRES);
	int cy = pCDC->GetDeviceCaps(VERTRES);

	BOOL bRet = pCDC->BitBlt(0, 0, cx, cy, &m_screenCDCMem, 0, 0, SRCCOPY);
	if (!bRet) {
		MessageBox(_T("cDC->BitBlt fail"), _T("message"), MB_OK);
	}

	//EndPaint(&ps);
}

BOOL SCEdit::DrawScreenCaptureResult(CDC * pCDC)
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

	TRACE("%s-%d:cx=%d, cy=%d\n", __FUNCTION__, __LINE__, cx, cy);

	if (m_startLocation.x >= 0 && m_startLocation.y >= 0 && m_endLocation.x >= 0 && m_endLocation.y >= 0) {
		bRet = DrawMette(&wndCDCMem);
		bRet = DrawSelectedArea(&wndCDCMem);
	}
	else {
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

BOOL SCEdit::DrawSelectedArea(CDC * pCDC)
{
	BOOL bRet = TRUE;

	// get image dc size
	int width = pCDC->GetDeviceCaps(HORZRES);
	int height = pCDC->GetDeviceCaps(VERTRES);

	TRACE("%s-%d:SM_CXSCREEN=%d, SM_CYSCREEN=%d\n", __FUNCTION__, __LINE__, width, height);

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

BOOL SCEdit::CreateUserDraw(CPoint * pPoint)
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


BOOL SCEdit::ModifyUserDrawEndPoint(CPoint * pPoint)
{
	if (m_userDrawArray.GetSize() <= 0) {
		return TRUE;
	}

	CUserDraw &userDraw = m_userDrawArray.ElementAt(m_userDrawArray.GetSize() - 1);
	userDraw.m_endPoint.SetPoint(pPoint->x, pPoint->y);

	return TRUE;
}


BOOL SCEdit::DrawUserDraw(CDC * pCDC)
{
	if (m_userDrawArray.GetSize() <= 0) {
		return TRUE;
	}

	for (int i = 0; i < m_userDrawArray.GetSize(); i++)
	{
		CUserDraw userDraw = m_userDrawArray.GetAt(i);
		m_messageStr.Format(_T("userDraw:%d, %d"), userDraw.m_size, userDraw.m_color);
		Invalidate();
		switch (userDraw.m_shape) {
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

BOOL SCEdit::DrawUserDrawArrow(CDC * pCDC, CUserDraw * pUserDraw)
{
	Gdiplus::Graphics g(pCDC->GetSafeHdc());
	g.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

	Gdiplus::Color gColor(GetRValue(pUserDraw->m_color), GetGValue(pUserDraw->m_color), GetBValue(pUserDraw->m_color));
	Gdiplus::Pen gArrowPen(gColor, pUserDraw->m_size);
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

BOOL SCEdit::DrawUserDrawCircle(CDC * pCDC, CUserDraw * pUserDraw)
{
	Gdiplus::Graphics g(pCDC->GetSafeHdc());
	g.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

	Gdiplus::Color gColor(GetRValue(pUserDraw->m_color), GetGValue(pUserDraw->m_color), GetBValue(pUserDraw->m_color));
	Gdiplus::Pen gCirclePen(gColor, pUserDraw->m_size);
	CRect cRect(pUserDraw->m_startPoint, pUserDraw->m_endPoint);
	g.DrawEllipse(&gCirclePen, cRect.left, cRect.top, cRect.Width(), cRect.Height());

	return TRUE;
}

BOOL SCEdit::DrawUserDrawRect(CDC * pCDC, CUserDraw * pUserDraw)
{
	Gdiplus::Graphics g(pCDC->GetSafeHdc());
	g.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);

	Gdiplus::Color gColor(GetRValue(pUserDraw->m_color), GetGValue(pUserDraw->m_color), GetBValue(pUserDraw->m_color));
	Gdiplus::Pen gRectPen(gColor, pUserDraw->m_size);
	CRect cRect(pUserDraw->m_startPoint, pUserDraw->m_endPoint);

	g.DrawRectangle(&gRectPen, cRect.left, cRect.top, cRect.Width(), cRect.Height());

	return TRUE;
}

BOOL SCEdit::DrawUserDrawText(CDC * pCDC, CUserDraw * pUserDraw)
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

void SCEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CDC *pCDC = &m_userDrawCDC;//GetWindowDC();
	pCDC->SetBkMode(TRANSPARENT);

	TEXTMETRIC tm;
	pCDC->GetTextMetrics(&tm);
	if (0x0d == nChar) {
		m_strLine.Empty();
		m_ptOrigin.y += tm.tmHeight;
	}
	else if (0x08 == nChar) {
		COLORREF clr = pCDC->SetTextColor(pCDC->GetBkColor());
		pCDC->TextOut(m_ptOrigin.x, m_ptOrigin.y, m_strLine);
		m_strLine = m_strLine.Left(m_strLine.GetLength() - 1);
		pCDC->SetTextColor(clr);
	}
	else {
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
	}
	else {
		//pCDC->TextOut(m_ptOrigin.x, m_ptOrigin.y, m_strLine);
	}

	Invalidate();

	CDialogEx::OnChar(nChar, nRepCnt, nFlags);
}


BOOL SCEdit::DrawMette(CDC *pCDC)
{
	BOOL bRet = FALSE;

	CRect cRect;
	bRet = GetSelectedAreaRect(&cRect);

	// get dc size
	int dcWidth = pCDC->GetDeviceCaps(HORZRES);
	int dcHeight = pCDC->GetDeviceCaps(VERTRES);
	TRACE("%s-%d:SM_CXSCREEN=%d, SM_CYSCREEN=%d\n", __FUNCTION__, __LINE__, dcWidth, dcHeight);
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

BOOL SCEdit::DrawMetteRect(CDC *pCDC, int startX, int startY, int width, int height)
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


BOOL SCEdit::GetSelectedAreaRect(CRect * pCrect)
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


int SCEdit::GetReSizeDirection(CPoint * point)
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
			}
			else if (i == 1) {
				return RESIZE_LOCATION_NORTHEAST;
			}
			else if (i == 2) {
				return RESIZE_LOCATION_SOUTHEAST;
			}
			else if (i == 3) {
				return RESIZE_LOCATION_SOUTHWEST;
			}
			else if (i == 4) {
				return RESIZE_LOCATION_NORTH;
			}
			else if (i == 5) {
				return RESIZE_LOCATION_EAST;
			}
			else if (i == 6) {
				return RESIZE_LOCATION_SOUTH;
			}
			else if (i == 7) {
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


BOOL SCEdit::IsPointInRect(CPoint * pPoint, CRect * pCRect)
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

BOOL SCEdit::GetAdjustToolRectList(CRect * pCrect, int size)
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


void SCEdit::OnLButtonDown(UINT nFlags, CPoint point)
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


void SCEdit::OnLButtonUp(UINT nFlags, CPoint point)
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


void SCEdit::OnRButtonDown(UINT nFlags, CPoint point)
{
	Reset();
	// reflash window
	Invalidate(TRUE);
	UpdateWindow();

	m_screenCaptureState = SCREEN_CAPTURE_STATE_START;

	CDialogEx::OnRButtonDown(nFlags, point);
}


void SCEdit::OnRButtonUp(UINT nFlags, CPoint point)
{
	CDialogEx::OnRButtonUp(nFlags, point);
}

// over ride and fix flicker issue
BOOL SCEdit::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;

	// return CDialogEx::OnEraseBkgnd(pDC);
}

void SCEdit::OnMouseMove(UINT nFlags, CPoint point)
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
		}
		else if (m_screenCaptureState == SCREEN_CAPTURE_STATE_RESIZE) {
			// update selected area
			UpdateSelectedArea(m_resizeDirection, &point);
		}

	}
	else {
		// handle user draw action
		ModifyUserDrawEndPoint(&point);
	}

	// reflash window
	Invalidate(TRUE);
	UpdateWindow();

	CDialogEx::OnMouseMove(nFlags, point);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////

void SCEdit::UpdateSelectedArea(int direction, CPoint * pPoint)
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


void SCEdit::UpdateSelecteAreaLocation(CPoint * pPoint, int direction)
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


BOOL SCEdit::GetEditModeCursorIcon(CPoint *pPoint, LONG *cursorIcon)
{
	CRect selectedArea;
	BOOL bRet = GetSelectedAreaRect(&selectedArea);

	// mouse is not in selecte area
	if (!IsPointInRect(pPoint, &selectedArea)) {
		*cursorIcon = (LONG)LoadCursor(NULL, IDC_ARROW);
		return TRUE;
	}

	switch (m_selectAreaEditMode) {

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

BOOL SCEdit::GetSelecteModeCursorIcon(CPoint *pPoint, LONG *cursorIcon)
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

void SCEdit::UpdateCursorIcon(CPoint *pPoint)
{
	LONG cursorIcon = (LONG)LoadCursor(NULL, IDC_ARROW);

	// selected area is in edit
	if (m_selectAreaEditMode != SELECT_AREA_EDIT_NONE) {
		GetEditModeCursorIcon(pPoint, &cursorIcon);
	}
	else {
		GetSelecteModeCursorIcon(pPoint, &cursorIcon);
	}

	SetClassLong(this->GetSafeHwnd(), GCL_HCURSOR, cursorIcon);
}

BOOL SCEdit::GetScreenCDCMem(CDC * pCDC, CBitmap *pBmp)
{
	if (pCDC == NULL || pBmp == NULL) {
		return FALSE;
	}

	// create screen cdc
	CDC screenCDC;
	screenCDC.CreateDCW(_T("DISPLAY"), NULL, NULL, NULL);   // 创建屏幕设备 dc


	HWND dWnd = ::GetDesktopWindow();
	HDC dhdc = ::GetDC(dWnd);
	CDC *ddc = CDC::FromHandle(dhdc);

	CDC *pDC = ddc;// &screenCDC;
	BOOL bRet = CreateCompatibleCDC(pDC, pCDC, pBmp);
	if (!bRet) {
		return FALSE;
	}

	int width = pDC->GetDeviceCaps(HORZRES);
	int height = pDC->GetDeviceCaps(VERTRES);

	TRACE("%s-%d:HORZRES=%d, VERTRES=%d\n", __FUNCTION__, __LINE__, width, height);

	pCDC->BitBlt(0, 0, width, height, pDC, 0, 0, SRCCOPY);

	return TRUE;
}

BOOL SCEdit::DrawInformation(CDC * pCDC)
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

void SCEdit::Reset(void)
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

void SCEdit::OnLButtonDblClk(UINT nFlags, CPoint point)
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

BOOL SCEdit::GetScreenCaptureBitmap(CBitmap *pSelectedBitmap)
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

	pSelectedCDC.StretchBlt(0, 0, cRect.Width(), cRect.Height(), pDlgCDC,
		cRect.left, cRect.top, cRect.Width(), cRect.Height(), SRCCOPY);

	pSelectedCDC.DeleteDC();

	return TRUE;
}

void SCEdit::SetDrawRect(BOOL wParam)
{
	if (wParam) {
		m_selectAreaEditMode = SELECT_AREA_EDIT_DRAW_RECT;
		m_toolBarSet.SetFontSize(m_rectSize);
		m_toolBarSet.ShowWindow(SW_SHOW);
	}
	else {
		m_toolBarSet.ShowWindow(SW_HIDE);
		m_selectAreaEditMode = SELECT_AREA_EDIT_NONE;
	}
}

void SCEdit::SetDrawCircel(BOOL wParam)
{
	if (wParam) {
		m_selectAreaEditMode = SELECT_AREA_EDIT_DRAW_CIRCLE;
		m_toolBarSet.SetFontSize(m_circleSize);
		m_toolBarSet.ShowWindow(SW_SHOW);
	}
	else {
		m_selectAreaEditMode = SELECT_AREA_EDIT_NONE;
		m_toolBarSet.ShowWindow(SW_HIDE);
	}
}

void SCEdit::SetDrawArrow(BOOL wParam)
{
	if (wParam) {
		m_selectAreaEditMode = SELECT_AREA_EDIT_DRAW_ARROW;
		m_toolBarSet.SetFontSize(m_arrowSize);
		m_toolBarSet.ShowWindow(SW_SHOW);
	}
	else {
		m_selectAreaEditMode = SELECT_AREA_EDIT_NONE;
		m_toolBarSet.ShowWindow(SW_HIDE);
	}
}