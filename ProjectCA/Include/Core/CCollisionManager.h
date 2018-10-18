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
	void Init();
	void Update(Collider* pCollider, Collider* pOther);
	void Update(std::shared_ptr<CActor> pActor, std::shared_ptr<CActor> pOther);


private:
	bool BoxAndBox(ColliderBox* collider1, ColliderBox* colldier2);
	bool BoxAndBox(std::shared_ptr<CActor> pActor, std::shared_ptr<CActor> pOther);
	//bool BoxAndCircle(ColliderBox* collider1, ColliderCircle* colldier2);
	//bool CircleToCircle(ColliderCircle* collider1, ColliderCircle* colldier2);


private:



};