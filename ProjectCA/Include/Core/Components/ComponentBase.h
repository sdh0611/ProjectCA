//컴포넌트들이 상속받을 최상위 추상클래스 정의

#pragma once

#include "..\..\..\stdafx.h"
#include "Message\ComponentMessage.h"

class CObject;

class ComponentBase {

public:
	ComponentBase(CObject* owner, const Types::tstring& strTag) 
		: m_pOwner(owner), m_strComponentTag(strTag) { };
	virtual ~ComponentBase() { };

	
public:
	//virtual void Init() = 0;
	virtual void Update(float fDeltaTime) = 0;
	virtual void OnMessage(ComponentMessage msg);

public:
	inline CObject* GetOwner() { return m_pOwner; }
	inline void SetOwner(CObject* owner) { m_pOwner = owner; }
	inline const Types::tstring& GetComponentTag() const { return m_strComponentTag; }
	inline void SetComponentName(const Types::tstring& tag) { m_strComponentTag = tag; }


protected:
	Types::Point				m_Point;
	Types::tstring			m_strComponentTag;
	CObject*					m_pOwner;

};