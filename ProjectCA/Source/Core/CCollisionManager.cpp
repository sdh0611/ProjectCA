#include "..\..\stdafx.h"
#include "..\..\Include\Core\CCollisionManager.h"
#include "..\..\Include\Scene\CCameraManager.h"
#include "..\..\Include\Core\Components\Collider.h"
#include "..\..\Include\Core\Components\ColliderBox.h"
#include "..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\Include\Scene\Actor\CActor.h"
//#include "..\..\Include\Core\Components\ColliderCircle.h"



CCollisionManager::CCollisionManager()
{
	
}

CCollisionManager::~CCollisionManager()
{
	m_ColliderList.clear();
}

bool CCollisionManager::Init()
{


	return true;
}

//void CCollisionManager::Update(Collider * pCollider, Collider * pOther)
//{
//	if ((pCollider->GetColliderType() == Collider::CT_BOX) && (pOther->GetColliderType() == Collider::CT_BOX))
//		BoxAndBox(static_cast<ColliderBox*>(pCollider), static_cast<ColliderBox*>(pOther));
//
//
//}

void CCollisionManager::Update()
{
}

//bool CCollisionManager::CheckCollision(std::shared_ptr<CActor> pActor, std::shared_ptr<CActor> pOther)
//{
//	bool							 bIsCollision = false;
//	std::shared_ptr<Collider> collider1 = STATIC_POINTER_CAST<Collider>(pActor->GetComponent(TEXT("Collider")).lock());
//	std::shared_ptr<Collider> collider2 = STATIC_POINTER_CAST<Collider>(pOther->GetComponent(TEXT("Collider")).lock());
//
//	if (collider1 == nullptr || collider2 == nullptr)
//		return bIsCollision;
//
//	if ((collider1->GetColliderType() == Collider::CT_BOX) && (collider2->GetColliderType() == Collider::CT_BOX))
//		bIsCollision = BoxAndBox(pActor, pOther);
//
//	return bIsCollision;
//}

void CCollisionManager::CheckCollision()
{
	for (auto first = m_ColliderList.cbegin(); first != m_ColliderList.cend(); ++first)
	{
		if ((*first)->GetOwner()->IsActive())
		{
			for (auto second = first; second != m_ColliderList.cend(); ++second)
			{
				if (second == first)
					continue;

				if ((*second)->GetOwner()->IsActive())
					CheckCollisionType((*first), (*second));

			}
	
		}

	}

}

void CCollisionManager::AddCollider(Collider * pCollider)
{
	m_ColliderList.emplace_back(pCollider);
}

void CCollisionManager::DeleteCollider(Collider * pCollider)
{
	for (auto it = m_ColliderList.cbegin(); it != m_ColliderList.cend(); ++it)
	{
		if ((*it) == pCollider)
		{
			m_ColliderList.erase(it);
			break;
		}

	}

}

void CCollisionManager::Clear()
{
	m_ColliderList.clear();
}


