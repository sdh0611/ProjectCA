#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Core\Components\Collider.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Scene\Actor\CActor.h"


Collider::Collider(ColliderType type)
	:m_Type(type)
{
	
}

Collider::~Collider()
{
	//Debug::MessageInfo(TEXT("Box Destruct"));
}

//Collision을 일으킨 Object Type별로 행동 정의
//Collision의 유형에 따른 동작도 따로 정의해야함(18.07.04)
void Collider::OnCollision(Types::ObjectType type, Collider::CollisionType collision)
{

	switch (type) {
	//case Types::OT_ENEMY:

	//	break;
	case Types::OT_PROBS:
		/*
			NOTE:
				PROBS Object의 옆면에 부딫힌거면 해당 방향으로의 이동을 무시하고,
				위아래로 부딫힌 경우 점프 동작에 영향력 행사.
		*/
		

		switch (collision) {
		case CollisionType::COLLISION_TOP:		//윗부분에 부딫힌 경우 중력 초기화
			{
				PhysicsComponent * physics = static_cast<PhysicsComponent*>(
					m_pOwner->GetComponent(TEXT("PhysicsComponent")));
				if (physics == nullptr)
					return;
					
				physics->ResetJumpForce();
				//Debug::MessageInfo(TEXT("TOP"));
			}
			break;

		//case CollisionType::COLLISION_BOT:		//밑부분에 부딫힌 경우 Y축아래로 진행하게함.
		//	break;
		case CollisionType::COLLISION_SIDE:		//옆면에 부딫힌 경우 X축상의 진행을 막음.
			m_pOwner->SetActorDirection(Types::DIR_IDLE);
			//Debug::MessageInfo(TEXT("SIDE"));
			break;
		}
		break;


	//case Types::OT_PICKUP:

	//	break;
	}



}

//오버로딩된 버전 제공
void Collider::OnCollision(Types::ObjectType type)
{
	if (m_bIsCollision) {
		ComponentBase* pComponent = nullptr;
		switch (type) {
		case Types::OT_PROBS:
			//m_pOwner->SetObjectState(Types::OS_IDLE);
			pComponent = m_pOwner->GetComponent(TEXT("PhysicsComponent"));
			PhysicsComponent* physics = static_cast<PhysicsComponent*>(pComponent);
			physics->ResetSpeed();
			break;
		}
	}
	//else {
	//	m_pOwner->SetObjectState(Types::OS_MOVE);
	//}
	

}

//void Collider::Init()
//{
//	//m_pOwner = owner;
//
//
//}
//
//void Collider::Update(const double & deltaTime)
//{
//
//
//}
