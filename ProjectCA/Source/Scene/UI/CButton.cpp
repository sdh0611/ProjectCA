#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\UI\CButton.h"
#include "..\..\..\Include\Core\Components\ImageRender.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\CResourceManager.h"
#include "..\..\..\Include\Core\CInputManager.h"
#include "..\..\..\Include\Core\Window\MainWindow.h"



CButton::CButton()
	: m_OnClick(nullptr)
{
}

CButton::~CButton()
{
	puts("Button Destroy");
}

bool CButton::PostInit(const ENTITY_DATA & objectData, CScene * pScene)
{
	if (!CInterface::PostInit(objectData, pScene))
		return false;

	//GetTransform().lock()->SetPivotRatio(0.5f, 1.f);

	return true;

}

void CButton::Update(double dDeltaTime)
{
	if (m_bActive)
	{
		CInterface::Update(dDeltaTime);

		if (IsClickOnButton())
		{
			m_OnClick();
		}
	}
	
}

//void CButton::Render(const HDC & hDC)
//{
//	if (m_bActive)
//	{
//		POSITION position = GetEntityPosition();
//		Rectangle(hDC, position.x - m_iEntityWidth * 0.5, position.y - m_iEntityHeight * 0.5f,
//			position.x + m_iEntityWidth * 0.5, position.y + m_iEntityHeight * 0.5);
//		GetComponent<ImageRender>().lock()->Draw(hDC);
//	}
//}

void CButton::SetOnClickCallback(Callback callback)
{
	m_OnClick = callback;
}

Callback CButton::GetOnClick() const
{
	return m_OnClick;
}


bool CButton::IsClickOnButton()
{
	POSITION buttonPosition = GetTransform().lock()->GetPivot();
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(MainWindow::GetInstance()->GetWindowHandle(), &point);

	if (point.x < buttonPosition.x || point.x >(buttonPosition.x + (float)m_iEntityWidth))
		return false;

	if (point.y < buttonPosition.y || point.y >(buttonPosition.y + (float)m_iEntityHeight))
		return false;

	return CInputManager::GetInstance()->IsMouseDown(TEXT("LBUTTON_DBCLK"));
}
