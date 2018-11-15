#pragma once

#include "..\..\..\stdafx.h"
#include "CActor.h"


class CCamera;

class CPlayer : public CActor {

public:
	enum PlayerState {
		PS_SMALL, PS_BIG, PS_FLOWER
	};

public:
	CPlayer();
	virtual ~CPlayer();


public:
	//virtual bool Init(const Types::ActorData&) override;
	virtual bool PostInit(const Types::ActorData&, CGameScene*) override;
	virtual void Init() override;
	virtual void Update(double fDeltaTime) override;
	virtual void Render(const HDC& hDC) override;


public:
	bool AttachCamera(std::shared_ptr<CCamera> pCamera);
	void SetPlayerState(PlayerState state);
	void SetAttack(bool bAttack);

public:
	bool								IsAttack();
	PlayerState						GetPlayerState();
	std::weak_ptr<CCamera>	GetCamera();


private:
	virtual void ActorBehavior(double dDeltaTime) override;


private:
	std::shared_ptr<CCamera>	m_pCamera;


private:
	bool				m_bAttack;
	PlayerState		m_PlayerState;


};