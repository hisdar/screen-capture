#pragma once

#include "SCBaseToolController.h"
#include "tool/base/SCBaseToolView.h"

#define SC_TOOL_TYPE_SELECT 1

class SCBaseTool
{
public:
	SCBaseTool() {};
	virtual ~SCBaseTool() {};

	virtual SCBaseToolController *GetController() = 0;
	virtual SCBaseToolView *GetView() = 0;
	virtual int GetToolType() = 0;
};

