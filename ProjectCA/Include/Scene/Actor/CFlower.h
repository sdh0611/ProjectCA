#pragma once

#include "..\..\..\stdafx.h"
#include "CActor.h"


class CFlower : public CActor {

public:
	CFlower();
	virtual ~CFlower();


public:
	virtual bool PostInit(const Types::ActorData& data, CGameScene* pScene) override;
	virtual bool Init() override;
	virtual void Update(double dDeltaTime);
	virtual void Render(const HDC& hDC);


};