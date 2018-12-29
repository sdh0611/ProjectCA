#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\UI\CMenu.h"
#include "..\..\..\Include\Scene\UI\CButton.h"
#include "..\..\..\Include\Core\Components\ImageRender.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\CResourceManager.h"
#include "..\..\..\Include\Core\CInputManager.h"
#include "..\..\..\Include\Core\Window\MainWindow.h"



CMenu::CMenu()
{
}

CMenu::~CMenu()
{
}

bool CMenu::PostInit(const ENTITY_DATA & data, CScene * pScene)
{
	if (!CEntity::PostInit(data, pScene))
		return false;



	return true;
}

void CMenu::Init()
{
	

}

void CMenu::Update(double dDeltaTime)
{
	if (m_bActive)
	{
		CInterface::Update(dDeltaTime);
		for (const auto& button : m_ButtonList)
		{
			if (button->IsActive())
			{
				button->Update(dDeltaTime);
			}
		}

		if (m_pFocusPointer)
		{
			m_pFocusPointer->Update(dDeltaTime);
		}
	}
}

void CMenu::Render(const HDC & hDC)
{
	if (m_bActive)
	{
		for (const auto& button : m_ButtonList)
		{
			if (button->IsActive())
			{
				button->Render(hDC);
			}
		}

		if (m_pFocusPointer)
		{
			m_pFocusPointer->Render(hDC);
		}
	}
}

void CMenu::LateUpdate()
{
}

void CMenu::AddButton(std::shared_ptr<CButton> pButton)
{
	if (pButton == nullptr)
		return;

	m_ButtonList.emplace_back(pButton);

}

void CMenu::DeleteButton(std::shared_ptr<CButton> pButton)
{
	for (auto it = m_ButtonList.cbegin(); it != m_ButtonList.cend();)
	{
		if (pButton == *it)
		{
			m_ButtonList.erase(it);
			return;
		}
		else
		{
			++it;
		}
	}

}

void CMenu::SetFocusPointer(std::shared_ptr<CInterface> pFocus)
{
	m_pFocusPointer = pFocus;

}

void CMenu::SetFocusPointerImage(const TSTRING & strImageName)
{
	m_pFocusPointer->SetImage(strImageName);
}

void CMenu::MenuBehavior()
{
}
