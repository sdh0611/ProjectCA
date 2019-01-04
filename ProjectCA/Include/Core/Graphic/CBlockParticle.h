#pragma once

#include "..\..\..\stdafx.h"
#include "CParticle.h"

//NOTE: CParticle을 상속받은 Particle객체.
//			Block이 파괴될 때 나오는 Particle

class CBlockParticle : public CParticle
{
public:
	CBlockParticle();
	virtual ~CBlockParticle();


public:
	virtual bool PostInit(const ENTITY_DATA& data, CScene* pScene) override;
	virtual void Init() override;
	virtual void Update(double dDeltaTime) override;
	virtual void Render(const HDC& hDC) override;
	virtual void LateUpdate() override;


private:
	bool GenerateBlockPeace();
	virtual void ParticleUpdate(double dDeltaTime) override;


private:
	std::vector<std::shared_ptr<CEntity>> m_BlockPeaceList;

};