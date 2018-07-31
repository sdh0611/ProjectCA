#pragma once

#include "..\..\stdafx.h"


class Collider;
class ColliderBox;
class ColliderCircle;
class CObject;

class CollisionDetector {
public:
	CollisionDetector();
	~CollisionDetector();


public:
	void Init();
	void Update(Collider* collider1, Collider* collider2);
	void Update(CObject* object1, CObject* object2);


private:
	bool BoxAndBox(ColliderBox* collider1, ColliderBox* colldier2);
	bool BoxAndBox(CObject* object1, CObject* object2);
	//bool BoxAndCircle(ColliderBox* collider1, ColliderCircle* colldier2);
	//bool CircleToCircle(ColliderCircle* collider1, ColliderCircle* colldier2);


private:



};