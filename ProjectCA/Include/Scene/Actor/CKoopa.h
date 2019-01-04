#pragma once

#include "..\..\..\stdafx.h"
#include "CEnemy.h"

//NOTE: CEnemy를 상속받아 작성한 Class.
//			Koopa를 구현함.
//			내부적으로 두가지의 상태값을 가지게됨.

class CKoopa : public CEnemy {

public:
	enum KoopaState {
		KOOPA_IDLE, KOOPA_SHELL
	};

public:
	CKoopa();
	virtual ~CKoopa();


public:
	virtual bool PostInit(const Types::ActorData&, CGameScene*) override;
	virtual void Init() override;
	virtual void Render(const HDC& hDC);


public:
	KoopaState GetKoopaState();


private:
	virtual void ActorBehavior(double dDeltaTime) override;
	virtual void DeadProcess(double dDeltaTime) override;
	virtual void HandlingEvent(EVENT_TYPE type) override;


private:
	KoopaState m_KoopaState;	


};