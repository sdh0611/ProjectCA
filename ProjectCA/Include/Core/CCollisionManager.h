#pragma once

#include "..\..\stdafx.h"
#include "..\Singleton.hpp"

class Collider;
class ColliderBox;
class ColliderCircle;
class CActor;

class CCollisionManager : public Singleton<CCollisionManager> {
	
	DECLARE_SINGLETON(CCollisionManager)

public:
	bool Init();
	//void Update(Collider* pCollider, Collider* pOther);	//삭제예정
	void Update();
	//bool CheckCollision(std::shared_ptr<CActor> pActor, std::shared_ptr<CActor> pOther);
	void CheckCollision();


public:
	void AddCollider(Collider* pCollider);
	void DeleteCollider(Collider* pCollider);
	void Clear();


private:
	void CheckCollisionType(Collider* pCollider, Collider* pOther);
	//bool BoxAndBox(std::shared_ptr<CActor> pActor, std::shared_ptr<CActor> pOther);
	bool BoxAndBox(ColliderBox* pCollider, ColliderBox* pOther);
	//bool BoxAndCircle(ColliderBox* collider1, ColliderCircle* colldier2);
	//bool CircleToCircle(ColliderCircle* collider1, ColliderCircle* colldier2);


private:
	typedef std::list<Collider*> ColliderList;
	ColliderList	m_ColliderList;

};