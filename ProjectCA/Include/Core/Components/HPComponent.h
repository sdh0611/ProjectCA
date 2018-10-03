#pragma once

#include "..\..\..\stdafx.h"
#include "ComponentBase.h"


class HPComponent : public ComponentBase {

public:
	HPComponent();
	virtual ~HPComponent();


public:
	bool Init(std::shared_ptr<CActor> pOwner, float fHP, float fMaxHP, 
		const Types::tstring& strTag = TEXT("HPComponent"));
	virtual void Update(double fDeltaTime) override;

	
public:
	void IncreaseHP(float fHeal);
	void DecreaseHP(float fDamage);

	
public:
	bool IsDead() const { return m_bIsDead; }
	float GetHP() const {	return m_fHP; }
	float GetMaxHP() const { return m_fMaxHP; }


private:
	bool		m_bIsDead;
	float		m_fHP;
	float		m_fMaxHP;


};