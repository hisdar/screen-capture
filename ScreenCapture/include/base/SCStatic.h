#pragma once


// HStatic

class SCStatic : public CStatic
{
	DECLARE_DYNAMIC(SCStatic)

public:
	SCStatic();
	virtual ~SCStatic();

private:
	COLORREF m_backgroundColor;
	BOOL m_isMouseIn;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

	void SetBgColor(COLORREF color);
	COLORREF GetBgColor();

	void SetIsMouseIn(BOOL isMouseIn);

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
};


