#include "stdafx.h"
#include "base/base-def.h"
#include "SCMonitorManager.h"


SCMonitorManager::SCMonitorManager()
{
}


SCMonitorManager::~SCMonitorManager()
{
	//m_monitorArray.RemoveAll();
}

BOOL SCMonitorManager::GetMonitors(SCMonitorArray &array)
{
	int ret = FALSE;

	//m_monitorArray.RemoveAll();

//	EnumDisplaySettings
//	EnumDisplayDevices
	ret = EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)&array);
	if (!ret) {
		SCErr("EnumDisplayMonitors fail, err:%d\n", GetLastError());
		return ret;
	}
	
	return TRUE;
}

BOOL SCMonitorManager::GetMonitor(HMONITOR &monitor, int index)
{
	int ret = FALSE;

	SCMonitorArray array;
	ret = GetMonitors(array);
	if (!ret) {
		SCErr("get monitors fail\n");
		return ret;
	}

	if (index >= array.GetSize()) {
		SCErr("index out of bounds, index=%d, array length=%d\n", index, array.GetSize());
		return FALSE;
	}

	monitor = array.GetAt(index);

	return TRUE;
}

BOOL SCMonitorManager::MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	int ret = FALSE;

	if (dwData == NULL) {
		SCErr("param check fail\n");
		return TRUE;
	}

	SCMonitorArray *monitorArray = (SCMonitorArray *)dwData;

	monitorArray->Add(hMonitor);

	return TRUE;
}

