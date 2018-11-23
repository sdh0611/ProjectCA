#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\UI\CNumberInterface.h"
#include "..\..\..\Include\Core\Components\ImageRender.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\CResourceManager.h"
#include "..\..\..\Include\Core\Graphic\CSprite.h"



CNumberInterface::CNumberInterface()
{
}

CNumberInterface::~CNumberInterface()
{
}

bool CNumberInterface::PostInit(const OBJECT_DATA & data, CScene * pScene)
{
	if (!CInterface::PostInit(data, pScene))
		return false;

	GetTransform().lock()->SetPivotRatio(0.f, 0.5f);

	auto pResourceMgr = CResourceManager::GetInstance();

	m_NumberFontList.emplace_back(pResourceMgr->GetWeakSprtiePtr(TEXT("UINumber0")).lock());
	m_NumberFontList.emplace_back(pResourceMgr->GetWeakSprtiePtr(TEXT("UINumber1")).lock());
	m_NumberFontList.emplace_back(pResourceMgr->GetWeakSprtiePtr(TEXT("UINumber2")).lock());
	m_NumberFontList.emplace_back(pResourceMgr->GetWeakSprtiePtr(TEXT("UINumber3")).lock());
	m_NumberFontList.emplace_back(pResourceMgr->GetWeakSprtiePtr(TEXT("UINumber4")).lock());
	m_NumberFontList.emplace_back(pResourceMgr->GetWeakSprtiePtr(TEXT("UINumber5")).lock());
	m_NumberFontList.emplace_back(pResourceMgr->GetWeakSprtiePtr(TEXT("UINumber6")).lock());
	m_NumberFontList.emplace_back(pResourceMgr->GetWeakSprtiePtr(TEXT("UINumber7")).lock());
	m_NumberFontList.emplace_back(pResourceMgr->GetWeakSprtiePtr(TEXT("UINumber8")).lock());
	m_NumberFontList.emplace_back(pResourceMgr->GetWeakSprtiePtr(TEXT("UINumber9")).lock());

	m_iFontWidth = m_NumberFontList[0].lock()->GetBitWidth() * 2;
	m_iDigit = 0;
	m_pValue = nullptr;

	return true;
}

void CNumberInterface::Init()
{
	CInterface::Init();
}

void CNumberInterface::Update(double dDeltaTime)
{
	CInterface::Update(dDeltaTime);

}

void CNumberInterface::Render(const HDC & hDC)
{
	if (m_bActive)
	{
		auto pRender = GetComponent<ImageRender>().lock();
		auto position = GetTransform().lock()->GetPivot();
		//for (int index = 0; index < m_iDigit; ++index)
		//{
		//	pRender->Draw(hDC, POSITION(position.x - m_iFontWidth * index, position.y), m_NumberFontList[index].lock());
		//}

		//auto position();
		for (int value = *m_pValue, index = 0; value > 0; value /= 10)
		{
			
			pRender->Draw(hDC, POSITION(position.x - m_iFontWidth * index++, position.y), m_NumberFontList[value%10].lock());

		}

	}

}

void CNumberInterface::LinkValuePtr(int * pValue)
{
	m_pValue = pValue;
}

void CNumberInterface::SetDigit(int iDigit)
{
	if (iDigit < 0)
		return;

	m_iDigit = iDigit;
}
