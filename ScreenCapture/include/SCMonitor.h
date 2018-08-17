#pragma once

#include "base/SCObject.h"
#include "base/base-def.h"

class SCMonitor : public SCObject
{
public:
	SCMonitor();
	SCMonitor(const SCMonitor &scMonitor);
	virtual ~SCMonitor();

	BOOL Create(HMONITOR hMonitor);

	void operator=(const SCMonitor &scMonitor);
	BOOL GetScreenCDC(CDC &cdc, CBitmap &bmp);
	CRect GetRect();

private:
	void Copy(const SCMonitor &scMonitor);
	BOOL Create(const CString &monitorName);
	CString GetMonitorName(HMONITOR hMonitor);

private:
	CDC m_cdc;
	CRect m_rect;
	CString m_name;
	HMONITOR m_hMonitor;
};

