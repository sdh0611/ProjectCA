#pragma once

#include "..\..\..\stdafx.h"
#include "CActor.h"

//NOTE: 게임 내 Pickup들이 상속받게 될 추상클래스.
//			PickupType을 내부 변수 값으로 갖고 있고, 
//			각각 습득했을 때 얻게되는 점수 값이 있음.

class CPickup : public CActor 
{
public:
	enum PickupType {
		PT_MUSHROOM_RED, PT_FLOWER, PT_COIN
	};


public:
	CPickup();
	virtual ~CPickup();


public:
	virtual bool PostInit(const Types::ActorData& data, CGameScene* pScene) override;
	virtual void Init() override;
	virtual void Update(double dDeltaTime) override;
	virtual void Render(const HDC& hDC) override = 0 ;
	virtual void LateUpdate() override;


public:
	void			SetStored();
	bool			IsStored() const;
	PickupType GetPickupType() const;


private:
	virtual void ActorBehavior(double dDeltaTime) override;


protected:
	bool			m_bStored;
	int				m_iScore;
	PickupType	m_Type;

};