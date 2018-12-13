#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CEndPickup.h"
#include "..\..\..\Include\Scene\Actor\CPlayer.h"
#include "..\..\..\Include\Scene\CGameScene.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Core\Components\ColliderBox.h"
#include "..\..\..\Include\Core\Components\ImageRender.h"

bool CEndPickup::PostInit(const Types::ActorData & data, CGameScene * pScene)
{
	if (!CPickup::PostInit(data, pScene))
		return false;

	//BoxCollider 추가
	auto pCollider = std::make_shared<ColliderBox>();
	if (!pCollider->PostInit(this))
		return false;
	auto colliderCallback = [&](CObject* pOther, Collider::CollisionType type, float fIntersectLength)->void {

		if (pOther->GetObjectType() == Types::OT_PLAYER)
		{
			static_cast<CGameScene*>(m_pOwnerScene)->SetIsGameClear();
		}
	};
	pCollider->SetOnCollision(colliderCallback);
	if (!AddComponent(pCollider, pCollider->GetComponentTag()))
		return false;


	//ImageRender 추가
	auto pRender = std::make_shared<ImageRender>();
	if (!pRender->PostInit(this))
		return false;
	if (!pRender->SetSprite(TEXT("End")))
		return false;
	pRender->SetExpansionRatio(2.5f);
	pRender->SetPivotRatio(0.5f, 1.f);
	if (!AddComponent(pRender, pRender->GetComponentTag()))
		return false;

	return true;
}


void CEndPickup::Render(const HDC & hDC)
{
	if (m_bActive)
	{
		GetComponent<ImageRender>().lock()->Draw(hDC);
	}
}
