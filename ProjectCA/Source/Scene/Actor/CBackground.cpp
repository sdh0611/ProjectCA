#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CBackground.h"
#include "..\..\..\Include\Scene\CGameScene.h"
#include "..\..\..\Include\Scene\CCameraManager.h"
#include "..\..\..\Include\Scene\Actor\CCamera.h"
#include "..\..\..\Include\Scene\Actor\CPlayer.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Core\Components\ImageRender.h"
#include "..\..\..\Include\Core\CResourceManager.h"
#include "..\..\..\Include\Scene\CCameraManager.h"
#include "..\..\..\Include\Scene\Actor\CCamera.h"
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
	auto pRender = std::make_shared<ImageRender>();
	if (!pRender->PostInit(this))
		return false;
	pRender->SetDrawSize(MAX_WIDTH, MAX_HEIGHT);
	if (!AddComponent(pRender, pRender->GetComponentTag()))
		return false;

	m_bStatic = false;
	m_fScrollSpeed = 100.f;

	return true;;
}

void CBackground::Init()
{
	CObject::Init();
}

void CBackground::Update(double dDeltaTIme)
{
	for (auto& it : m_ComponentTable)
		it.second->Update(dDeltaTIme);

	if (!m_bStatic)
	{
		float fSpeed = CCameraManager::GetInstance()->GetMainCamera().lock()->GetCameraCurSpeed();
		if (fSpeed)
		{
			GetTransform().lock()->Move(-1 * fSpeed * 0.2f* dDeltaTIme, 0);
		}

	}

}

void CBackground::Render(const HDC & hDC)
{
	auto pTransform = GetComponent<TransformComponent>().lock();
	auto pRender = GetComponent<ImageRender>().lock();
	POSITION screenPosition = pTransform->GetScreenPosition();

	if (m_bStatic)
	{
		pRender->Draw(hDC, GetObjectPosition());
	}
	else
	{
		UINT iCameraWidth = CCameraManager::GetInstance()->GetMainCamera().lock()->GetCameraWidth();
		POSITION position = GetObjectPosition();
		//카메라 좌측 맵출력
		pRender->Draw(hDC, POSITION(position.x - m_iObjectWidth, position.y));
		//카메라 출력부분
		pRender->Draw(hDC, position);

		if (position.x > iCameraWidth)
		{
			pTransform->Move(-1.f * iCameraWidth, 0);
		}
		else if (position.x < 0.f)
		{
			pTransform->Move(2.f * iCameraWidth, 0);
		}

	}

}


bool CBackground::SetBackgroundImage(const TSTRING & strImageName)
{

	if (!GetComponent<ImageRender>().lock()->SetSprite(strImageName))
	{
		return false;
	}

	return true;
}

void CBackground::SetStatic(bool bStatic)
{
	m_bStatic = bStatic;
}

void CBackground::SetScrollSpeed(float fSpeed)
{
	m_fScrollSpeed = fSpeed;
}

bool CBackground::IsStatic()
{
	return m_bStatic;
}

