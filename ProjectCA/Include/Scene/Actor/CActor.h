#pragma once

/*
	NOTE:
		게임 내 모든 Actor들의 기본 Class 정의.
		CWorld객체에서만 생성가능하게 설정.
*/

#include "..\..\..\stdafx.h"
#include "..\"
//#include "CActorManager.h"



class CWorld;
class CGameScene;
class ComponentBase;

class CActor {
	////Layer, World Class만 Actor의 생성, 파괴가 가능함.
	//friend class CWorld;
	//friend class CLayer;
	
	//08:17 : ActorFactory에서만 생성 가능하게 바꿈.
	//파괴의 경우엔 Actor 자체의 Destroy 메소드를 이용하여 자체적 파괴가 가능하게 수정.
	//friend class CActorFactory;
	friend class CActorManager;

private:
	CActor();
	virtual ~CActor();
	

public:
	virtual bool Init(const Types::ObjectData&);
	virtual void Update(float fDeltaTime);
	virtual void Render(const HDC& hDC);
	virtual void LateUpdate(float fDeltaTime);
	virtual void Destroy();


public:
	ComponentBase * GetComponent(const Types::tstring& strTag);
	bool AddComponent(ComponentBase* pComponent, const Types::tstring& strTag);
	bool DeleteComponent(const Types::tstring& strTag);

public:
	inline bool IsActive() const { return m_bActive; }
	inline void SetActive(bool bActive) { m_bActive = bActive; }
	Types::ObjectType	GetActorType() const { return m_actorType; }
	Types::ObjectState GetActorState() const { return m_actorState; }
	void SetActorState(Types::ObjectState state) { m_actorState = state; }
	Types::Direction GetActorDirection() const { return m_direction; }
	void SetActorDirection(Types::Direction dir) { m_direction = dir; }
	UINT GetActorWidth() const { return m_iActorWidth; }
	void SetActorWidth(UINT iWidth) { m_iActorWidth = iWidth; }
	UINT GetActorHeight() const { return m_iActorHeight; }
	void SetActorHeight(UINT iHeight) { m_iActorHeight = iHeight; }
	Types::Point GetActorPoint() const { return m_fActorPoint; }
	bool SetActorPoint(float fx, float fy) {
		if (fx < 0 || fy < 0)
			return false;

		m_fActorPoint.x = fx;
		m_fActorPoint.y = fy;
		
		return true;
	}
	inline Types::ActorID GetActorID() const { return m_actorID; }	
	CWorld* const GetOwnerWorld() const;
	void SetOwnerWorld(CWorld* pWorld);
	CGameScene* const GetOwnerScene() const;
	void SetOwnerScene(CGameScene* pScene);

private:
	//Actor, World, Layer클래스 마저 손보고, Git에다가 새로운 브랜치를 만들어서 Push하자.


protected:
	bool						m_bActive;
	UINT						m_iActorWidth;
	UINT						m_iActorHeight;
	Types::Point				m_fActorPoint;
	Types::ObjectType		m_actorType;
	Types::ObjectState		m_actorState;
	Types::Direction		m_direction;
	Types::ActorID			m_actorID;
	CWorld*					m_pOwnerWorld;
	CGameScene*			m_pOwnerScene;


protected:
	typedef std::unordered_map<Types::tstring, class ComponentBase*> ComponentTable;
	ComponentTable		m_componentTable;


};