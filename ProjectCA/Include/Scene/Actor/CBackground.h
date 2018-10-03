#pragma once

#include "..\..\..\stdafx.h"
#include "CActor.h"


class CBackground : public CActor {

public:
	CBackground();
	virtual ~CBackground();


public:
	virtual bool PostInit(const Types::ActorData& data, CGameScene* pScene) override;
	virtual bool Init() override;
	virtual void Update(double dDeltaTIme) override;
	virtual void Render(const HDC& hDC) override;


};