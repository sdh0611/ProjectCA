#pragma once

#include "..\..\..\stdafx.h"
#include "..\..\Scene\CEntity.hpp"

//NOTE: Particle들이 상속받게 되는 클래스

class CScene;

class CParticle : public CEntity
{
public:
	CParticle();
	virtual ~CParticle();


public:
	virtual bool PostInit(const ENTITY_DATA& data, CScene* pScene) override;
	virtual void Init() override;
	virtual void Update(double dDeltaTime);


public:
	void SetOwnerEntity(CEntity* pOwner);
	void SetPlayTime(double dPlayTime);
	void SetXSpeed(float fSpeed);
	void SetYSpeed(float fSpeed);


public:
	bool				IsParticleEnd();
	CEntity * const GetOwnerEntity() const;


protected:
	virtual void ParticleUpdate(double dDeltaTime) = 0;


protected:
	float		m_fXSpeed;
	float		m_fYSpeed;
	double	m_dTimeElapsed;
	double	m_dPlayTime;
	CEntity*	m_pOwnerEntity;

};