#pragma once

#include "..\..\stdafx.h"

class CSprite;

class CTile {

public:
	CTile();
	~CTile();


public:
	bool PostInit(const Types::tstring& strImage);
	void Init();


public:
	void SetTileWidth(UINT iWidth);
	void SetTileHeight(UINT iHeight);
	void SetTileSize(UINT iWidth, UINT iHeight);
	bool SetTileImage(const Types::tstring& strImage);
	UINT GetTileWidth();
	UINT GetTileHeight();


private:
	UINT							m_iTileWidth;
	UINT							m_iTileHeight;
	std::weak_ptr<CSprite>	m_pTileImage;


};