#pragma once

#include "..\..\stdafx.h"

class CSprite;
class CActor;


class CTile {

public:
	CTile();
	~CTile();


public:
	bool PostInit(const TSTRING& strImage);
	void Init();


public:
	void SetTileWidth(UINT iWidth);
	void SetTileHeight(UINT iHeight);
	void SetTileSize(UINT iWidth, UINT iHeight);
	bool SetTileImage(const Types::tstring& strImage);
	

public:
	UINT							GetTileWidth();
	UINT							GetTileHeight();
	std::weak_ptr<CSprite>	GetTileImageInfo();
	

private:
	UINT							m_iTileWidth;
	UINT							m_iTileHeight;
	std::weak_ptr<CActor>	m_pActor;				//해당 타일에 위치한 Actor를 가리키는 weak_ptr
	std::weak_ptr<CSprite>	m_pTileImage;			


};