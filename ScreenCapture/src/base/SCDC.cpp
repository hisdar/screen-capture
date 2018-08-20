#include "stdafx.h"
#include "base/SCDC.h"
#include "base/base-def.h"

SCDC::SCDC()
{
	m_width = 0;
	m_height = 0;
}

SCDC::SCDC(CDC &dc)
{
	BOOL ret = FALSE;

	m_width = 0;
	m_height = 0;

	ret = Create(dc);
	if (!ret) {
		SCErr("Create SCDC fail\n");
		return;
	}
}


SCDC::~SCDC()
{
	m_dc.DeleteDC();
	m_bmp.DeleteObject();
}

BOOL SCDC::Create(CDC *dc)
{
	BOOL ret = FALSE;

	if (dc == NULL) {
		SCErr("input dc is NULL\n");
		return FALSE;
	}

	CBitmap *dcBmp = dc->GetCurrentBitmap();
	if (dcBmp == NULL) {
		SCErr("CBitmap object not selected\n");
		return FALSE;
	}

	CImage dcImage;
	dcImage.Attach((HBITMAP)dcBmp->GetSafeHandle());

	m_width = dcImage.GetWidth();
	m_height = dcImage.GetHeight();

	m_dc.DeleteDC();
	ret = m_dc.CreateCompatibleDC(dc);
	if (!ret) {
		SCErr("create dc fail\n");
		return FALSE;
	}

	m_bmp.DeleteObject();
	ret = m_bmp.CreateCompatibleBitmap(dc, m_width, m_height);
	if (!ret) {
		SCErr("create bitmap fail\n");
		return FALSE;
	}

	m_dc.SelectObject(m_bmp);

	ret = m_dc.BitBlt(0, 0, m_width, m_height, dc, 0, 0, SRCCOPY);

	return ret;
}

BOOL SCDC::Create(CDC &dc)
{
	return Create(&dc);
}

BOOL SCDC::Save(LPCTSTR path)
{
	BOOL ret = FALSE;

	CBitmap *dcBmp = m_dc.GetCurrentBitmap();
	if (dcBmp == NULL) {
		SCErr("CBitmap object not selected\n");
		return FALSE;
	}

	CImage dcImage;
	dcImage.Attach((HBITMAP)dcBmp->GetSafeHandle());

	HRESULT result = dcImage.Save(path);
	return TRUE;
}

CDC *SCDC::GetDC()
{
	return &m_dc;
}

CBitmap *SCDC::GetBitMap()
{
	return &m_bmp;
}

int SCDC::GetWidth()
{
	return m_width;
}

int SCDC::GetHeight()
{
	return m_height;
}