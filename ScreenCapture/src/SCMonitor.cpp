#include "stdafx.h"
#include "base/base-def.h"
#include "SCMonitor.h"

SCMonitor::SCMonitor(HMONITOR hMonitor)
:m_hMonitor(hMonitor)
{

}

SCMonitor::SCMonitor(const SCMonitor &scMonitor)
{
	Copy(scMonitor);
}

SCMonitor::~SCMonitor()
{

}

void SCMonitor::operator=(const SCMonitor &scMonitor)
{
	Copy(scMonitor);
}

void SCMonitor::Copy(const SCMonitor &scMonitor)
{
	int ret = FALSE;

	if (this == &scMonitor) {
		return;
	}

	m_hMonitor = scMonitor.m_hMonitor;
}

BOOL SCMonitor::GetScreenImage(CImage &image)
{
	BOOL ret = FALSE;

	MONITORINFOEX mi;
	memset(&mi, 0x00, sizeof(mi));
	mi.cbSize = sizeof(mi);

	ret = GetMonitorInfo(m_hMonitor, &mi);
	if (!ret) {
		SCErr("GetMonitorInfo fail\n");
		return FALSE;
	}

	CDC dc;
	ret = dc.CreateDCW(mi.szDevice, NULL, NULL, NULL);
	if (!ret) {
		SCErr("create dc fail, device name:%s\n", mi.szDevice);
		return FALSE;
	}

	CBitmap *selectedBitmap = dc.GetCurrentBitmap();

	// exchange bitmap to image
	CImage selectedImage;
	selectedImage.Attach((HBITMAP)selectedBitmap->GetSafeHandle());

	SCDbg("img size:%d, %d\n", selectedImage.GetWidth(), selectedImage.GetHeight());
	selectedImage.Save(L"GetScreenImage-dc.bmp", Gdiplus::ImageFormatBMP);

	//SaveBmp(dc, L"GetScreenImage-dc.bmp");

	//ret = DCToImage(dc, image);
	if (!ret) {
		SCErr("DCToImage fail\n");
		return ret;
	}

	//image.Save(L"GetScreenImage.bmp");
	return TRUE;
}



