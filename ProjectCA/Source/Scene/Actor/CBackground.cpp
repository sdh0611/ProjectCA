#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CBackground.h"
#include "..\..\..\Include\Scene\CGameScene.h"
#include "..\..\..\Include\Scene\CCameraManager.h"
#include "..\..\..\Include\Scene\Actor\CCamera.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\CResourceManager.h"
#include "..\..\..\Include\Core\Graphic\CSprite.h"


CBackground::CBackground()
{
}

CBackground::~CBackground()
{
}

bool CBackground::PostInit(const Types::ActorData & data, CGameScene * pScene)
{
	if (data.iActorWidth > MAX_ACTOR_SIZE || data.iActorHeight > MAX_ACTOR_SIZE)
		return false;

	if (data.actorPoint.x < 0 || data.actorPoint.x > MAX_WIDTH ||
		data.actorPoint.y < 0 || data.actorPoint.y > MAX_HEIGHT)
		return false;

	m_iActorWidth				= data.iActorWidth;
	m_iActorHeight				= data.iActorHeight;
	//m_actorPoint = m_spawnPoint = data.actorPoint;
	m_actorType				= data.actorType;
	m_actorCurState			= data.actorState;
	m_actorCurVerticalState	= Types::VS_IDLE;
	m_actorHorizonalState	= Types::HS_IDLE;
	m_direction					= data.direction;
	m_actorVector				= data.vector;
	m_actorID					= data.actorID;
	m_strActorTag				= data.strActorTag;
	m_pOwnerScene			= pScene;
	m_bActive					= data.bActive;

	m_fScrollSpeed				= 15.f;
	m_ColorRef					= RGB(248, 7, 220);


	//TransformComponent 추가
	TransformComponent* pTransform = new TransformComponent;
	if (!pTransform->PostInit(this, data.actorPoint))
		return false;

	if (!AddComponent(pTransform, pTransform->GetComponentTag()))
		return false;


	return true;;
}

bool CBackground::Init()
{
	return true;
}

void CBackground::Update(double dDeltaTIme)
{
	for (auto& it : m_componentTable)
		it.second->Update(dDeltaTIme);


}

void CBackground::Render(const HDC & hDC)
{
	HDC memDC = CreateCompatibleDC(hDC);
	HBITMAP hOldBit = (HBITMAP)SelectObject(memDC, m_pBackgroundImage.lock()->GetBitmap());
	TransformComponent* pTransform = GetComponent<TransformComponent>();
	POSITION screenPosition = pTransform->GetScreenPosition();
	UINT iCameraWidth = CCameraManager::GetInstance()->GetMainCamera().lock()->GetCameraWidth();

	//카메라 좌측 맵출력
	TransparentBlt(hDC, screenPosition.x - m_iActorWidth, screenPosition.y,
		m_iActorWidth, m_iActorHeight, memDC, 0, 0,
		m_iBackgroundWidth, m_iBackgroundHeight, m_ColorRef);

	//카메라 출력부분
	TransparentBlt(hDC, screenPosition.x, screenPosition.y,
		m_iActorWidth, m_iActorHeight, memDC, 0, 0,
		m_iBackgroundWidth, m_iBackgroundHeight, m_ColorRef);

	if (screenPosition.x > iCameraWidth)
	{
		pTransform->Move(-1.f * iCameraWidth, 0);
	}
	else if(screenPosition.x < 0.f)
	{
		pTransform->Move(2.f * iCameraWidth, 0);
	}


	SelectObject(memDC, hOldBit);
	DeleteDC(memDC);

}

void CBackground::ActorBehavior(double dDeltaTime)
{
}

bool CBackground::SetBackgroundImage(const TSTRING & strImageName)
{
	m_pBackgroundImage = CResourceManager::GetInstance()->GetWeakSprtiePtr(TEXT("BackgroundMountain2"));
	if (m_pBackgroundImage.expired())
	{
		return false;
	}
	m_iBackgroundWidth = m_pBackgroundImage.lock()->GetBitWidth();
	m_iBackgroundHeight = m_pBackgroundImage.lock()->GetBitHeight();

	return true;
}

