#pragma once

#include "..\..\..\stdafx.h"
#include "CActor.h"
#include "CFireball.h"
//#include "..\..\Core\Components\PlayerInputComponent.h"

class CCamera;

class CPlayer : public CActor {

	friend void CFireball::SetFireballInactive();

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
	void SetPlayerDead();
	void DeadProcess(double dDeltaTime);
	void InterruptProecess(double dDeltaTime);


public:
	void SetRequestInterrupt(bool bInterrupt);
	void SetStoredPickup(std::shared_ptr<CObject> pPickup);


public:
	PlayerState	GetPlayerState();
	bool			IsDead();
	bool			IsRequestInterrupt();
	std::weak_ptr<CObject> GetStoredPickup();


private:
	void			Attack();
	bool			GenerateFireball();
	void			IncreaseAvailableFireballCount();
	void			SetPlayerState(PlayerState state);
	void			ChangeAnimationClip(float fCurSpeed, float fWalkSpeed, float fMaxSpeed, float fCurJumpForce);
	void			PopStoredPickup();
	virtual void	ActorBehavior(double dDeltaTime) override;


private:
	bool							m_bProtected;
	bool							m_bInterrupt;
	bool							m_bTransforming;
	double						m_dTimeElapsed;
	UINT							m_iSmallStateWidth;
	UINT							m_iSmallStateHeight;
	PlayerState					m_PlayerState;
	std::weak_ptr<CObject>	m_pCurPickupPtr;
	std::weak_ptr<CObject>	m_pStoredPickupPtr;
	std::weak_ptr<CObject> m_pPickObjectPtr;


private:
	typedef std::vector<std::shared_ptr<class CFireball>> FireballPool;
	FireballPool						m_FireballPool;
	UINT								m_iAvailableFireballCount;
	std::weak_ptr<CCamera>	m_pCamera;



};