#pragma once

#include "tool/base/SCBaseToolView.h"
#include "base/SCObject.h"

/// @brief 蒙版工具，用来绘制蒙版
class SCMaskTool : public SCBaseToolView, public SCObject
{
public:
	SCMaskTool();
	virtual ~SCMaskTool();

	virtual BOOL Draw(CDC &cdc);

	void SetRect(CRect &rect);
	void SetRect(int left, int top, int right, int bottom);

private:
	BOOL DrawMetteRect(CDC *pCDC, int startX, int startY, int width, int height);

private:
	CRect m_rect;
};

