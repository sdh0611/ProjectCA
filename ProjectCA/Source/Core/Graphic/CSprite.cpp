#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Core\Graphic\CSprite.h"


CSprite::CSprite()
{

}

CSprite::~CSprite()
{
	if (m_hBit)
		DeleteObject(m_hBit);
}

bool CSprite::Init(const Types::tstring& strPath)
{
	m_hBit = (HBITMAP)LoadImage(NULL, strPath.c_str(), IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE);
	if (!m_hBit)
		return false;
	
	BITMAP bit;

	GetObject(m_hBit, sizeof(bit), &bit);
	
	m_iBitWidth = bit.bmWidth;
	m_iBitHeight = bit.bmHeight;

	return true;
}

void CSprite::Update()
{
}
