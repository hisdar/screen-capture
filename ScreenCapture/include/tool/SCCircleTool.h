#pragma once

#include "base/SCObject.h"

class SCCircleTool : public SCObject
{
public:
	SCCircleTool();
	virtual ~SCCircleTool();

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

