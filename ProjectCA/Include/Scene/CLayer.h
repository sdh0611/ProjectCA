#pragma once


#include "..\..\stdafx.h"
#include "CScene.h"
#include "Actor\CActor.h"

//class CObject;
//class CActor;

class CLayer {
	//Scene Class에서만 Layer 생성, 파괴 가능.
	friend class CScene;
	friend class CGameScene;

	//CWorld에서 Layer들을 관리하게끔 변경함으로 써 CWorld Class에서 생성과 파괴를 모두 할 수 있도록 변경(08.14)
	friend class CWorld;
private:
	//상기 friend지정 메소드를 제외한 외부에서의 생성을 막기 위해 private으로 선언
	//CLayer(int order, const Types::tstring& tag);
	CLayer();
	~CLayer();


public:
	bool Init(const Types::tstring& strTag, UINT iOrder);
	void Update(float fDeltaTime);
	void Render(const HDC& hDC);
	void LateUpdate(float fDeltaTime);

public:
	void AddActor(CActor* pActor);
	//Actor삭제 메소드는 2가지의 오버로딩 버전 제공.
	bool DeleteActor(ActorID actorID);
	bool DeleteActor(CActor* pActor);


public:
	inline const std::list<CActor*>& GetObjectList() const { return m_actorList; }
	CActor* FindActor(ActorID actorID);
	CActor* FindActor(CActor* pActor);


	//Object를 Actor로 대체하면 안쓰게 될 함수들.(08.14)
//public:
	//void AddObjectToLayer(CObject* object);
	//bool DeleteObjectFromLayer(CObject * object);


public:
	//inline const std::list<CObject*>& GetObjectList() const { return m_ObjectList; }
	inline const Types::tstring& GetLayerTag() const { return m_strLayerTag; }
	inline void SetLayerTag(const Types::tstring& tag) { m_strLayerTag = tag; }
	inline int GetLayerOrder() { return m_iOrder; }
	inline void SetLayerOrder(int order) { if (order >= 0) m_iOrder = order; }
	inline UINT GetLastActorNumber() const { return m_iActorNumber; }

private:



private:
	int											m_iOrder;
	UINT										m_iActorNumber;	//안쓰게될 것 같음.(08.14)
	Types::tstring							m_strLayerTag;
	std::list<CActor*>						m_actorList;

	//Actor로 대체하면서 필요없어짐.(08.14)
//private:
//	std::list<CObject*>					m_ObjectList;
//	std::list<CObject*>::iterator			m_it;



};