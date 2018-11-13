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

private:
	void CheckCollisionType(std::shared_ptr<Collider> pCollider, std::shared_ptr<Collider> pOther);
	//bool BoxAndBox(std::shared_ptr<CActor> pActor, std::shared_ptr<CActor> pOther);
	bool BoxAndBox(std::shared_ptr<ColliderBox> pCollider, std::shared_ptr<ColliderBox> pOther);
	//bool BoxAndCircle(ColliderBox* collider1, ColliderCircle* colldier2);
	//bool CircleToCircle(ColliderCircle* collider1, ColliderCircle* colldier2);


private:
	typedef std::list<std::shared_ptr<Collider>> ColliderList;
	ColliderList	m_ColliderList;

};