void CCollisionManager::CheckCollisionType(Collider* pCollider, Collider* pOther)
{
	Collider::ColliderType colliderType			= pCollider->GetColliderType();
	Collider::ColliderType otherColliderType = pOther->GetColliderType();

	if ((colliderType == Collider::CT_BOX) && (otherColliderType == Collider::CT_BOX))
		BoxAndBox(static_cast<ColliderBox*>(pCollider), static_cast<ColliderBox*>(pOther));

}
//
//bool CCollisionManager::BoxAndBox(std::shared_ptr<CActor> pActor, std::shared_ptr<CActor> pOtherActor)
//{
//	ColliderBox* pCollider = static_cast<ColliderBox*>(pActor->GetComponent(TEXT("Collider")));
//	ColliderBox* pOther = static_cast<ColliderBox*>(pOtherActor->GetComponent(TEXT("Collider")));
//	if (pCollider == nullptr || pOther == nullptr) // 둘 중 하나라도 Collider를 갖고 있지 않은 경우
//		return false;
//
//	const Types::Rect& box1 = pCollider->GetRect();
//	const Types::Rect& box2 = pOther->GetRect();
//
//	//충돌이 아닌 경우이다.
//	if ((box1.right < box2.left) || (box1.left > box2.right))
//	{
//		pCollider->SetIsCollision(false);
//		pOther->SetIsCollision(false);
//		pCollider->SetCollisionType(Collider::COLLISION_IDLE);
//		pOther->SetCollisionType(Collider::COLLISION_IDLE);
//		return false;
//	}
//
//	if ((box1.top > box2.bottom) || (box1.bottom < box2.top))
//	{
//		pCollider->SetIsCollision(false);
//		pOther->SetIsCollision(false);
//		pCollider->SetCollisionType(Collider::COLLISION_IDLE);
//		pOther->SetCollisionType(Collider::COLLISION_IDLE);
//
//		return false;
//	}
//
//	//여기부터 무조건 충돌이 일어난 경우이다.
//	//Collision이 일어났음을 각각의 Collider Component에 알리고, OnCollision 메소드를 실행하여 충돌 후 동작 처리.
//	float fIntersecRectWidth = 0.f;
//	float fIntersecRectHeight = 0.f;
//
//	//box1 : Left, Box2 : Right
//	if (box1.right < box2.right)
//	{
//		if (box1.left < box2.left)
//		{
//			fIntersecRectWidth = box1.right - box2.left;
//			if (box1.top < box2.top && box1.bottom >= box2.top)		//위
//			{
//				fIntersecRectHeight = box1.bottom - box2.top;
//				if (fIntersecRectHeight > fIntersecRectWidth)
//				{
//					pCollider->SetCollisionType(Collider::COLLISION_RIGHT);
//					pOther->SetCollisionType(Collider::COLLISION_LEFT);
//				}
//				else
//				{
//					pCollider->SetCollisionType(Collider::COLLISION_BOT);
//					pOther->SetCollisionType(Collider::COLLISION_TOP);
//				}
//			}
//			else if (box1.top <= box2.bottom && box1.bottom > box2.bottom)	//아래
//			{
//				fIntersecRectHeight = box2.bottom - box1.top;
//				if (fIntersecRectHeight > fIntersecRectWidth)
//				{
//					pCollider->SetCollisionType(Collider::COLLISION_RIGHT);
//					pOther->SetCollisionType(Collider::COLLISION_LEFT);
//				}
//				else
//				{
//					pCollider->SetCollisionType(Collider::COLLISION_TOP);
//					pOther->SetCollisionType(Collider::COLLISION_BOT);
//				}
//			}
//			else   //중간
//			{
//				pCollider->SetCollisionType(Collider::COLLISION_RIGHT);
//				pOther->SetCollisionType(Collider::COLLISION_LEFT);
//			}
//		}
//		else
//		{
//			if (box1.bottom >= box2.top && box1.bottom < box2.bottom)
//			{
//				pCollider->SetCollisionType(Collider::COLLISION_BOT);
//				pOther->SetCollisionType(Collider::COLLISION_TOP);
//			}
//			else if(box1.top <= box2.bottom && box1.top > box2.top)
//			{
//				pCollider->SetCollisionType(Collider::COLLISION_TOP);
//				pOther->SetCollisionType(Collider::COLLISION_BOT);
//			}
//		}
//	}
//	//box1 : Right, box2 : Left
//	else if (box1.left > box2.left)
//	{
//		if (box1.right > box2.right)
//		{
//			fIntersecRectWidth = box2.right - box1.left;
//			if (box1.top < box2.top && box1.bottom >= box2.top)		//위
//			{
//				fIntersecRectHeight = box1.bottom - box2.top;
//				if (fIntersecRectHeight > fIntersecRectWidth)
//				{
//					pCollider->SetCollisionType(Collider::COLLISION_LEFT);
//					pOther->SetCollisionType(Collider::COLLISION_RIGHT);
//				}
//				else
//				{
//					pCollider->SetCollisionType(Collider::COLLISION_BOT);
//					pOther->SetCollisionType(Collider::COLLISION_TOP);
//				}
//			}
//			else if (box1.top < box2.bottom && box1.bottom > box2.bottom)	//아래
//			{
//				fIntersecRectHeight = box2.bottom - box1.top;
//				if (fIntersecRectHeight > fIntersecRectWidth)
//				{
//					pCollider->SetCollisionType(Collider::COLLISION_LEFT);
//					pOther->SetCollisionType(Collider::COLLISION_RIGHT);
//				}
//				else
//				{
//					pCollider->SetCollisionType(Collider::COLLISION_TOP);
//					pOther->SetCollisionType(Collider::COLLISION_BOT);
//				}
//			}
//			else   //중간
//			{
//				pCollider->SetCollisionType(Collider::COLLISION_RIGHT);
//				pOther->SetCollisionType(Collider::COLLISION_LEFT);
//			}
//		}
//		else
//		{
//			if (box1.bottom >= box2.top && box1.bottom < box2.bottom)
//			{
//				pCollider->SetCollisionType(Collider::COLLISION_BOT);
//				pOther->SetCollisionType(Collider::COLLISION_TOP);
//			}
//			else if (box1.top <= box2.bottom && box1.top > box2.top)
//			{
//				pCollider->SetCollisionType(Collider::COLLISION_TOP);
//				pOther->SetCollisionType(Collider::COLLISION_BOT);
//			}
//		}
//	}
//
//	pCollider->SetIsCollision(true);
//	pOther->SetIsCollision(true);
//	pCollider->ResolveCollision(pOtherActor);
//	pOther->ResolveCollision(pActor);
//
//	return true;
//
//}


