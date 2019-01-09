#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Core\Graphic\CParticle.h"
#include "..\..\..\Include\Core\Components\AnimationRender.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"


CParticle::CParticle()
{
}

CParticle::~CParticle()
{
}

bool CParticle::PostInit(const ENTITY_DATA & data, CScene * pScene)
{
	if (!CEntity::PostInit(data, pScene))
	{
		return false;
	}
	GetComponent<TransformComponent>().lock()->SetPivotRatio(0.5f, 1.f);

	auto pRender = std::make_shared<AnimationRender>();
	if (!pRender->PostInit(this))
	{
		return false;
	}
	if (!AddComponent(pRender, pRender->GetComponentTag()))
	{
		return false;
	}
	pRender->SetExpansionRatio(2.5f);
	pRender->SetPivotRatio(0.5f, 1.f);

	m_fXSpeed			= 0.f;
	m_fYSpeed			= 0.f;
	m_dTimeElapsed	= 0.f;
	m_dPlayTime		= 0.f;


	return true;
}

void CParticle::Init()
{
	m_bActive = false;
	for (const auto& component : m_ComponentTable)
	{
		component.second->Init();
	}

	m_dTimeElapsed = 0.f;
}

void CParticle::Update(double dDeltaTime)
{
	if (m_bActive)
	{
		CEntity::Update(dDeltaTime);
		ParticleUpdate(dDeltaTime);
		m_dTimeElapsed += dDeltaTime;
		if (IsParticleEnd())
		{
			SetActive(false);
		}
	}
}

void CParticle::SetOwnerEntity(CEntity * pOwner)
{
	m_pOwnerEntity = pOwner;
}

void CParticle::SetPlayTime(double dPlayTime)
{
	if (dPlayTime < 0.f)
		return;

	m_dPlayTime = dPlayTime;
}

void CParticle::SetXSpeed(float fSpeed)
{
	m_fXSpeed = fSpeed;
}

void CParticle::SetYSpeed(float fSpeed)
{
	m_fYSpeed = fSpeed;
}

bool CParticle::IsParticleEnd()
{
	return m_dTimeElapsed > m_dPlayTime ? true : false;
}

CEntity * const CParticle::GetOwnerEntity() const
{
	return m_pOwnerEntity;
}
