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
	virtual void LateUpdate() override;

public:
	void SetPlayerState(PlayerState state);


public:
	PlayerState						GetPlayerState();


private:
	void Attack();
	bool GenerateFireball();
	virtual void ActorBehavior(double dDeltaTime) override;


private:
	UINT				m_iSmallStateWidth;
	UINT				m_iSmallStateHeight;
	PlayerState		m_PlayerState;


private:
	typedef std::vector<std::shared_ptr<class CFireball>> FireballPool;
	FireballPool						m_FireballPool;
	UINT								m_iAvailableFireballCount;
	std::weak_ptr<CCamera>	m_pCamera;



};