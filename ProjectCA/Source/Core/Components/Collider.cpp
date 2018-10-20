#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Core\Components\Collider.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Scene\Actor\CActor.h"


Collider::Collider(ColliderType type)
	:m_Type(type), m_pDelegate(nullptr)
{
	
}

Collider::~Collider()
{
	//Debug::MessageInfo(TEXT("Box Destruct"));
}

//오버로딩된 버전 제공
//	->CallBack 메소드로 구현해볼까 ?
//void Collider::OnCollision(Types::ObjectType type)
//{
//	if (m_bIsCollision) {
//		ComponentBase* pComponent = nullptr;
//		switch (type) {
//		case Types::OT_PROB:
//			//m_pOwner->SetObjectState(Types::OS_IDLE);
//			pComponent = m_pOwner->GetComponent(TEXT("PhysicsComponent"));
//			PhysicsComponent* physics = static_cast<PhysicsComponent*>(pComponent);
//			physics->RestoreActorPoint();
//			break;
//		}
//	}
//	//else {
//	//	m_pOwner->SetObjectState(Types::OS_MOVE);
//	//}
//	
//
//}

//Collision이 일어났다면 호출하게 될 메소드
void Collider::OnCollision(std::shared_ptr<CActor> pOther)
{
	if (m_pDelegate != nullptr)
	{
		m_pDelegate(m_pOwner, pOther, m_collisionType);
		m_collisionType = COLLISION_IDLE;
	}
	//m_bIsCollision = false;
}

void Collider::OnTriggered(std::shared_ptr<CActor> pOther)
{
	if(m_pDelegate != nullptr)
		m_pDelegate(m_pOwner, pOther, m_collisionType);
}

Delegate Collider::SetDelegate(Delegate dele)
{
	Delegate temp = m_pDelegate;

	m_pDelegate = dele;

	return temp;
}
