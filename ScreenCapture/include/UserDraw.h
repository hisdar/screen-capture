#pragma once
#include "atltypes.h"

#define USER_DRAW_SHAPE_NONE	0
#define USER_DRAW_SHAPE_RECT	1
#define USER_DRAW_SHAPE_CIRCLE	2
#define USER_DRAW_SHAPE_ARROW	3
#define USER_DRAW_SHAPE_TEXT	4

class CUserDraw
{
public:
	CUserDraw(void);
	CUserDraw(const CUserDraw &userDraw);
	~CUserDraw(void);

	void CopyUserDraw(const CUserDraw &userDraw);
	CUserDraw& operator=(const CUserDraw &userDraw);

public:
	CPoint m_startPoint;
	CPoint m_endPoint;
	CString m_str;

	COLORREF m_color;
	int m_shape;
	int m_size;
};

