#pragma once

#include "..\..\..\stdafx.h"
#include "CPickup.h"

//NOTE: CPickup을 상속받아 작성한 Pickup Class
//			먹으면 게임이 끝나게된다.

class CEndPickup : public CPickup
{
public:
	virtual bool PostInit(const Types::ActorData& data, CGameScene* pScene) override;
	virtual void Render(const HDC& hDC) override;
	virtual void LateUpdate() override;


};