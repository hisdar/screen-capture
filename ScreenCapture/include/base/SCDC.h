#pragma once
class SCDC
{
public:
	SCDC();
	SCDC(CDC &dc);
	virtual ~SCDC();

	BOOL Create(CDC *dc);
	BOOL Create(CDC &dc);
	BOOL Save(LPCTSTR path);

	CDC *GetDC();
	CBitmap *GetBitMap();
	int GetWidth();
	int GetHeight();

private:
	CDC m_dc;
	CBitmap m_bmp;
	int m_width;
	int m_height;
};

