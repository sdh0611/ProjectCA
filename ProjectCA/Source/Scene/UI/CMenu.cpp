#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\UI\CMenu.h"
#include "..\..\..\Include\Scene\UI\CButton.h"
#include "..\..\..\Include\Core\Components\ImageRender.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\CResourceManager.h"
#include "..\..\..\Include\Core\CInputManager.h"
#include "..\..\..\Include\Core\Sound\CSoundManager.h"
#include "..\..\..\Include\Core\Window\MainWindow.h"



CMenu::CMenu()
{
}

CMenu::~CMenu()
{
	m_ButtonList.clear();
}

bool CMenu::PostInit(const ENTITY_DATA & data, CScene * pScene)
{
	if (!CUI::PostInit(data, pScene))
		return false;

	m_iCurFocusButtonIndex		= 0;
	m_iNumberOfButton			= 0;
	m_fInterval						= 35.f;

	return true;
}

void CMenu::Update(double dDeltaTime)
{
	if (m_bActive)
	{
		CUI::Update(dDeltaTime);
		if (m_iNumberOfButton > 0)
		{
			MenuBehavior();
		}
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
	++m_iNumberOfButton;

	//넣은 버튼에 대한 Menu 박스의 크기와 버튼 좌표 조정
	{
		UINT iButtonWidth = pButton->GetEntityWidth();
		UINT iButtonHeight = pButton->GetEntityHeight();

		if (iButtonWidth > m_iEntityWidth)
		{
			SetEntityWidth(iButtonWidth);
		}

		if (iButtonHeight > m_iEntityHeight)
		{
			//버튼 사이의 간격을 위해 EntityHeight에 약간의 상수를 곱해줌.
			SetEntityHeight(1.2f * m_iEntityHeight + iButtonHeight);
		}

		LiningButton();
	}

	if (m_iNumberOfButton == 1)
	{
		if (m_pFocusPointer)
		{
			POSITION position = m_ButtonList[m_iNumberOfButton-1]->GetEntityPosition();
			m_pFocusPointer->SetEntityPosition(position.x - m_ButtonList[m_iNumberOfButton-1]->GetEntityWidth() / 2.f - m_pFocusPointer->GetEntityWidth() / 2.f, position.y);
		}
	}
}

void CMenu::DeleteButton(std::shared_ptr<CButton> pButton)
{
	for (auto it = m_ButtonList.cbegin(); it != m_ButtonList.cend();)
	{
		if (pButton == *it)
		{
			m_ButtonList.erase(it);
			--m_iNumberOfButton;
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

	if (m_iNumberOfButton > 0)
	{
		m_pFocusPointer->SetEntityPosition(m_ButtonList[0]->GetEntityPosition().x - m_ButtonList[0]->GetEntityWidth() * 0.5f - m_pFocusPointer->GetEntityWidth() * 0.5f,
			m_ButtonList[0]->GetEntityPosition().y);
	}
}

void CMenu::SetInterval(float fInterval)
{
	if (fInterval < 0.f)
		return;

	m_fInterval = fInterval;

}

void CMenu::SetFocusPointerImage(const TSTRING & strImageName)
{
	if (m_pFocusPointer)
	{
		m_pFocusPointer->SetImage(strImageName);
	}
}

void CMenu::MenuBehavior()
{
	auto pInput = CInputManager::GetInstance();
	POSITION position;
	if (pInput->IsKeyDown(VK_UP))
	{
		m_iCurFocusButtonIndex = ++m_iCurFocusButtonIndex % m_iNumberOfButton;
		position = m_ButtonList[m_iCurFocusButtonIndex]->GetEntityPosition();
		m_pFocusPointer->SetEntityPosition(position.x - m_ButtonList[m_iCurFocusButtonIndex]->GetEntityWidth() * 0.5f - m_pFocusPointer->GetEntityWidth() * 0.5f, position.y);
		CSoundManager::GetInstance()->SoundPlay(TEXT("SFXMovePointer"));
	}
	else if (pInput->IsKeyDown(VK_DOWN))
	{
		if (m_iCurFocusButtonIndex == 0)
		{
			m_iCurFocusButtonIndex = m_iNumberOfButton - 1;
		}
		else
		{
			--m_iCurFocusButtonIndex;
		}
		position = m_ButtonList[m_iCurFocusButtonIndex]->GetEntityPosition();
		m_pFocusPointer->SetEntityPosition(position.x - m_ButtonList[m_iCurFocusButtonIndex]->GetEntityWidth() * 0.5f - m_pFocusPointer->GetEntityWidth() * 0.5f, position.y);
		CSoundManager::GetInstance()->SoundPlay(TEXT("SFXMovePointer"));
	}
	else if (pInput->IsKeyDown(VK_RETURN))
	{
		//Click상태로 설정.
		m_ButtonList[m_iCurFocusButtonIndex]->GetOnClick()();
	}

}

void CMenu::LiningButton()
{
	int iIndex = 1;
	POSITION position = GetEntityPosition();
	float fLength = position.y - m_iEntityHeight / 2.f + m_fInterval;

	for (const auto& button : m_ButtonList)
	{
		button->GetTransform().lock()->SetPosition(position.x, fLength + button->GetEntityHeight()/2.f);
		fLength += button->GetEntityHeight() / 2.f + m_fInterval;
	}

}
