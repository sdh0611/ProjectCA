#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Core\Components\ColliderBox.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Scene\Actor\CActor.h"



ColliderBox::ColliderBox()
	:Collider(CT_BOX), m_BoxSize(0, 0, 0, 0)
{

	//Debug::MessageInfo(TEXT("Create Box!"));
}

//ColliderBox::ColliderBox(CObject * owner, const Types::Rect& rect) :
//	Collider(owner, CT_BOX), m_BoxSize(rect),
//	m_iWidth(rect.right - rect.left), m_iHeight(rect.bottom - rect.top)
//{     
//	//Debug::MessageInfo(TEXT("Create Box2!"));
//	
//}

ColliderBox::~ColliderBox()
{
	//Debug::MessageInfo(TEXT("Bos Destruct"));
}

bool ColliderBox::Init(CActor* pOwner, const Types::tstring& strTag)
{
	auto pActor = std::shared_ptr<CActor>(pOwner);

	//if (point.x < 0.f || point.y < 0.f)
	//	return false;

	//m_ColliderPoint = point;
	m_pOwner = pActor;
	
	//처음 Init할 때 기본값으로 Object의 너비, 높이를 따라가도록 함.
	m_iWidth = m_pOwner->GetActorWidth();
	m_iHeight = m_pOwner->GetActorHeight();

	m_BoxSize.left = m_pOwner->GetActorPoint().x;
	m_BoxSize.top = m_pOwner->GetActorPoint().y;
	m_BoxSize.right = m_pOwner->GetActorPoint().x + m_pOwner->GetActorWidth();
	m_BoxSize.bottom = m_pOwner->GetActorPoint().y + m_pOwner->GetActorHeight();

	m_bIsCollision = false;
	m_strComponentTag = strTag;
	//Debug::MessageInfo(TEXT("Box Init"));

	return true;
}

//bool ColliderBox::Init(CActor * owner, const Types::tstring & strTag)
//{
//	//m_Collide
//
//
//	return true;
//}

//물체가 움직임에 따라 CollisionBox의 좌표도 같이 이동해야함.
//아직 Offset적용 전이라 일단 해놓고, 나중에 Offset 적용할 때 수정할 것.(5.31)
void ColliderBox::Update(double fDeltaTime)
{
	//물체 위치에 따라 CollisionBox 위치이동
	m_BoxSize.left = m_pOwner->GetActorPoint().x;
	m_BoxSize.top = m_pOwner->GetActorPoint().y;
	m_BoxSize.right = m_pOwner->GetActorPoint().x + (float)m_iWidth;
	m_BoxSize.bottom = m_pOwner->GetActorPoint().y + (float)m_iHeight;

	if (m_bIsCollision) {
		//m_bIsCollision = false;
		//PhysicsComponent* physics = static_cast<PhysicsComponent*>(m_pOwner->GetComponent(TEXT("PhysicsPhysicsComponent")));
		//if (physics == nullptr)
		//	return;

		//physics->Update(deltaTime);
	}
	
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
