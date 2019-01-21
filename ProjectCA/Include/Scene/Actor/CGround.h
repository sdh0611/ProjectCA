#pragma once

#include "..\..\..\stdafx.h"
#include "CProp.h"

//NOTE: Ground를 구현한 Class.
//			타일의 집합체로 이루어져있으며, ObjectType(Ground, Prop)에 따라 타일값이 조금 달라짐
//			생성할 때는 Width와 Height값이 각각 타일의 개수여야 함.
class CSprite;

class CGround : public CProp {
	enum GroundInfo { 
		GI_INNER = 0,
		GI_EDGE_RIGHT_TOP, GI_EDGE_RIGHT_BOT, GI_EDGE_RIGHT_INNER,
		GI_EDGE_LEFT_TOP, GI_EDGE_LEFT_BOT, GI_EDGE_LEFT_INNER,
		GI_LINE_TOP, GI_LINE_LEFT, GI_LINE_BOT, GI_LINE_RIGHT, 
		GI_EDGE_RIGHT_TOP2, GI_EDGE_LEFT_TOP2, GI_LINE_RIGHT2, GI_LINE_LEFT2
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
	CGround();
	virtual ~CGround();


public:
	virtual bool PostInit(const OBJECT_DATA&, CScene*) override;
	virtual void Init() override;
	virtual void Update(double dDeltaTime) override;
	virtual void Render(const HDC& hDC) override;
	virtual void LateUpdate() override;

	
private:
	bool LoadTileImage();
	
	
private:
	typedef std::vector<std::vector<GroundInfo>> TileInfoList;
	TileInfoList		m_TileInfoList;

	typedef std::vector <std::weak_ptr<CSprite>> TileSpriteTable;
	TileSpriteTable	m_TileSpriteTable;
};