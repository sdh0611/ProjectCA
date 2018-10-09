#include "..\..\stdafx.h"
#include "..\..\Include\Core\CollisionDetector.h"
#include "..\..\Include\Core\Components\Collider.h"
#include "..\..\Include\Core\Components\ColliderBox.h"
#include "..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\Include\Scene\Actor\CActor.h"
//#include "..\..\Include\Core\Components\ColliderCircle.h"



CollisionDetector::CollisionDetector()
{
	
}

CollisionDetector::~CollisionDetector()
{



}

void CollisionDetector::Init()
{



}

void CollisionDetector::Update(Collider * pCollider, Collider * pOther)
{
	if ((pCollider->GetColliderType() == Collider::CT_BOX) && (pOther->GetColliderType() == Collider::CT_BOX))
		BoxAndBox(static_cast<ColliderBox*>(pCollider), static_cast<ColliderBox*>(pOther));


}

void CollisionDetector::Update(std::shared_ptr<CActor> pActor, std::shared_ptr<CActor> pOther)
{
	Collider* collider1 = static_cast<Collider*>(pActor->GetComponent(TEXT("Collider")));
	Collider* collider2 = static_cast<Collider*>(pOther->GetComponent(TEXT("Collider")));

	if (collider1 == nullptr || collider2 == nullptr)
		return;

	if ((collider1->GetColliderType() == Collider::CT_BOX) && (collider2->GetColliderType() == Collider::CT_BOX))
		BoxAndBox(pActor, pOther);


}

//
bool CollisionDetector::BoxAndBox(ColliderBox * pCollider, ColliderBox * pOther)
{
	Types::Rect box1 = pCollider->GetBoxSize();
	Types::Rect box2 = pOther->GetBoxSize();

	if ( (box1.right < box2.left) || (box1.left > box2.right) )
	{
		pCollider->SetIsCollision(false);
		pOther->SetIsCollision(false);
		return false;
	}
	
	if ((box1.top > box2.bottom) || (box1.bottom < box2.top))
	{
		//Debug::MessageInfo(TEXT("Collision!"));
		pCollider->SetIsCollision(false);
		pOther->SetIsCollision(false);
		return false;
	}

	
	pCollider->SetIsCollision(true);
	pOther->SetIsCollision(true);
	//collider1->ResolveCollision();
	return true;

}

//메커니즘을 바꿔야할 것 같은데..
bool CollisionDetector::BoxAndBox(std::shared_ptr<CActor> pActor, std::shared_ptr<CActor> pOther)
{
	ColliderBox* collider1 = static_cast<ColliderBox*>(pActor->GetComponent(TEXT("Collider")));
	ColliderBox* collider2 = static_cast<ColliderBox*>(pOther->GetComponent(TEXT("Collider")));
	if (collider1 == nullptr || collider2 == nullptr) // 둘 중 하나가 Collider를 갖고 있지 않음.
		return false;

	PhysicsComponent* pPhysics1 =  static_cast<PhysicsComponent*>(pActor->GetComponent(TEXT("PhysicsComponent")));
	PhysicsComponent* pPhysics2 = static_cast<PhysicsComponent*>(pOther->GetComponent(TEXT("PhysicsComponent")));

	const Types::Rect& box1 = collider1->GetBoxSize();
	const Types::Rect& box2 = collider2->GetBoxSize();

	//bool isCollision = false;

	//Collider::CollisionType collisionType = Collider::COLLISION_TOP;

	//충돌이 아닌 경우이다.
	//일단 테스트용으로 충돌이 없는 모든 구간에 grounded를 false로 설정하게끔 함. -> 나중에 반드시 수정
	if ((box1.right < box2.left) || (box1.left > box2.right))
	{
		collider1->SetIsCollision(false);
		collider2->SetIsCollision(false);
		//if (pPhysics1 != nullptr)
		//	pPhysics1->SetGrounded(false);
		//if (pPhysics2 != nullptr)
		//	pPhysics2->SetGrounded(false);
		//
		return false;
	}

	if ((box1.top > box2.bottom) || (box1.bottom < box2.top))
	{
		//Debug::MessageInfo(TEXT("Collision!"));
		collider1->SetIsCollision(false);
		collider2->SetIsCollision(false);
		if (pPhysics1 != nullptr)
			pPhysics1->SetGrounded(false);
		if (pPhysics2 != nullptr)
			pPhysics2->SetGrounded(false);

		return false;
	}


	//여기부터 무조건 충돌이 일어난 경우이다.
	//Collision이 일어났음을 각각의 Collider Component에 알리고, OnCollision 메소드를 실행하여 충돌 후 동작 처리.
	collider1->SetIsCollision(true);
	collider2->SetIsCollision(true);
	collider1->OnCollision(pOther);
	collider2->OnCollision(pActor);

	return true;
}

//bool CollisionDetector::BoxAndCircle(ColliderBox * collider1, ColliderCircle * colldier2)
//{
//	return false;
//}
//
//bool CollisionDetector::CircleToCircle(ColliderCircle * collider1, ColliderCircle * colldier2)
//{
//	return false;
//}
