#pragma once

/*
	NOTE:
		Enemy 객체들이 상속받게 될 Class
		
*/


#include "..\..\..\stdafx.h"
#include "CActor.h"



class CEnemy : public CActor {

public:
	CEnemy();
	virtual ~CEnemy();


public:
	//virtual bool Init(const Types::ActorData&) override;
	virtual bool PostInit(const Types::ActorData&, CGameScene*) override;
	virtual bool Init() override;
	virtual void Update(float fDeltaTime) override;
	virtual void Render(const HDC& hDC) override;


protected:
	virtual void ActorBehavior() override;


protected:
	



};