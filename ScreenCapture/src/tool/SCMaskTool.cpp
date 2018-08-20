#include "stdafx.h"
#include "base/base-def.h"
#include "tool/SCMaskTool.h"


SCMaskTool::SCMaskTool()
{
}

SCMaskTool::~SCMaskTool()
{
}

BOOL SCMaskTool::Draw(CDC &cdc)
{
	return DrawMetteRect(&cdc, m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height());
}

void SCMaskTool::SetRect(CRect &rect)
{
	m_rect = rect;
}

void SCMaskTool::SetRect(int left, int top, int right, int bottom)
{
	m_rect.SetRect(left, top, right, bottom);
}

BOOL SCMaskTool::DrawMetteRect(CDC *pCDC, int startX, int startY, int width, int height)
{
	BOOL bRet = FALSE;

	// mette hdc
	CDC metteCDC;
	CBitmap metteBmp;

	bRet = CreateCompatibleCDC(pCDC, &metteCDC, &metteBmp);
	if (!bRet) {
		SCDbg("CreateCompatibleCDC fail\n");
		return bRet;
	}

	COLORREF colorREF = RGB(0, 0, 0);
	metteCDC.FillSolidRect(0, 0, width, height, colorREF);

	// 
	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.AlphaFormat = 0;
	bf.SourceConstantAlpha = 90;
	bRet = pCDC->AlphaBlend(startX, startY, width, height, &metteCDC, 0, 0, width, height, bf);
	if (!bRet) {
		SCDbg("AlphaBlend fail: target point[%d, %d], target size:[%d, %d] \n", startX, startY, width, height);
		SCDbg("AlphaBlend fail: src point[%d, %d], src size:[%d, %d] \n", startX, startY, width, height);
		return bRet;
	}

	metteBmp.DeleteObject();
	metteCDC.DeleteDC();

	return bRet;
}
