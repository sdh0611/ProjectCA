#pragma once

#include "..\..\..\stdafx.h"
#include "CActor.h"


class CBomb : public CActor {

public:
	CBomb();
	virtual ~CBomb();


public:
	virtual bool PostInit(const Types::ActorData&, CGameScene*) override;
	virtual bool Init() override;
	virtual void Update(double fDeltaTime) override;
	virtual void Render(const HDC& hDC) override;
	virtual void Destroy() override;
	virtual void ActorBehavior(double dDeltaTime) override;

public:
	virtual void SetOwnerActor(std::shared_ptr<CActor> pActor) { m_pOwnerActor = pActor; }
	std::weak_ptr<CActor> GetOwnerActor() const { return m_pOwnerActor; }



private:



private:
	float m_fDamage;
	std::weak_ptr<CActor> m_pOwnerActor;

};