#pragma once

#include "..\..\..\stdafx.h"
#include "CPickup.h"


class CEndPickup : public CPickup
{
public:
	virtual bool PostInit(const Types::ActorData& data, CGameScene* pScene) override;
	virtual void Render(const HDC& hDC) override;


};