#pragma once

#include "..\..\..\stdafx.h"
#include "CActor.h"
#include "CFireball.h"

//NOTE: Player객체 구현.
//			내부적으로 3가지의 상태값을 가지고 있음.(Small, Big, Flower)
//			Fireball객체의 생성, 관리 담당

class CCamera;

class CPlayer : public CActor {

	friend void CFireball::SetFireballInactive();

	enum PlayerEvent {
		PE_IDLE, PE_PIPE, PE_DAMAGED, PE_TRANSFORM
	};

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
	void SetPlayerDead();
	void DeadProcess(double dDeltaTime);
	void InterruptProecess(double dDeltaTime);


public:
	void SetRequestInterrupt(bool bInterrupt);
	void SetStoredPickup(std::shared_ptr<CObject> pPickup);


public:
	PlayerState					GetPlayerState();
	bool							IsRequestInterrupt();
	bool							IsPickingObject();
	std::weak_ptr<CObject> GetStoredPickup();


private:
	void			Attack();
	bool			GenerateFireball();
	void			IncreaseAvailableFireballCount();
	void			SetPlayerState(PlayerState state);
	void			ChangeAnimationClip(float fCurSpeed, float fWalkSpeed, float fMaxSpeed, float fCurJumpForce);
	void			PopStoredPickup();
	virtual void	ActorBehavior(double dDeltaTime) override;
	virtual void HandlingEvent(EVENT_TYPE type) override;


private:
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
	class CSoundManager*		m_pSoundManager;


};