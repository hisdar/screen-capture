// SCEditor.cpp : 实现文件
//

#include "stdafx.h"
#include "SCDrawPanel.h"
#include "afxdialogex.h"
#include "base/base-def.h"
#include "tool/mask/SCMaskTool.h"
#include "tool/rectangle/SCRectangleTool.h"
#include "tool/circle/SCCircleTool.h"
#include "tool/arrow/SCArrowTool.h"
#include "tool/text/SCTextTool.h"
#include "tool/select/SCSelectTool.h"
#include "SCMonitorManager.h"
// SCEditor 对话框

IMPLEMENT_DYNAMIC(SCDrawPanel, CDialogEx)

SCDrawPanel::SCDrawPanel(CWnd* pParent /*=NULL*/)
	: CDialogEx(SCDrawPanel::IDD, pParent)
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
	, m_size(1)
	, m_color(RGB(0, 0, 0))
	, m_currentTool(NULL)
{
	m_curPointZoomX = 1.0f;
	m_curPointZoomY = 1.0f;
}

SCDrawPanel::~SCDrawPanel()
{
}

void SCDrawPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(SCDrawPanel, CDialogEx)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL SCDrawPanel::UpdateBaseImage(SCDC &scDC)
{
	int ret = FALSE;

	ret = m_scDC.Create(scDC.GetDC());
	if (!ret) {
		SCDbg("scdc create fail\n");
		return FALSE;
	}

	UpdateCurLocationZoom();

	Invalidate();

	return TRUE;
}

void SCDrawPanel::SetState(int state)
{
	m_screenCaptureState = state;
	if (m_screenCaptureState == SCREEN_CAPTURE_STATE_START) {
		if (m_currentTool != NULL) {
			delete m_currentTool;
		}

		m_currentTool = new SCSelectTool(this);
	}
}

void SCDrawPanel::SetEditMode(int editMode)
{
	m_selectAreaEditMode = editMode;
}

int SCDrawPanel::GetRectSize()
{
	return m_rectSize;
}

void SCDrawPanel::AddListener(SCDrawPanelListener *listener)
{
	for (unsigned i = 0; i < m_scDrawPanelListenerArray.GetSize(); i++) {
		if (m_scDrawPanelListenerArray.GetAt(i) == listener) {
			return;
		}
	}

	m_scDrawPanelListenerArray.Add(listener);
}

void SCDrawPanel::RemoveListener(SCDrawPanelListener *listener)
{
	for (unsigned i = 0; i < m_scDrawPanelListenerArray.GetSize(); i++) {
		if (m_scDrawPanelListenerArray.GetAt(i) == listener) {
			m_scDrawPanelListenerArray.RemoveAt(i);
			return;
		}
	}
}

void SCDrawPanel::UpdateCurLocationZoom()
{
	CSize baseImgSize;
	GetCBitmapSize(m_scDC.GetDC(), baseImgSize);

	CRect wndRect;
	GetWindowRect(&wndRect);

	m_curPointZoomX = 1.0f * baseImgSize.cx / wndRect.Width();
	m_curPointZoomY = 1.0f * baseImgSize.cy / wndRect.Height();
}

CPoint SCDrawPanel::GetZoomedPoint(const CPoint &srcPoint)
{
	CPoint zoomedPoint;
	zoomedPoint.x = srcPoint.x * m_curPointZoomX;
	zoomedPoint.y = srcPoint.y * m_curPointZoomY;

	return zoomedPoint;
}

