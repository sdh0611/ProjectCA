#include "..\..\stdafx.h"
#include "..\..\Include\Core\CollisionDetector.h"
#include "..\..\Include\Core\Components\Collider.h"
#include "..\..\Include\Core\Components\ColliderBox.h"
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

void CollisionDetector::Update(Collider * collider1, Collider * collider2)
{
	if ((collider1->GetColliderType() == Collider::CT_BOX) && (collider2->GetColliderType() == Collider::CT_BOX))
		BoxAndBox(static_cast<ColliderBox*>(collider1), static_cast<ColliderBox*>(collider2));


}

void CollisionDetector::Update(CObject * object1, CObject * object2)
{
	Collider* collider1 = static_cast<Collider*>(object1->GetComponent(TEXT("Collider")));
	Collider* collider2 = static_cast<Collider*>(object2->GetComponent(TEXT("Collider")));

	if ((collider1->GetColliderType() == Collider::CT_BOX) && (collider2->GetColliderType() == Collider::CT_BOX))
		BoxAndBox(object1, object2);


}

//
bool CollisionDetector::BoxAndBox(ColliderBox * collider1, ColliderBox * collider2)
{
	Types::Rect box1 = collider1->GetBoxSize();
	Types::Rect box2 = collider2->GetBoxSize();

	if ( (box1.right < box2.left) || (box1.left > box2.right) )
	{
		collider1->SetIsCollision(false);
		collider2->SetIsCollision(false);
		return false;
	}
	
	if ((box1.top > box2.bottom) || (box1.bottom < box2.top))
	{
		//Debug::MessageInfo(TEXT("Collision!"));
		collider1->SetIsCollision(false);
		collider2->SetIsCollision(false);
		return false;
	}

	
	collider1->SetIsCollision(true);
	collider2->SetIsCollision(true);
	//collider1->ResolveCollision();
	return true;

}

//메커니즘을 바꿔야할 것 같은데..
bool CollisionDetector::BoxAndBox(CObject * object1, CObject * object2)
{
	ColliderBox* collider1 = static_cast<ColliderBox*>(object1->GetComponent(TEXT("Collider")));
	ColliderBox* collider2 = static_cast<ColliderBox*>(object2->GetComponent(TEXT("Collider")));

	const Types::Rect& box1 = collider1->GetBoxSize();
	const Types::Rect& box2 = collider2->GetBoxSize();

	bool isCollision = false;

	//Collider::CollisionType collisionType = Collider::COLLISION_TOP;

	//충돌이 아닌 경우이다.
	if ((box1.right < box2.left) || (box1.left > box2.right))
	{
		collider1->SetIsCollision(false);
		collider2->SetIsCollision(false);
		return false;
	}

	if ((box1.top > box2.bottom) || (box1.bottom < box2.top))
	{
		//Debug::MessageInfo(TEXT("Collision!"));
		collider1->SetIsCollision(false);
		collider2->SetIsCollision(false);
		return false;
	}


	//여기부터 무조건 충돌이 일어난 경우이다.
	//box2를 기준으로 충돌 유형을 검사한다.
	collider1->SetIsCollision(true);
	collider2->SetIsCollision(true);

	//만약 Trigger나 pickup등에 사용될 경우 아래 if문은 무시되어야 한다. -> 조건 추가예정(18.07.12)
	//BUG(18.07.12) : 아예 collider 충돌반응이 안일어남 -> 아예 충돌이 일어난 경우가 if문에 바로 걸리는듯?
	//if (box1.bottom> box2.top && box1.top < box2.bottom) {
	//	//side인 경우
	//	Debug::MessageInfo(TEXT("COLSIDE"));
	//	collisionType = Collider::COLLISION_SIDE;
	//}
	//else {
	//	//top인 경우
	//	Debug::MessageInfo(TEXT("COLTOP"));
	//	collisionType = Collider::COLLISION_TOP;
	//}
	////else if (box1.top < box2.bottom) {
	////	//bot인 경우
	////}	
	//
	//collider1->ResolveCollision(object2->GetObjectType(), collisionType);
	//collider2->ResolveCollision(object1->GetObjectType(), collisionType);

	////For FrameWork Testing
	////충돌이 일어난 상대 Object의 ObjectType정보를 인수로 보내준다.
	//collider1->ResolveCollision(object2->GetObjectType());
	//collider2->ResolveCollision(object1->GetObjectType());

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
