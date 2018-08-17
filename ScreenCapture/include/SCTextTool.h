#pragma once

#include "tool/base/SCBaseToolView.h"
#include "base/SCObject.h"

#define FONT_ADJUST						8
#define DEFAULT_FONT					_T("Î¢ÈíÑÅºÚ")

class SCTextTool : public SCBaseToolView, public SCObject
{
public:
	SCTextTool();
	virtual ~SCTextTool();

	virtual BOOL Draw(CDC &cdc);

	void SetColor(COLORREF color);
	void SetSize(int size);
	void SetPoint(CPoint &point);
	void SetString(CString &string);

private:
	CRect m_rect;
	COLORREF m_color;
	CPoint m_point;
	CString m_string;

	int m_size;
};

