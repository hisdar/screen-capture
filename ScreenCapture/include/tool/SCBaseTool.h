#pragma once

typedef enum SC_TOOL_TYPE {
	SC_TOOL_TYPE_NONE,
	SC_TOOL_TYPE_ARROW,
	SC_TOOL_TYPE_CIRCLE,
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

	virtual int GetToolType() = 0;

	virtual BOOL Draw(CDC &cdc) { return TRUE; };
	virtual void OnLButtonDown(UINT nFlags, CPoint point) {};
	virtual void OnLButtonUp(UINT nFlags, CPoint point) {};
	virtual void OnMouseMove(UINT nFlags, CPoint point) {};
	virtual void OnRButtonDown(UINT nFlags, CPoint point) {};
	virtual void OnRButtonUp(UINT nFlags, CPoint point) {};
	virtual void OnLButtonDblClk(UINT nFlags, CPoint point) {};
};

