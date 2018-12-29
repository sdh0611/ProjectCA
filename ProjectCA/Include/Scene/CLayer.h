#pragma once

/*
	NOTE:
		Scene내부 같은 카테고리의 Object(or Actor)들을 관리하기 위한 Class.
		Object(or Actor)들의 shared_ptr을 관리하고, 다른 Class에는 weak_ptr을 반환하도록 함.(08.17);
		->Layer Class의 역할을 Rendering만으로 축소하기로(08.31)
*/

#include "..\..\stdafx.h"
#include "CScene.h"

class CActor;
class CObject;
class CEntity;

class CLayer {
	//Scene Class에서만 Layer 생성, 파괴 가능.
	friend class CScene;
	friend class CGameScene;


private:
	//상기 friend지정 메소드를 제외한 외부에서의 생성을 막기 위해 private으로 선언
	//CLayer(int order, const Types::tstring& tag);
	CLayer();
	~CLayer();


public:
	bool Init(const Types::tstring& strTag, UINT iOrder);
	void Update(double fDeltaTime);
	void Render(const HDC& hDC);
	void Destroy();


public:
	void AddActor(std::shared_ptr<CEntity> pEntity);
	//Actor삭제 메소드는 2가지의 오버로딩 버전 제공.
	//bool DeleteActor(Types::ActorID actorID);
	bool DeleteActor(std::shared_ptr<CEntity>& pEntity);


public:
	//std::weak_ptr<CObject> FindObject(Types::ActorID actorID);
	std::weak_ptr<CEntity> FindEntity(const std::shared_ptr<CEntity>& pEntity);
	const std::list<std::weak_ptr<CEntity>>& GetEntityList() const;


public:
	inline const TSTRING& GetLayerTag() const { return m_strLayerTag; }
	inline void SetLayerTag(const TSTRING& tag) { m_strLayerTag = tag; }
	inline int GetLayerOrder() { return m_iOrder; }
	inline void SetLayerOrder(int order) { if (order >= 0) m_iOrder = order; }
	inline UINT GetLastActorNumber() const { return m_iActorNumber; }


private:
	int												m_iOrder;
	UINT											m_iActorNumber;	//안쓰게될 것 같음.(08.14)
	Types::tstring								m_strLayerTag;
	std::list<std::weak_ptr<CEntity>>		m_EntityList;


};