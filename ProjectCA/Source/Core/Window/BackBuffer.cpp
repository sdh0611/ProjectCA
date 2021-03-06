#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Core\Window\BackBuffer.h"


BackBuffer::BackBuffer()
	:m_hMemDC(NULL), m_hBlendDC(NULL), 
	m_hBit(NULL), m_hOldBit(NULL)
{
}

BackBuffer::~BackBuffer()
{
	if (m_hBit)
		DeleteObject(m_hBit);

	if (m_hOldBit)
		DeleteObject(m_hOldBit);
	
	if (m_hMemDC)
		DeleteDC(m_hMemDC);

	if (m_hBlendDC)
		DeleteDC(m_hBlendDC);

}

bool BackBuffer::Init(const HDC & hDC)
{
	m_hMemDC = CreateCompatibleDC(hDC);
	if (m_hMemDC == NULL)
		return false;

	m_hBlendDC = CreateCompatibleDC(hDC);
	if (m_hBlendDC == NULL)
		return false;

	return true;
}

void BackBuffer::Update()
{
}

bool BackBuffer::DrawSet(const HDC & hDC)
{
	m_hBit = CreateCompatibleBitmap(hDC, MAX_WIDTH, MAX_HEIGHT);
	if (!m_hBit)
		return false;

	m_hOldBit = (HBITMAP)SelectObject(m_hMemDC, m_hBit);

	return true;
}

bool BackBuffer::DrawEnd()
{
	DeleteObject(SelectObject(m_hMemDC, m_hOldBit));

	return true;
}

HDC&& BackBuffer::AllocationCompatibleDC()
{
	return std::move(CreateCompatibleDC(m_hMemDC));
}

const HDC & BackBuffer::GetMemDC() const
{
	return m_hMemDC;
}

const HDC & BackBuffer::GetBlendDC() const
{
	return m_hBlendDC;
}

