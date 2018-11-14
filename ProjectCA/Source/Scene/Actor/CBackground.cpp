#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CBackground.h"
#include "..\..\..\Include\Scene\CGameScene.h"
#include "..\..\..\Include\Scene\CCameraManager.h"
#include "..\..\..\Include\Scene\Actor\CCamera.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\Components\ImageRender.h"
#include "..\..\..\Include\Core\CResourceManager.h"
#include "..\..\..\Include\Core\Graphic\CSprite.h"


CBackground::CBackground()
{
}

CBackground::~CBackground()
{
}

bool CBackground::PostInit(const Types::ObjectData & data, CScene * pScene)
{
	if (!CObject::PostInit(data, pScene))
	{
		return false;
	}

	//ImageRender 추가
	std::shared_ptr<ImageRender> pRender = std::make_shared<ImageRender>();
	if (!pRender->PostInit(this))
		return false;

	if (!AddComponent(pRender, pRender->GetComponentTag()))
		return false;

	return true;;
}

void CBackground::Init()
{
}

void CBackground::Update(double dDeltaTIme)
{
	for (auto& it : m_ComponentTable)
		it.second->Update(dDeltaTIme);

}

void CBackground::Render(const HDC & hDC)
{
	auto pTransform = GetComponent<TransformComponent>().lock();
	POSITION screenPosition = pTransform->GetScreenPosition();
	UINT iCameraWidth = CCameraManager::GetInstance()->GetMainCamera().lock()->GetCameraWidth();

	auto pRender = GetComponent<ImageRender>().lock();
	
	//카메라 좌측 맵출력
	pRender->Draw(hDC, POSITION(screenPosition.x - m_iObjectWidth, screenPosition.y));
	//카메라 출력부분
	pRender->Draw(hDC, screenPosition);

	if (screenPosition.x > iCameraWidth)
	{
		pTransform->Move(-1.f * iCameraWidth, 0);
	}
	else if(screenPosition.x < 0.f)
	{
		pTransform->Move(2.f * iCameraWidth, 0);
	}


	//SelectObject(memDC, hOldBit);
	//DeleteDC(memDC);

}


bool CBackground::SetBackgroundImage(const TSTRING & strImageName)
{

	if (!GetComponent<ImageRender>().lock()->SetSprite(strImageName))
	{
		return false;
	}
	//m_iBackgroundWidth = m_pBackgroundImage.lock()->GetBitWidth();
	//m_iBackgroundHeight = m_pBackgroundImage.lock()->GetBitHeight();

	return true;
}

