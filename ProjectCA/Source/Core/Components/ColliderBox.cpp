#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Core\Components\ColliderBox.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Scene\Actor\CActor.h"



ColliderBox::ColliderBox()
	:Collider(CT_BOX), m_BoxSize(0, 0, 0, 0)
{
}

ColliderBox::~ColliderBox()
{
}

bool ColliderBox::PostInit(CActor* pOwner, const Types::tstring& strTag)
{
	auto pActor = std::shared_ptr<CActor>(pOwner);

	//if (point.x < 0.f || point.y < 0.f)
	//	return false;

	//m_ColliderPoint = point;
	m_pOwner = pActor;
	
	//처음 Init할 때 기본값으로 Actor의 너비, 높이를 따라가도록 함.
	m_fWidth = m_pOwner->GetActorWidth();
	m_fHeight = m_pOwner->GetActorHeight();

	m_ColliderPoint = m_CurColliderPoint = m_pOwner->GetActorPoint();

	m_BoxSize.left = m_CurColliderPoint.x - m_fWidth / 2;
	m_BoxSize.top = m_CurColliderPoint.y - m_fHeight;
	m_BoxSize.right = m_CurColliderPoint.x + m_fWidth / 2;
	m_BoxSize.bottom = m_CurColliderPoint.y;

	m_bIsCollision = false;
	m_strComponentTag = strTag;
	//Debug::MessageInfo(TEXT("Box Init"));

	return true;
}

void ColliderBox::Init()
{
	m_CurColliderPoint = m_ColliderPoint;

	m_BoxSize.left = m_ColliderPoint.x - m_fWidth / 2;
	m_BoxSize.top = m_ColliderPoint.y - m_fHeight;
	m_BoxSize.right = m_ColliderPoint.x + m_fWidth / 2;
	m_BoxSize.bottom = m_ColliderPoint.y;

}


//물체가 움직임에 따라 CollisionBox의 좌표도 같이 이동해야함.
void ColliderBox::Update(double dDeltaTime)
{
	//물체 위치에 따라 CollisionBox 위치이동
	//m_BoxSize.left = m_pOwner->GetActorPoint().x;
	//m_BoxSize.top = m_pOwner->GetActorPoint().y;
	//m_BoxSize.right = m_pOwner->GetActorPoint().x + (float)m_iWidth;
	//m_BoxSize.bottom = m_pOwner->GetActorPoint().y + (float)m_iHeight;
	auto pComponent = m_pOwner->GetComponent(TEXT("PhysicsComponent"));
	if (pComponent)
	{
		auto pPhysics = static_cast<PhysicsComponent*>(pComponent);
		
		float fCurSpeed = pPhysics->GetCurSpeed();
		float fCurJump = pPhysics->GetCurJumpForce();
		
		float fCurWidth = m_fWidth;
		float fCurHeight = m_fHeight;

		m_CurColliderPoint.x += fCurSpeed * dDeltaTime;
		m_CurColliderPoint.y -= fCurJump * dDeltaTime;

		if (m_pOwner->GetActorState() == Types::AS_SITDOWN)
		{
			fCurHeight /= 2.f;
		}
		m_BoxSize.left = m_CurColliderPoint.x - fCurWidth / 2;
		m_BoxSize.top = m_CurColliderPoint.y - fCurHeight;
		m_BoxSize.right = m_CurColliderPoint.x + fCurWidth / 2;
		m_BoxSize.bottom = m_CurColliderPoint.y;
		
	}

	if (m_bIsCollision) {
		//m_bIsCollision = false;
		//PhysicsComponent* physics = static_cast<PhysicsComponent*>(m_pOwner->GetComponent(TEXT("PhysicsPhysicsComponent")));
		//if (physics == nullptr)
		//	return;

		//physics->Update(deltaTime);
	}
	
}

void ColliderBox::DrawCollider(const HDC & hDC)
{
	Rectangle(hDC, m_BoxSize.left, m_BoxSize.top, m_BoxSize.right, m_BoxSize.bottom);

}

void ColliderBox::SetSize(float fWidth, float fHeight)
{
	if (fWidth < 0.f || fHeight < 0.f)
		return;

	m_fWidth = fWidth;
	m_fHeight = fHeight;
}

void ColliderBox::SetRect(float left, float top, float right, float bottom)
{
	m_BoxSize.left = left;
	m_BoxSize.top = top;
	m_BoxSize.right = right;
	m_BoxSize.bottom = bottom;
}

//void ColliderBox::ResolveCollision(Types::ObjectType type, CollisionType collision)
//{
//
//	switch (type) {
//		//case Types::OT_ENEMY:
//
//		//	break;
//	case Types::OT_PROBS:
//		/*
//		NOTE:
//		PROBS Object의 옆면에 부딫힌거면 해당 방향으로의 이동을 무시하고, -> Object의 방향을 IDLE로 바꾸면됨.
//		위아래로 부딫힌 경우 점프 동작에 영향력 행사.
//		*/
//		//if(m_pOwner->GetObjectPoint().x < )
//		switch (collision) {
//		case CollisionType::COLLSION_TOP:		//윗부분에 부딫힌 경우 중력 초기화
//			PhysicsComponent * physics = static_cast<PhysicsComponent*>(
//				m_pOwner->GetComponent(TEXT("PhysicsComponent")));
//			physics->ResetJumpForce();
//			break;
//		case CollisionType::COLLSION_BOT:		//밑부분에 부딫힌 경우 Y축아래로 진행하게함.
//
//			break;
//		case CollisionType::COLLSION_SIDE:		//옆면에 부딫힌 경우 X축상의 진행을 막음.
//			
//			break;
//		}
//		break;
//		//case Types::OT_PICKUP:
//
//		//	break;
//	}
//
//
//
//}
