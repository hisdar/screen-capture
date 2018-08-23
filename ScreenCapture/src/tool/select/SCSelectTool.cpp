#include "stdafx.h"
#include "tool/select/SCSelectTool.h"

SCSelectTool::SCSelectTool(CWnd *wnd)
: m_view(wnd)
, m_ctrl(&m_view)
{
}

SCSelectTool::~SCSelectTool()
{
}

SCBaseToolController *SCSelectTool::GetController()
{
	return &m_ctrl;
}

SCBaseToolView *SCSelectTool::GetView()
{
	return &m_view;
}

int SCSelectTool::GetToolType()
{
	return SC_TOOL_TYPE_SELECT;
}
