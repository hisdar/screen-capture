#include "stdafx.h"
#include "base/base-def.h"
#include "SCMonitor.h"

SCMonitor::SCMonitor()
//: m_cdc(NULL)
{

}

SCMonitor::SCMonitor(const SCMonitor &scMonitor)
{
	Copy(scMonitor);
}

SCMonitor::~SCMonitor()
{
	//CHECK_DELETE(m_cdc);
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

	ret = Create(scMonitor.m_hMonitor);
	if (!ret) {
		SCErr("Create fail\n");
	}
}

CRect SCMonitor::GetRect()
{
	return m_rect;
}

CString SCMonitor::GetMonitorName(HMONITOR hMonitor)
{
	BOOL ret = FALSE;

	m_hMonitor = hMonitor;

	MONITORINFOEX mi;
	memset(&mi, 0x00, sizeof(mi));
	mi.cbSize = sizeof(mi);

	ret = GetMonitorInfo(hMonitor, &mi);
	if (!ret) {
		SCErr("GetMonitorInfo fail\n");
		return L"";
	}

	m_rect = mi.rcMonitor;

	return CString(mi.szDevice);
}

BOOL SCMonitor::Create(const CString &monitorName)
{
	BOOL ret = FALSE;

	m_name = monitorName;

	//CHECK_DELETE(m_cdc);
	//m_cdc = new CDC();
	ret = m_cdc.CreateDC(monitorName, NULL, NULL, NULL);
	if (!ret) {
		SCErr("m_cdc.CreateDC fail\n");
		return ret;
	}

	return TRUE;
}

BOOL SCMonitor::Create(HMONITOR hMonitor)
{
	CString name = GetMonitorName(hMonitor);

	return Create(name);
}

BOOL SCMonitor::GetScreenCDC(CDC &cdc, CBitmap &bmp)
{
	return CopyCompatibleCDC(&m_cdc, &cdc, &bmp);
}

