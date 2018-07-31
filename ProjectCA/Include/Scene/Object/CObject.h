#pragma once

/*
	NOTE:
		게임 내 모든 Object들의 공통 속성, 동작들을 정의한 Class
		모든 Object들은 이 Class를 상속받고, 구체적인 동작은 따로 구현해야함.
*/

#include "..\..\..\stdafx.h"

class ComponentBase;

class CObject {
	
public:
	CObject(Types::ObjectType type, Types::Point point, UINT iWidth, UINT iHeight , Types::ObjectState state = Types::OS_IDLE,
		Types::Direction dir = Types::DIR_IDLE);
	virtual ~CObject();


public:
	virtual bool Init();
	virtual void Update(float fDeltaTime);
	virtual void Render(const HDC& hDC) = 0;
	virtual void LateUpdate(float fDeltaTime);

public:
	ComponentBase * GetComponent(const Types::tstring& tag);
	bool AddComponent(const Types::tstring&, ComponentBase* component);
	bool DeleteComponent(const Types::tstring& tag);


public:
	const Types::ObjectType& GetObjectType() const { return m_ObjectType; }
	void SetObjectType(Types::ObjectType type) { m_ObjectType = type; }
	const Types::ObjectState& GetObjectState() const { return m_ObjectState; }
	void SetObjectState(Types::ObjectState state) { m_ObjectState = state; }
	const Types::Direction& GetObjectDirection() const { return m_ObjectDirection; }
	void SetObjectDirection(Types::Direction dir) { m_ObjectDirection = dir; }
	const Types::Point& GetObjectPoint() const { return m_fObjectPoint; }
	void SetObjectPoint(Types::Point point) { if(point.x >= 0 && point.y >= 0) m_fObjectPoint = point; }
	void SetObjectPoint(float x, float y) { if (x>=0 && y>=0) m_fObjectPoint = { x, y }; }
	void SetObjectPointX(float x) { if (x>=0) m_fObjectPoint.x = x; }
	void SetObjectPointY(float y) { if (y>=0) m_fObjectPoint.y = y; }


protected:
	Types::ObjectType						m_ObjectType;
	Types::ObjectState						m_ObjectState;
	Types::Direction						m_ObjectDirection;	
	Types::Point								m_fObjectPoint;
	UINT										m_iObjectWidth;
	UINT										m_iObjectHeight;
	typedef std::unordered_map<Types::tstring, ComponentBase*> ComponentTable;
	ComponentTable						m_ComponentTable;
	ComponentTable::iterator			m_it;


};