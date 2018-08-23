#pragma once

#include "SCBaseToolController.h"
#include "tool/base/SCBaseToolView.h"

typedef enum SC_TOOL_TYPE {
	SC_TOOL_TYPE_ARROW = 1,
	SC_TOOL_TYPE_MASK,
	SC_TOOL_TYPE_RECTANGLE,
	SC_TOOL_TYPE_SELECT,
	SC_TOOL_TYPE_TEXT,
} SCToolType;

class SCBaseTool
{
public:
	SCBaseTool() {};
	virtual ~SCBaseTool() {};

	virtual SCBaseToolController *GetController() = 0;
	virtual SCBaseToolView *GetView() = 0;
	virtual int GetToolType() = 0;
};

