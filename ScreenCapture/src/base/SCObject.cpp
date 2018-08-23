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
	SCDbg("width=%d, height=%d\n", cx, cy);

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

BOOL SCObject::SaveBmp(CDC &dc, LPCTSTR pszFileName)
{
	// get selected bit map
	CBitmap *selectedBitmap = dc.GetCurrentBitmap();

	// exchange bitmap to image
	CImage selectedImage;
	selectedImage.Attach((HBITMAP)selectedBitmap->GetSafeHandle());
	
	SCDbg("img size:%d, %d\n", selectedImage.GetWidth(), selectedImage.GetHeight());
	selectedImage.Save(pszFileName, Gdiplus::ImageFormatBMP);
	
	return TRUE;
}

BOOL SCObject::DCToImage(CDC &dc, CImage &image)
{
	BOOL ret = FALSE;

	CImage bmpImage;

	CBitmap *cBmp = dc.GetCurrentBitmap();
	bmpImage.Attach((HBITMAP)cBmp->GetSafeHandle());

	bmpImage.Save(L"DCToImage.bmp");

	image.Destroy();
	ret = image.Create(bmpImage.GetWidth(), bmpImage.GetHeight(), bmpImage.GetBPP());
	if (!ret) {
		SCErr("image.Create\n");
		return ret;
	}

	ret = bmpImage.Draw(image.GetDC(), 0, 0);
	
	image.ReleaseDC();

	return TRUE;
}

BOOL SCObject::GetCBitmapSize(CDC *pCDC, CSize &size)
{
	int ret = FALSE;

	CBitmap *dcBitmap = pCDC->GetCurrentBitmap();
	if (dcBitmap == NULL) {
		SCErr("dc GetCurrentBitmap is NULL\n");
		return FALSE;
	}

	BITMAP dcBMP;
	ret = dcBitmap->GetBitmap(&dcBMP);
	if (!ret) {
		SCErr("dcBitmap->GetBitmap fail\n");
		return FALSE;
	}

	size.cx = dcBMP.bmWidth;
	size.cy = dcBMP.bmHeight;

	return TRUE;
}
