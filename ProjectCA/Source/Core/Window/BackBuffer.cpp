#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Core\Window\BackBuffer.h"


BackBuffer::BackBuffer(const HDC& hDC)
	:m_hMemDC(NULL), m_hDrawDC(NULL),
	m_hBit(NULL), m_hLoadBit(NULL), m_hOldBit(NULL)
{
	m_hMemDC = CreateCompatibleDC(hDC);
	m_hDrawDC = CreateCompatibleDC(hDC);
}

BackBuffer::BackBuffer()
	:m_hMemDC(NULL), m_hBit(NULL), m_hLoadBit(NULL), m_hOldBit(NULL)
{
}

BackBuffer::~BackBuffer()
{
	if (m_hBit)
		DeleteObject(m_hBit);

	if (m_hLoadBit)
		DeleteObject(m_hLoadBit);

	if (m_hOldBit)
		DeleteObject(m_hOldBit);
	
	if (m_hMemDC)
		DeleteDC(m_hMemDC);

	if (m_hDrawDC)
		DeleteDC(m_hDrawDC);

}

bool BackBuffer::DrawSet(const HDC & hDC)
{
	//m_hMemDC = CreateCompatibleDC(hDC);
	//if (!m_hMemDC)
	//	return false;

	m_hBit = CreateCompatibleBitmap(hDC, MAX_WIDTH, MAX_HEIGHT);
	if (!m_hBit)
		return false;

	m_hOldBit = (HBITMAP)SelectObject(m_hMemDC, m_hBit);

	return true;
}

bool BackBuffer::DrawEnd()
{
	DeleteObject(SelectObject(m_hMemDC, m_hOldBit));

	//if (m_hMemDC)
	//	DeleteDC(m_hMemDC);

	return true;
}
