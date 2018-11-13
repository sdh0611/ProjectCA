#pragma once

/*
	NOTE:
		게임 내 모든 Actor들의 기본 Class 정의.
		CWorld객체에서만 생성가능하게 설정.
*/

#include "..\..\..\stdafx.h"
//#include "..\"
//#include "CActorManager.h"


class CWorld;
class CGameScene;
class ComponentBase;

//typedef std::shared_ptr<ComponentBase> StrongComponentPtr;

class CActor {
	////Layer, World Class만 Actor의 생성, 파괴가 가능함.
	//friend class CWorld;
	//friend class CLayer;
	
	//08:17 : ActorFactory에서만 생성 가능하게 바꿈.
	//파괴의 경우엔 Actor 자체의 Destroy 메소드를 이용하여 내부 컴포넌트들을 정리하게끔 함.
	//friend class CActorFactory;
	//friend class로 ActorManager를 했던건 ActorManager에서만 생성 가능하게끔 하려는 의도였으나,
	//스마트포인터 사용할 때 생기는 문제를 도저히 못고치겠어서 걍 생성자를 Public으로 풀었다..(09.15)
	friend class CActorManager;

public:
	CActor();
	virtual ~CActor();
	

public:
	virtual bool PostInit(const Types::ActorData&, CGameScene*) = 0;
	virtual bool Init() = 0;
	virtual void Update(double dDeltaTime);
	virtual void Render(const HDC& hDC) = 0;
	virtual void LateUpdate(double dDeltaTime);
	virtual void Destroy();


public:
	std::weak_ptr<ComponentBase>	GetComponent(const TSTRING& strTag);
	bool										AddComponent(std::shared_ptr<ComponentBase> pComponent, const TSTRING& strTag);
	bool										DeleteComponent(const TSTRING& strTag);
	template<typename T>
	std::weak_ptr<T>	GetComponent() const
	{
		const std::type_info& type = typeid(T);
		for (auto& it : m_componentTable)
		{
			if (typeid(*(it.second)) == type)
				return STATIC_POINTER_CAST(T, it.second);
		}

		return std::weak_ptr<T>();
	}

public:
	void SetActive(bool bActive);
	void SetActorState(Types::ActorState state);
	void SetActorDirection(Types::Direction dir);
	void SetActorWidth(UINT iWidth);
	void SetActorHeight(UINT iHeight);
	void SetActorPoint(float fx, float fy);
	void SetActorTag(const Types::tstring& strTag);
	void SetActorVerticalState(Types::VerticalState vertical);
	void SetActorHorizonalState(Types::HorizonalState horizonal);
	void SetOwnerScene(CGameScene* pScene);
	//void SetOwnerWorld(CWorld* pWorld);
	bool SetLayer(const Types::tstring& strLayerTag);


public:
	bool						IsActive() const;
	Types::ActorType		GetActorType() const;
	Types::ActorState		GetActorState() const;
	Types::Direction		GetActorDirection() const;
	UINT						GetActorWidth() const;
	UINT						GetActorHeight() const;
	Types::Point				GetActorPoint() const;
	const Types::Point		GetActorPivot() const;
	const Types::tstring& GetActorTag() const;
	Types::ActorID			GetActorID() const;
	Types::VerticalState	GetActorVerticalState() const;
	Types::HorizonalState GetActorHorizonalState() const;
	//CWorld* const			GetOwnerWorld() const;
	CGameScene* const	GetOwnerScene() const;


public:
	void FlipActorDirection();


private:
	virtual void ActorBehavior(double dDeltaTime) = 0;


protected:
	UINT							m_iActorWidth;
	UINT							m_iActorHeight;
	Types::ActorType			m_actorType;
	Types::ActorState			m_actorCurState;
	Types::VerticalState		m_actorCurVerticalState;
	Types::HorizonalState		m_actorHorizonalState;
	Types::Direction			m_direction;
	Types::ActorID				m_actorID;
	Types::tstring				m_strActorTag;		 
	//CWorld*					m_pOwnerWorld;
	CGameScene*				m_pOwnerScene;
	bool							m_bActive;


protected:
	typedef std::unordered_map<Types::tstring, std::shared_ptr<ComponentBase>> ComponentTable;
	ComponentTable		m_componentTable;

	
};