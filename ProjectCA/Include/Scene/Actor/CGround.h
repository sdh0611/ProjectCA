#pragma once

#include "..\..\..\stdafx.h"
#include "CProb.h"

class CSprite;

typedef std::vector<std::vector<std::weak_ptr<CSprite>>> TileInfoList;

class CGround : public CProb {

public:
	virtual bool PostInit(const Types::ActorData&, CGameScene*) override;
	virtual bool Init() override;
	virtual void Update(double dDeltaTime) override;
	virtual void Render(const HDC& hDC) override;
	virtual void ActorBehavior(double dDeltaTime) override;
	
	
private:
	bool LoadTileImage();
	
	
private:
	TileInfoList		m_TileInfoList;


};