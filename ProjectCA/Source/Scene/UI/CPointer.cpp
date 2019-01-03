#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\UI\CPointer.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\CInputManager.h"

CPointer::CPointer()
{
}

CPointer::~CPointer()
{
}

void CPointer::Update(double dDeltaTime)
{
	if (m_bActive)
	{
		CUI::Update(dDeltaTime);
		Behavior(dDeltaTime);
	}
}

void CPointer::SetCurFocusButton(std::shared_ptr<CButton> pButton)
{
	m_pCurFocusButton = pButton;
}

void CPointer::Behavior(double dDelatTime)
{
	auto pInput = CInputManager::GetInstance();
	


}
