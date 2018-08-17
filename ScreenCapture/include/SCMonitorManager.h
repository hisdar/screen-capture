#pragma once

#include "SCMonitor.h"

typedef CArray <HMONITOR, HMONITOR> SCMonitorArray;

class SCMonitorManager
{
public:
	SCMonitorManager();
	virtual ~SCMonitorManager();

	BOOL GetMonitors(SCMonitorArray &array);

private:
	static BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData);

private:
	//SCMonitorArray m_monitorArray;
};

