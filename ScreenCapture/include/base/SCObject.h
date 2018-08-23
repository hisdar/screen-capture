#pragma once
class SCObject
{
public:
	SCObject();
	virtual ~SCObject();

	BOOL CreateCompatibleCDC(CDC *pSrcCDC, CDC *pTagetCDC, CBitmap *pTargetBmp);
	BOOL DCToImage(CDC &dc, CImage &image);
	BOOL GetCBitmapSize(CDC *pCDC, CSize &size);

	static BOOL SaveBmp(CDC &dc, LPCTSTR pszFileName);	
};

