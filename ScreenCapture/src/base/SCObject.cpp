#include "stdafx.h"
#include "base/base-def.h"
#include "base/SCObject.h"


SCObject::SCObject()
{
}


SCObject::~SCObject()
{
}

BOOL SCObject::CreateCompatibleCDC(CDC *pSrcCDC, CDC *pTagetCDC, CBitmap *pTargetBmp)
{
	BOOL bRet = FALSE;

	if (pSrcCDC == NULL || pTargetBmp == NULL || pTagetCDC == NULL) {
		return FALSE;
	}

	// get src cdc size
	int cx = pSrcCDC->GetDeviceCaps(HORZRES);
	int cy = pSrcCDC->GetDeviceCaps(VERTRES);

	int phyWidth = pSrcCDC->GetDeviceCaps(HORZSIZE);
	int phyHeight = pSrcCDC->GetDeviceCaps(VERTSIZE);
	SCDbg("phyWidth=%d, phyHeight=%d\n", phyWidth, phyHeight);

	// create window dc memory copy
	pTagetCDC->DeleteDC();
	bRet = pTagetCDC->CreateCompatibleDC(pSrcCDC);
	if (!bRet) {
		SCErr("wndCDCMem.CreateCompatibleDC fail\n");
		return FALSE;
	}

	pTargetBmp->DeleteObject();
	bRet = pTargetBmp->CreateCompatibleBitmap(pSrcCDC, cx, cy);
	if (!bRet) {
		SCErr("wndBitmap.CreateCompatibleBitmap fail\n");
		return FALSE;
	}

	pTagetCDC->SelectObject(pTargetBmp);

	return TRUE;
}

BOOL SCObject::CopyCompatibleCDC(CDC *pSrcCDC, CDC *pTagetCDC, CBitmap *pTargetBmp)
{
	BOOL bRet = FALSE;

	if (pSrcCDC == NULL || pTargetBmp == NULL || pTagetCDC == NULL) {
		return FALSE;
	}

	// get src cdc size
	int nHorz = pSrcCDC->GetDeviceCaps(LOGPIXELSX);
	int nVert = pSrcCDC->GetDeviceCaps(LOGPIXELSY);
	SCDbg("LOGPIXELSX=%d, LOGPIXELSY=%d\n", nHorz, nVert);

	int cx = pSrcCDC->GetDeviceCaps(HORZRES);
	int cy = pSrcCDC->GetDeviceCaps(VERTRES);
	SCDbg("HORZRES=%d, VERTRES=%d\n", cx, cy);

	bRet = CreateCompatibleCDC(pSrcCDC, pTagetCDC, pTargetBmp);

	// copy src image to buffer dc
	bRet = pTagetCDC->StretchBlt(0, 0, cx, cy, pSrcCDC, 0, 0, cx, cy, SRCCOPY);
	// bRet = pTagetCDC->BitBlt(0, 0, cx, cy, pSrcCDC, 0, 0, SRCCOPY);
	if (!bRet) {
		SCErr("wndCDCMem.BitBlt fail\n");
		return FALSE;
	}

	return TRUE;
}
