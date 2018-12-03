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

	auto pResourceMgr = CResourceManager::GetInstance();
	
	m_NumberFontList.emplace_back(NumberFontList());
	m_NumberFontList.emplace_back(NumberFontList());

	m_NumberFontList[0].emplace_back(pResourceMgr->GetWeakSprtiePtr(TEXT("UINumberWhite0")).lock());
	m_NumberFontList[0].emplace_back(pResourceMgr->GetWeakSprtiePtr(TEXT("UINumberWhite1")).lock());
	m_NumberFontList[0].emplace_back(pResourceMgr->GetWeakSprtiePtr(TEXT("UINumberWhite2")).lock());
	m_NumberFontList[0].emplace_back(pResourceMgr->GetWeakSprtiePtr(TEXT("UINumberWhite3")).lock());
	m_NumberFontList[0].emplace_back(pResourceMgr->GetWeakSprtiePtr(TEXT("UINumberWhite4")).lock());
	m_NumberFontList[0].emplace_back(pResourceMgr->GetWeakSprtiePtr(TEXT("UINumberWhite5")).lock());
	m_NumberFontList[0].emplace_back(pResourceMgr->GetWeakSprtiePtr(TEXT("UINumberWhite6")).lock());
	m_NumberFontList[0].emplace_back(pResourceMgr->GetWeakSprtiePtr(TEXT("UINumberWhite7")).lock());
	m_NumberFontList[0].emplace_back(pResourceMgr->GetWeakSprtiePtr(TEXT("UINumberWhite8")).lock());
	m_NumberFontList[0].emplace_back(pResourceMgr->GetWeakSprtiePtr(TEXT("UINumberWhite9")).lock());

	m_NumberFontList[1].emplace_back(pResourceMgr->GetWeakSprtiePtr(TEXT("UINumberYellow0")).lock());
	m_NumberFontList[1].emplace_back(pResourceMgr->GetWeakSprtiePtr(TEXT("UINumberYellow1")).lock());
	m_NumberFontList[1].emplace_back(pResourceMgr->GetWeakSprtiePtr(TEXT("UINumberYellow2")).lock());
	m_NumberFontList[1].emplace_back(pResourceMgr->GetWeakSprtiePtr(TEXT("UINumberYellow3")).lock());
	m_NumberFontList[1].emplace_back(pResourceMgr->GetWeakSprtiePtr(TEXT("UINumberYellow4")).lock());
	m_NumberFontList[1].emplace_back(pResourceMgr->GetWeakSprtiePtr(TEXT("UINumberYellow5")).lock());
	m_NumberFontList[1].emplace_back(pResourceMgr->GetWeakSprtiePtr(TEXT("UINumberYellow6")).lock());
	m_NumberFontList[1].emplace_back(pResourceMgr->GetWeakSprtiePtr(TEXT("UINumberYellow7")).lock());
	m_NumberFontList[1].emplace_back(pResourceMgr->GetWeakSprtiePtr(TEXT("UINumberYellow8")).lock());
	m_NumberFontList[1].emplace_back(pResourceMgr->GetWeakSprtiePtr(TEXT("UINumberYellow9")).lock());


	m_iFontInterval		= m_NumberFontList[0][0].lock()->GetBitWidth();
	m_iFontWidth		= m_NumberFontList[0][0].lock()->GetBitWidth();
	m_iFontHeight		= m_NumberFontList[0][0].lock()->GetBitHeight();
	m_iDigit				= 0;
	m_pValue			= nullptr;
	m_FontType			= FONT_WHITE;

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

		if (*m_pValue == 0)
		{
			pRender->Draw(hDC, POSITION(position.x, position.y), m_NumberFontList[m_FontType][0].lock());
		}
		else
		{
			for (int value = *m_pValue, index = 0; value > 0 && index < m_iDigit; value /= 10)
			{
				pRender->Draw(hDC, POSITION(position.x - m_iFontInterval * index++, position.y), m_NumberFontList[m_FontType][value % 10].lock());
			}
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

void CNumberInterface::SetFontType(FontType type)
{
	m_FontType = type;
}

void CNumberInterface::SetFontInterval(UINT iInteval)
{
	m_iFontInterval = iInteval;
}

void CNumberInterface::SetFontWidth(UINT iWidth)
{
	m_iFontWidth = iWidth;
	GetComponent<ImageRender>().lock()->SetDrawWidth(iWidth);
}

void CNumberInterface::SetFontHeight(UINT iHeight)
{
	m_iFontHeight = iHeight;
	GetComponent<ImageRender>().lock()->SetDrawHeight(iHeight);
}

void CNumberInterface::SetFontSize(UINT iWidth, UINT iHeight)
{
	m_iFontWidth = iWidth;
	m_iFontHeight = iHeight;
	GetComponent<ImageRender>().lock()->SetDrawSize(iWidth, iHeight);
}
