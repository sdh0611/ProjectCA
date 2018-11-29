#pragma once

#include "..\..\..\stdafx.h"
#include "CActor.h"



class CPickup : public CActor 
{
public:
	enum PickupType {
		PT_MUSHROOM_RED, PT_MUSHROOM_GREEN, PT_FLOWER, PT_COIN
	};


public:
	CPickup();
	virtual ~CPickup();


public:
	virtual bool PostInit(const Types::ActorData& data, CGameScene* pScene) override;
	virtual void Init() override;
	virtual void Update(double dDeltaTime) override;
	virtual void Render(const HDC& hDC) override = 0 ;


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