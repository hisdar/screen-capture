#pragma once


// HButton

class HButton : public CButton
{
	DECLARE_DYNAMIC(HButton)

public:
	HButton();
	virtual ~HButton();

	BOOL m_isMouseIn;
	BOOL m_isClicked;

	CBitmap m_bmpMouseIn;
	CBitmap m_bmpDefault;
	CBitmap m_bmpClicked;

protected:
	DECLARE_MESSAGE_MAP()

private:
	BOOL HSetBtnBitMap();

public:
	afx_msg void OnMouseLeave();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	BOOL HLoadBitMap(UINT deffaultBmpId, UINT mouseInBmpId, UINT clickedBmpId);
	BOOL HSetBtnClicked(BOOL isClicked);
};


