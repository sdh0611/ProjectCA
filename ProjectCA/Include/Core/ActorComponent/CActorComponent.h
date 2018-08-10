#pragma once

#include "..\..\..\stdafx.h"


typedef std::shared_ptr<class CActor> StrongActorPtr;

class CActorComponent {

public:
	CActorComponent() = default;
	virtual ~CActorComponent() { };


public:
	virtual bool Init() = 0;
	virtual void Update(float fDeltaTime);
	

public:
	void SetOwner(StrongActorPtr pOwner) { m_pOwner = pOwner; }
	std::weak_ptr<CActor> const GetOwner() { return m_pOwner; }
	void SetTag(const Types::tstring& strTag) { m_strComponentTag = strTag; }
	const Types::tstring& GetTag() const { return m_strComponentTag; }


protected:
	StrongActorPtr		m_pOwner;
	Types::tstring		m_strComponentTag;


};