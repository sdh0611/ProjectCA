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
	virtual void Init() = 0;
	virtual void Update(double dDeltaTime) = 0;
	virtual void LateUpdate() = 0;


public:
	CObject* const GetOwner() { return m_pOwner; }
	void SetOwner(CObject* pOwner) { m_pOwner = pOwner; }
	const TSTRING& GetComponentTag() const { return m_strComponentTag; }
	void SetComponentName(const TSTRING& strTag) { m_strComponentTag = strTag; }
	void SetActive(bool bActive) { m_bActive = bActive; }
	bool IsActive() const { return m_bActive; }


protected:
	bool					m_bActive;
	TSTRING				m_strComponentTag;
	CObject*				m_pOwner;

};