#pragma once

#include "tool/base/SCBaseToolView.h"
#include "base/SCObject.h"

class SCArrowTool : public SCBaseToolView, public SCObject
{
public:
	SCArrowTool();
	virtual ~SCArrowTool();

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

