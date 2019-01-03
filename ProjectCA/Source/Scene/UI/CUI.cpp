#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\UI\CUI.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\Components\ImageRender.h"


CUI::CUI()
{
}

CUI::~CUI()
{
}

bool CUI::PostInit(const ENTITY_DATA & data, CScene * pScene)
{
	if (!CEntity::PostInit(data, pScene))
	{
		return false;
	}

	GetTransform().lock()->SetPivotRatio(0.5f, 0.5f);

	auto pRender = std::make_shared<ImageRender>();
	if (!pRender->PostInit(this))
		return false;
	pRender->SetDrawSize(m_iEntityWidth, m_iEntityHeight);
	if (!AddComponent(pRender, pRender->GetComponentTag()))
		return false;

	return true;
}

void CUI::Render(const HDC & hDC)
{
	if (m_bActive)
	{
		GetComponent<ImageRender>().lock()->Draw(hDC, GetTransform().lock()->GetPivot());
	}
}

void CUI::LateUpdate()
{
}
