#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\UI\CInterface.h"
#include "..\..\..\Include\Core\Components\ImageRender.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"


CInterface::CInterface()
{
}

CInterface::~CInterface()
{
}

bool CInterface::PostInit(const ENTITY_DATA & data, CScene * pScene)
{
	if (!CEntity::PostInit(data, pScene))
		return false;

	GetTransform().lock()->SetPivotRatio(0.5f, 0.5f);

	auto pRender = std::make_shared<ImageRender>();
	if (!pRender->PostInit(this))
		return false;
	pRender->SetDrawSize(m_iEntityWidth, m_iEntityHeight);
	if (!AddComponent(pRender, pRender->GetComponentTag()))
		return false;

	return true;
}

void CInterface::Init()
{
	CEntity::Init();
}

void CInterface::Update(double dDeltaTime)
{
	CEntity::Update(dDeltaTime);
}

void CInterface::Render(const HDC & hDC)
{
	if (m_bActive)
	{
		GetComponent<ImageRender>().lock()->Draw(hDC, GetTransform().lock()->GetPivot());
	}
}

void CInterface::LateUpdate()
{
}

bool CInterface::SetImage(const TSTRING & strImageName)
{
	return GetComponent<ImageRender>().lock()->SetSprite(strImageName);
}
