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

BOOL SCMonitor::GetMonitorRect(CRect &rect)
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

	rect = mi.rcMonitor;

	return TRUE;
}

BOOL SCMonitor::GetMonitorSCDC(SCDC &scDC)
{
	int ret = FALSE;

	MONITORINFOEX mi;
	mi.cbSize = sizeof(MONITORINFOEX);
	ret = GetMonitorInfo(m_hMonitor, &mi);
	if (!ret) {
		SCErr("GetMonitorInfo fail\n");
		return FALSE;
	}

	DEVMODE devMode;
	ret = EnumDisplaySettings(mi.szDevice, ENUM_CURRENT_SETTINGS, &devMode);
	if (!ret) {
		SCErr("EnumDisplaySettings fail\n");
		return FALSE;
	}

	CDC dc;
	ret = dc.CreateDCW(mi.szDevice, NULL, NULL, NULL);
	if (!ret) {
		SCErr("dc.CreateDCW fail\n");
		return FALSE;
	}

	CDC *monitorDC = scDC.GetDC();
	monitorDC->DeleteDC();
	ret = monitorDC->CreateCompatibleDC(&dc);
	if (!ret) {
		SCErr("monitorDC->CreateCompatibleDC fail\n");
		return FALSE;
	}

	CBitmap *monitorBmp = scDC.GetBitMap();
	monitorBmp->DeleteObject();
	ret = monitorBmp->CreateCompatibleBitmap(&dc, devMode.dmPelsWidth, devMode.dmPelsHeight);
	if (!ret) {
		SCErr("monitorBmp->CreateCompatibleBitmap fail\n");
		return FALSE;
	}

	monitorDC->SelectObject(monitorBmp);

	CSize dcSize(dc.GetDeviceCaps(HORZRES), dc.GetDeviceCaps(VERTRES));
	int offsetX = devMode.dmPosition.x - mi.rcMonitor.left;
	int offsetY = devMode.dmPosition.y - mi.rcMonitor.top;

	ret = monitorDC->BitBlt(0, 0, devMode.dmPelsWidth, devMode.dmPelsHeight, &dc, offsetX, offsetY, SRCCOPY);

	return ret;
}



