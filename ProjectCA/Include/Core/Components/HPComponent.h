#pragma once

#include "..\..\..\stdafx.h"
#include "ComponentBase.h"

class CObject;


class HPComponent : public ComponentBase {

public:
	HPComponent();
	virtual ~HPComponent();


public:
	bool PostInit(CObject* pOwner, float fHP, float fMaxHP, int iLifeCount,
		const TSTRING& strTag = TEXT("HPComponent"));
	virtual void Init();
	virtual void Update(double dDeltaTime) override;
	virtual void LateUpdate(double dDeltaTime) override;

	
public:
	void SetDead(bool bDead);
	void IncreaseHP(float fHeal);
	void DecreaseHP(float fDamage);
	void IncreaseLife(UINT iValue);
	void DecreaseLife(UINT iValue);

	
public:
	bool	IsDead() const { return m_bDead; }
	float	GetHP() const {	return m_fHP; }
	float	GetMaxHP() const { return m_fMaxHP; }
	int		GetLifeCount() const { return m_iLife; }


private:
	bool		m_bDead;
	int			m_iLife;
	int			m_iInitialLife;
	float		m_fHP;
	float		m_fMaxHP;

};