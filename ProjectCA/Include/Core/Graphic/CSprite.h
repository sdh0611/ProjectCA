#pragma once


#include "..\..\..\stdafx.h"

const unsigned int SPRITE_WIDTH = 32;
const unsigned int SPRITE_HEIGHT = 48;


class CSprite {

public:
	CSprite();
	~CSprite();


public:
	bool Init(const Types::tstring& strPath);
	void Update();


public:
	const HBITMAP& GetBitmap() const { return m_hBit; }
	UINT GetBitWidth() const { return m_iBitWidth; }
	UINT GetBitHeight() const { return m_iBitHeight; }


private:
	HBITMAP	m_hBit;
	UINT			m_iBitWidth;
	UINT			m_iBitHeight;


};