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

BOOL SCMonitor::GetScreenImage(SCDC &scDC)
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
	dc.CreateDCW(mi.szDevice, NULL, NULL, NULL);
	if (!ret) {
		SCErr("create dc fail, device name:%s\n", mi.szDevice);
		return FALSE;
	}

	ret = scDC.Create(dc);
	if (!ret) {
		SCErr("scDC create fail\n");
		return FALSE;
	}

	scDC.Save(L"GetScreenImage.bmp");

	return TRUE;
}



