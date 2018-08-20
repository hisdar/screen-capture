#pragma once

#include "base/SCObject.h"
#include "base/base-def.h"

class SCMonitor : public SCObject
{
public:
	SCMonitor(HMONITOR hMonitor);
	SCMonitor(const SCMonitor &scMonitor);
	virtual ~SCMonitor();

	void operator=(const SCMonitor &scMonitor);
	BOOL GetScreenImage(CImage &image);

private:
	void Copy(const SCMonitor &scMonitor);

private:
	HMONITOR m_hMonitor;
};

