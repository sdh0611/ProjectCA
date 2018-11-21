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
	virtual bool PostInit(const Types::ActorData&, CGameScene*) override;
	virtual void Init() override;
	virtual void Update(double fDeltaTime) override;
	virtual void Render(const HDC& hDC) override;
	virtual void LateUpdate() override;
	

public:
	void DeadProcess(double dDeltaTime);
	void InterruptProecess(double dDeltaTime);
	

public:
	void SetPlayerState(PlayerState state);
	void SetRequestInterrupt(bool bInterrupt);


public:
	PlayerState	GetPlayerState();
	bool			IsDead();
	bool			IsRequestInterrupt();


private:
	void			Attack();
	bool			GenerateFireball();
	void			ChangeAnimationClip(float fCurSpeed, float fWalkSpeed, float fMaxSpeed, float fCurJumpForce);
	virtual void	ActorBehavior(double dDeltaTime) override;


private:
	bool				m_bProtected;
	bool				m_bInterrupt;
	bool				m_bTransforming;
	double			m_dTimeElapsed;
	UINT				m_iSmallStateWidth;
	UINT				m_iSmallStateHeight;
	PlayerState		m_PlayerState;


private:
	typedef std::vector<std::shared_ptr<class CFireball>> FireballPool;
	FireballPool						m_FireballPool;
	UINT								m_iAvailableFireballCount;
	std::weak_ptr<CCamera>	m_pCamera;



};