////////////////////////////////////////////////////////////
BOOL SCDrawPanel::DrawScreenCaptureResult(CDC * pTagDC)
{
	BOOL bRet = FALSE;

	// 1. draw user draw
	// 1.1 create user draw cdc based on screen cdc
	m_usreDrawSCDC.Create(m_scDC.GetDC());

	// 1.2 draw user darw
	bRet = DrawUserDraw(&m_usreDrawSCDC);
	if (!bRet) {
		SCErr("DrawUserDrawfail\n");
		return FALSE;
	}
	// 2. copy user draw to wnndow cdc
	SCDC wndSCDC(m_usreDrawSCDC.GetDC());

	// 3. draw rect on window cdc
	CSize wndBmpSize, dialogBmpSize;
	bRet = GetCBitmapSize(wndSCDC.GetDC(), wndBmpSize);
	GetCBitmapSize(pTagDC, dialogBmpSize);

	int cxDialog = pTagDC->GetDeviceCaps(HORZRES);
	int cyDialog = pTagDC->GetDeviceCaps(VERTRES);

	int cxWnd = wndSCDC.GetDC()->GetDeviceCaps(HORZRES);
	int cyWnd = wndSCDC.GetDC()->GetDeviceCaps(VERTRES);

	CRect dialogRect;
	GetWindowRect(&dialogRect);

	int cx = cxDialog;
	int cy = cyDialog;
	// 4. draw selected area
	//m_currentTool
	SCDbg("dialog dc size:%d, %d, dialog bmp size:%d, %d\n", cxDialog, cyDialog, dialogBmpSize.cx, dialogBmpSize.cy);
	SCDbg("scdc dc size:%d, %d, scdc bmp size:%d, %d\n", cxWnd, cyWnd, wndBmpSize.cx, wndBmpSize.cy);
	SCDbg("dialog rect:%d, %d, %d, %d\n", dialogRect.left, dialogRect.top, dialogRect.right, dialogRect.bottom);

	m_currentTool->GetView()->Draw(*wndSCDC.GetDC());

	bRet = pTagDC->StretchBlt(0, 0, dialogBmpSize.cx, dialogBmpSize.cy,
		wndSCDC.GetDC(), 0, 0, wndBmpSize.cx, wndBmpSize.cy, SRCCOPY);
	if (!bRet) {
		SCErr("cDC->BitBlt fail");
		return FALSE;
	}

	return TRUE;
}

BOOL SCDrawPanel::DrawUserDraw(SCDC * pCDC)
{
	/*if (m_userDrawArray.GetSize() <= 0) {
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
	}*/

	return TRUE;
}

BOOL SCDrawPanel::CreateUserDraw(CPoint * pPoint)
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

BOOL SCDrawPanel::IsPointInRect(CPoint * pPoint, CRect * pCRect)
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


BOOL SCDrawPanel::ModifyUserDrawEndPoint(CPoint * pPoint)
{
	if (m_userDrawArray.GetSize() <= 0) {
		return TRUE;
	}

	CUserDraw &userDraw = m_userDrawArray.ElementAt(m_userDrawArray.GetSize() - 1);
	userDraw.m_endPoint.SetPoint(pPoint->x, pPoint->y);

	return TRUE;
}

void SCDrawPanel::Reset(void)
{
	m_isMouseLButtonDown = FALSE;
	m_screenCaptureState = SCREEN_CAPTURE_STATE_NONE;
	m_resizeDirection = RESIZE_LOCATION_NONE;
	m_selectAreaEditMode = SELECT_AREA_EDIT_NONE;

	m_startLocation.SetPoint(-1, -1);
	m_endLocation.SetPoint(-1, -1);
	m_selecteAreaLastPoint.SetPoint(-1, -1);

	m_userDrawArray.RemoveAll();

}

void SCDrawPanel::UpdateCursorIcon(CPoint *pPoint)
{
	LONG cursorIcon = (LONG)LoadCursor(NULL, IDC_ARROW);

	// selected area is in edit
	if (m_selectAreaEditMode != SELECT_AREA_EDIT_NONE) {
		GetEditModeCursorIcon(pPoint, &cursorIcon);
	}
	

	//GetClassLong();
	SetClassLong(this->GetSafeHwnd(), GCL_HCURSOR, cursorIcon);
}

