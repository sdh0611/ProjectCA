#pragma once

/*
	NOTE:
		컴포넌트들이 상속받을 최상위 추상클래스 정의
*/

#include "..\..\..\stdafx.h"
#include "Message\ComponentMessage.h"

class CObject;

class ComponentBase {

public:
	ComponentBase() = default;
	virtual ~ComponentBase() { };

	
public:
	//virtual bool Init(CActor* pOwner, const Types::tstring& strTag) = 0;
	virtual void Init() = 0;
	virtual void Update(double dDeltaTime) = 0;
	//virtual void OnMessage(ComponentMessage msg);
	virtual void LateUpdate(double dDeltaTime) = 0;
	void Destroy();

public:
	inline std::weak_ptr<CObject> GetOwner() { return m_pOwner; }
	inline void SetOwner(std::shared_ptr<CObject> owner) { m_pOwner = owner; }
	inline const Types::tstring& GetComponentTag() const { return m_strComponentTag; }
	inline void SetComponentName(const Types::tstring& tag) { m_strComponentTag = tag; }


protected:
	Types::tstring						m_strComponentTag;
	std::shared_ptr<CObject>		m_pOwner;

};