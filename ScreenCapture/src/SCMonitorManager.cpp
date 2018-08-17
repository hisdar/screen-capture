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

	ret = EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)&array);
	if (!ret) {
		SCErr("EnumDisplayMonitors fail, err:%d\n", GetLastError());
		return ret;
	}
	
	return TRUE;
}

BOOL SCMonitorManager::MonitorEnumProc(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	int ret = FALSE;

	if (dwData == NULL) {
		SCErr("param check fail\n");
		return TRUE;
	}

	SCDbg("monitor rect:[%d, %d, %d, %d]\n", lprcMonitor->left, lprcMonitor->top, lprcMonitor->right, lprcMonitor->bottom);
	SCMonitorArray *self = (SCMonitorArray *)dwData;

	SCMonitor scMonitor;
	ret = scMonitor.Create(hMonitor);
	if (!ret) {
		SCErr("scMonitor.Create fail\n");
		return TRUE; // 这里要返回TRUE， 返回FALSE标识停止枚举
	}

	//self->Add(scMonitor);
	self->Add(hMonitor);

	return TRUE;
}

