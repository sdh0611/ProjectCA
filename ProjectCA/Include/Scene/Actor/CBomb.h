#pragma once

#include "..\..\..\stdafx.h"
#include "CActor.h"


class CBomb : public CActor {

public:
	CBomb();
	virtual ~CBomb();


public:
	virtual bool Init(const Types::ActorData&) override;
	virtual void Update(float fDeltaTime) override;
	virtual void Render(const HDC& hDC) override;
	virtual void Destroy() override;


public:
	virtual void SetOwnerActor(std::shared_ptr<CActor> pActor) { m_pOwnerActor = pActor; }
	std::weak_ptr<CActor> GetOwnerActor() const { return m_pOwnerActor; }



private:



private:
	float m_fDamage;
	std::weak_ptr<CActor> m_pOwnerActor;

};