bool CCollisionManager::BoxAndBox(ColliderBox* pCollider, ColliderBox* pOther)
{
	const Types::Rect& box1 = pCollider->GetRect();
	const Types::Rect& box2 = pOther->GetRect();

	//충돌이 아닌 경우이다.
	if ((box1.right < box2.left) || (box1.left > box2.right))
	{
		pCollider->SetIsCollision(false);
		pOther->SetIsCollision(false);
		pCollider->SetCollisionType(Collider::COLLISION_IDLE);
		pOther->SetCollisionType(Collider::COLLISION_IDLE);
		return false;
	}

	if ((box1.top > box2.bottom) || (box1.bottom < box2.top))
	{
		pCollider->SetIsCollision(false);
		pOther->SetIsCollision(false);
		pCollider->SetCollisionType(Collider::COLLISION_IDLE);
		pOther->SetCollisionType(Collider::COLLISION_IDLE);

		return false;
	}

	//여기부터 무조건 충돌이 일어난 경우이다.
	//Collision이 일어났음을 각각의 Collider Component에 알리고, OnCollision 메소드를 실행하여 충돌 후 동작 처리.
	float fIntersecRectWidth = 0.f;
	float fIntersecRectHeight = 0.f;

	//box1 : Left, Box2 : Right
	if (box1.right < box2.right)
	{
		if (box1.left < box2.left)
		{
			fIntersecRectWidth = box1.right - box2.left;
			if (box1.top < box2.top && box1.bottom >= box2.top)		//위
			{
				fIntersecRectHeight = box1.bottom - box2.top;
				if (fIntersecRectHeight > fIntersecRectWidth)
				{
					pCollider->SetCollisionType(Collider::COLLISION_RIGHT);
					pOther->SetCollisionType(Collider::COLLISION_LEFT);
				}
				else
				{
					pCollider->SetCollisionType(Collider::COLLISION_BOT);
					pOther->SetCollisionType(Collider::COLLISION_TOP);
				}
			}
			else if (box1.top <= box2.bottom && box1.bottom > box2.bottom)	//아래
			{
				fIntersecRectHeight = box2.bottom - box1.top;
				if (fIntersecRectHeight > fIntersecRectWidth)
				{
					pCollider->SetCollisionType(Collider::COLLISION_RIGHT);
					pOther->SetCollisionType(Collider::COLLISION_LEFT);
				}
				else
				{
					pCollider->SetCollisionType(Collider::COLLISION_TOP);
					pOther->SetCollisionType(Collider::COLLISION_BOT);
				}
			}
			else   //중간
			{
				pCollider->SetCollisionType(Collider::COLLISION_RIGHT);
				pOther->SetCollisionType(Collider::COLLISION_LEFT);
			}
		}
		else
		{
			if (box1.bottom >= box2.top && box1.bottom < box2.bottom)
			{
				pCollider->SetCollisionType(Collider::COLLISION_BOT);
				pOther->SetCollisionType(Collider::COLLISION_TOP);
			}
			else if (box1.top <= box2.bottom && box1.top > box2.top)
			{
				pCollider->SetCollisionType(Collider::COLLISION_TOP);
				pOther->SetCollisionType(Collider::COLLISION_BOT);
			}
		}
	}
	//box1 : Right, box2 : Left
	else if (box1.left > box2.left)
	{
		if (box1.right > box2.right)
		{
			fIntersecRectWidth = box2.right - box1.left;
			if (box1.top < box2.top && box1.bottom >= box2.top)		//위
			{
				fIntersecRectHeight = box1.bottom - box2.top;
				if (fIntersecRectHeight > fIntersecRectWidth)
				{
					pCollider->SetCollisionType(Collider::COLLISION_LEFT);
					pOther->SetCollisionType(Collider::COLLISION_RIGHT);
				}
				else
				{
					pCollider->SetCollisionType(Collider::COLLISION_BOT);
					pOther->SetCollisionType(Collider::COLLISION_TOP);
				}
			}
			else if (box1.top < box2.bottom && box1.bottom > box2.bottom)	//아래
			{
				fIntersecRectHeight = box2.bottom - box1.top;
				if (fIntersecRectHeight > fIntersecRectWidth)
				{
					pCollider->SetCollisionType(Collider::COLLISION_LEFT);
					pOther->SetCollisionType(Collider::COLLISION_RIGHT);
				}
				else
				{
					pCollider->SetCollisionType(Collider::COLLISION_TOP);
					pOther->SetCollisionType(Collider::COLLISION_BOT);
				}
			}
			else   //중간
			{
				pCollider->SetCollisionType(Collider::COLLISION_RIGHT);
				pOther->SetCollisionType(Collider::COLLISION_LEFT);
			}
		}
		else
		{
			if (box1.bottom >= box2.top && box1.bottom < box2.bottom)
			{
				pCollider->SetCollisionType(Collider::COLLISION_BOT);
				pOther->SetCollisionType(Collider::COLLISION_TOP);
			}
			else if (box1.top <= box2.bottom && box1.top > box2.top)
			{
				pCollider->SetCollisionType(Collider::COLLISION_TOP);
				pOther->SetCollisionType(Collider::COLLISION_BOT);
			}
		}
	}

	pCollider->SetIsCollision(true);
	pOther->SetIsCollision(true);
	pCollider->ResolveCollision(pOther->GetOwner());
	pOther->ResolveCollision(pCollider->GetOwner());

	return true;

}