BOOL SCDrawPanel::GetEditModeCursorIcon(CPoint *pPoint, LONG *cursorIcon)
{
	CRect selectedArea = m_selectedRect;

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

BOOL SCDrawPanel::DrawInformation(CDC * pCDC)
{
	CRect cRect = m_selectedRect;

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

BOOL SCDrawPanel::GetSelectedBitmap(CBitmap *pSelectedBitmap)
{
	if (pSelectedBitmap == NULL) {
		return FALSE;
	}

	CRect cRect = m_selectedRect;

	CDC *pDlgCDC = m_usreDrawSCDC.GetDC();

	CDC pSelectedCDC;
	pSelectedCDC.CreateCompatibleDC(pDlgCDC);

	pSelectedBitmap->DeleteObject();
	pSelectedBitmap->CreateCompatibleBitmap(pDlgCDC, cRect.Width(), cRect.Height());
	pSelectedCDC.SelectObject(pSelectedBitmap);

	pSelectedCDC.SetStretchBltMode(STRETCH_HALFTONE);
	pSelectedCDC.StretchBlt(0, 0, cRect.Width(), cRect.Height(), pDlgCDC,
		cRect.left, cRect.top, cRect.Width(), cRect.Height(), SRCCOPY);

	pSelectedCDC.DeleteDC();

	return TRUE;
}


/////////////////////// SCEditor 消息处理程序 ////////////////////////////////////////
void SCDrawPanel::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	// 3. draw rect on window cdc
	int width = dc.GetDeviceCaps(HORZRES);
	int height = dc.GetDeviceCaps(VERTRES);

	DrawScreenCaptureResult(&dc);
}

void SCDrawPanel::OnLButtonDown(UINT nFlags, CPoint point)
{
	BOOL bRet = FALSE;

	m_currentTool->GetController()->OnLButtonDown(nFlags, GetZoomedPoint(point));

	CDialogEx::OnLButtonDown(nFlags, point);
}

void SCDrawPanel::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_currentTool->GetController()->OnLButtonUp(nFlags, GetZoomedPoint(point));
	CDialogEx::OnLButtonUp(nFlags, point);
}

void SCDrawPanel::OnRButtonDown(UINT nFlags, CPoint point)
{
	m_currentTool->GetController()->OnRButtonDown(nFlags, GetZoomedPoint(point));
	CDialogEx::OnRButtonDown(nFlags, point);
}

void SCDrawPanel::OnRButtonUp(UINT nFlags, CPoint point)
{
	m_currentTool->GetController()->OnRButtonUp(nFlags, GetZoomedPoint(point));
	CDialogEx::OnRButtonUp(nFlags, point);
}

void SCDrawPanel::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	m_currentTool->GetController()->OnLButtonDblClk(nFlags, GetZoomedPoint(point));
	CDialogEx::OnLButtonDblClk(nFlags, point);
}

void SCDrawPanel::OnMouseMove(UINT nFlags, CPoint point)
{
	m_currentTool->GetController()->OnMouseMove(nFlags, GetZoomedPoint(point));

	if (m_currentTool->GetToolType() == SC_TOOL_TYPE_SELECT) {
		CRect rect;
		((SCSelectToolView *)m_currentTool->GetView())->GetSelectedRect(rect);

		if (!rect.IsRectEmpty() && !m_selectedRect.EqualRect(rect)) {
			for (unsigned i = 0; i < m_scDrawPanelListenerArray.GetSize(); i++) {
				SCDrawPanelListener *l = m_scDrawPanelListenerArray.GetAt(i);
				l->SelectedAreaChangeEvent(this, rect);
			}

			m_selectedRect = rect;
		}
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

// over ride and fix flicker issue
BOOL SCDrawPanel::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;

	// return CDialogEx::OnEraseBkgnd(pDC);
}

void SCDrawPanel::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	UpdateCurLocationZoom();
}
