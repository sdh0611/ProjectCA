#pragma once

#include "..\..\..\stdafx.h"
#include "CProb.h"

class CSprite;


class CGround : public CProb {
	enum GroundInfo { 
		GI_INNER = 0,
		GI_EDGE_RIGHT_TOP, GI_EDGE_RIGHT_BOT, GI_EDGE_RIGHT_INNER,
		GI_EDGE_LEFT_TOP, GI_EDGE_LEFT_BOT, GI_EDGE_LEFT_INNER,
		GI_LINE_TOP, GI_LINE_LEFT, GI_LINE_BOT, GI_LINE_RIGHT
	};

	struct TileInfo {
		TileInfo(float fx, float fy, UINT iWidth, UINT iHeight, GroundInfo _info)
			: position(fx, fy), iTileWidth(iWidth), iTileHeight(iHeight), info(_info)
		{
		}
		
		UINT			iTileWidth;
		UINT			iTileHeight;
		GroundInfo	info;
		POSITION	position;
	};

public:
	virtual bool PostInit(const OBJECT_DATA&, CScene*) override;
	virtual void Init() override;
	virtual void Update(double dDeltaTime) override;
	virtual void Render(const HDC& hDC) override;
	
	
private:
	bool LoadTileImage();
	
	
private:
	typedef std::vector<std::vector<GroundInfo>> TileInfoList;
	TileInfoList		m_TileInfoList;

	typedef std::vector <std::weak_ptr<CSprite>> TileSpriteTable;
	TileSpriteTable	m_TileSpriteTable;
};