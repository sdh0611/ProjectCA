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


void CCollisionManager::Update()
{
}

void CCollisionManager::CheckCollision()
{
	for (auto first = m_ColliderList.cbegin(); first != m_ColliderList.cend(); ++first)
	{
		if ((*first)->IsActive())
		{
			for (auto second = first; second != m_ColliderList.cend(); ++second)
			{
				if (second == first)
					continue;

				if ((*second)->IsActive() && (*first)->IsActive())
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

	if (box1.left < box2.left)
	{
		fIntersecRectWidth = box1.right - box2.left;
		if (box1.top < box2.top)
		{
			fIntersecRectHeight = box1.bottom - box2.top;
			if (fIntersecRectWidth >= fIntersecRectHeight)
			{
				pCollider->SetCollisionType(Collider::COLLISION_BOT);
				pOther->SetCollisionType(Collider::COLLISION_TOP);
			}
			else
			{
				pCollider->SetCollisionType(Collider::COLLISION_RIGHT);
				pOther->SetCollisionType(Collider::COLLISION_LEFT);
			}

		}
		else if (box1.bottom > box2.bottom)
		{
			fIntersecRectHeight = box2.bottom - box1.top;
			if (fIntersecRectWidth >= fIntersecRectHeight)
			{
				pCollider->SetCollisionType(Collider::COLLISION_TOP);
				pOther->SetCollisionType(Collider::COLLISION_BOT);
			}
			else
			{
				pCollider->SetCollisionType(Collider::COLLISION_RIGHT);
				pOther->SetCollisionType(Collider::COLLISION_LEFT);
			}
		}
		else
		{
			fIntersecRectHeight = box1.bottom - box1.top;
			pCollider->SetCollisionType(Collider::COLLISION_RIGHT);
			pOther->SetCollisionType(Collider::COLLISION_LEFT);
		}

	}
	else if (box1.right > box2.right)
	{
		fIntersecRectWidth = box2.right - box1.left;
		if (box1.top < box2.top)
		{
			fIntersecRectHeight = box1.bottom - box2.top;
			if (fIntersecRectWidth >= fIntersecRectHeight)
			{
				pCollider->SetCollisionType(Collider::COLLISION_BOT);
				pOther->SetCollisionType(Collider::COLLISION_TOP);
			}
			else
			{
				pCollider->SetCollisionType(Collider::COLLISION_LEFT);
				pOther->SetCollisionType(Collider::COLLISION_RIGHT);
			}
		}
		else if (box1.bottom > box2.bottom)
		{
			fIntersecRectHeight = box2.bottom - box1.top;
			if (fIntersecRectWidth >= fIntersecRectHeight)
			{
				pCollider->SetCollisionType(Collider::COLLISION_TOP);
				pOther->SetCollisionType(Collider::COLLISION_BOT);
			}
			else
			{
				pCollider->SetCollisionType(Collider::COLLISION_LEFT);
				pOther->SetCollisionType(Collider::COLLISION_RIGHT);
			}
		}
		else
		{
			fIntersecRectHeight = box1.bottom - box1.top;
			pCollider->SetCollisionType(Collider::COLLISION_LEFT);
			pOther->SetCollisionType(Collider::COLLISION_RIGHT);
		}
	}
	else
	{
		fIntersecRectWidth = box1.right - box1.left;
		if (box1.top < box2.top && box1.bottom < box2.bottom)
		{
			fIntersecRectHeight = box1.bottom - box2.top;
			pCollider->SetCollisionType(Collider::COLLISION_BOT);
			pOther->SetCollisionType(Collider::COLLISION_TOP);
		}
		//else if (box1.bottom > box2.bottom)
		//{
		//	fIntersecRectHeight = box2.bottom - box1.top;
		//	pCollider->SetCollisionType(Collider::COLLISION_TOP);
		//	pOther->SetCollisionType(Collider::COLLISION_BOT);	
		//}
		else
		{
			fIntersecRectHeight = box2.bottom - box1.top;
			pCollider->SetCollisionType(Collider::COLLISION_TOP);
			pOther->SetCollisionType(Collider::COLLISION_BOT);
		}

	}

	pCollider->SetIsCollision(true);
	pOther->SetIsCollision(true);
	pCollider->ResolveCollision(static_cast<CObject*>(pOther->GetOwner()), (fIntersecRectWidth < fIntersecRectHeight ? fIntersecRectWidth : fIntersecRectHeight));
	pOther->ResolveCollision(static_cast<CObject*>(pCollider->GetOwner()), (fIntersecRectWidth < fIntersecRectHeight ? fIntersecRectWidth : fIntersecRectHeight));

	return true;

}
