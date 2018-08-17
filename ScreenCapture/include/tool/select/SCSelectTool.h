#pragma once

#include "tool/base/SCBaseTool.h"
#include "tool/select/SCSelectToolView.h"
#include "tool/select/SCSelectToolController.h"

class SCSelectTool : public SCBaseTool
{
public:
	SCSelectTool(CWnd *wnd);
	virtual ~SCSelectTool();

	virtual SCBaseToolController *GetController();
	virtual SCBaseToolView *GetView();
	virtual int GetToolType();

private:
	SCSelectToolView m_view;
	SCSelectToolController m_ctrl;
};

