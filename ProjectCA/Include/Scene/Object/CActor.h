#pragma once

#include "..\..\..\stdafx.h"


typedef unsigned long ActorID;
typedef std::shared_ptr<class CActorComponent> StrongActorComponentPtr;


class CActor {

public:
	CActor(ActorID id);
	virtual ~CActor();


public:
	bool Init();
	void Update(float fDeltaTIme);
	void Render(const HDC& hDC);
	void Destroy();


public:
	template<class ComponentType>
	std::weak_ptr<ComponentType> GetComponent(const Types::tstring& strTag) {
		auto it = m_componentTable.find(strTag);

		if (it == m_componentTable.end())
			return nullptr;
		else {
			StrongActorComponentPtr pComponent(it->second);
			std::shared_ptr<ComponentType> pTemp(std::tr1::static_pointer_cast
				<ComponentType>(pComponent));
			std::weak_ptr<ComponentType> pRet(pTemp);

			return pRet;
		}
	}
	void AddComponent(StrongActorComponentPtr pComponent);


private:



protected:
	typedef std::unordered_map<Types::tstring, StrongActorComponentPtr> ComponentTable;
	ComponentTable			m_componentTable;
	ActorID						m_actorID:




};