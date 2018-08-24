#pragma once

#include "base/SCObject.h"

/// @brief 矩形工具，用来绘制矩形
class SCRectangleTool : public SCObject
{
public:
	SCRectangleTool();
	virtual ~SCRectangleTool();

	virtual BOOL Draw(CDC &cdc);

	void SetRect(CRect &rect);
	void SetRect(CPoint &startPoint, CPoint &endPoint);
	void SetColor(COLORREF color);
	void SetSize(int size);

private:
	CRect m_rect;
	COLORREF m_color;
	int m_size;
};

