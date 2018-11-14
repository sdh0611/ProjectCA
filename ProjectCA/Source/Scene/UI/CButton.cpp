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

bool CButton::PostInit(const OBJECT_DATA & objectData, CScene * pScene)
{
	if (!CObject::PostInit(objectData, pScene))
		return false;

	auto pRender = std::make_shared<ImageRender>();
	if (!pRender->PostInit(this))
		return false;
	if (!pRender->SetSprite(TEXT("TestButton")))
		return false;
	if (!AddComponent(pRender, pRender->GetComponentTag()))
		return false;

	return true;
}

void CButton::Init()
{
}

void CButton::Update(double dDeltaTime)
{
	if (IsClickOnButton())
	{
		m_OnClick();
	}

	
}

void CButton::Render(const HDC & hDC)
{
	//POSITION position = GetTransform().lock()->GetPosition();
	GetComponent<ImageRender>().lock()->Draw(hDC);
}

void CButton::SetOnClickCallback(Callback callback)
{
	m_OnClick = callback;
}

bool CButton::IsClickOnButton()
{
	POSITION buttonPosition = GetObjectPosition();
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(MainWindow::GetInstance()->GetWindowHandle(), &point);

	if (point.x < buttonPosition.x || point.x >(buttonPosition.x + (float)m_iObjectWidth))
		return false;

	if (point.y < buttonPosition.y || point.y >(buttonPosition.y + (float)m_iObjectHeight))
		return false;

	return CInputManager::GetInstance()->IsMouseDown(TEXT("LBUTTON_DBCLK"));
}
