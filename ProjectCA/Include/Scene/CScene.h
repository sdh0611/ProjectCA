#pragma once

/*
	NOTE:
		게임 내 모든 Scene들의 기본 동작들을 구현해놓은 Class.
		Layer객체들을 List로 관리함.
		Scene의 구체적인 동작은 이 Class를 상속한 후 따로 구현해야함.
*/

//TODO(09.15) : Layer 개념 없애버리고 일단 빌드부터 되게끔 만들자.
#include "..\..\stdafx.h"

class CObject;
class CCamera;
class CLayer;
class CEntity;

typedef std::shared_ptr<CEntity> EntityPtr;
typedef std::weak_ptr<CEntity> WeakEntityPtr;

class CScene {

public:
	CScene(Types::SceneType);
	virtual ~CScene();


public:
	void			StartScene();
	virtual bool Init();
	virtual void Update(double dDeltaTime);
	virtual void Render(const HDC& hDC);
	

public:
	bool		CreateLayer(const TSTRING& strTag, int order);	//Layer생성이 가능한 유일한 메소드
	bool		DeleteLayer(const TSTRING& strTag);
	CLayer*	FindLayer(const TSTRING& strTag);


public:
	void				AddEntityToScene(CEntity* pEntity);
	void				AddEntityToScene(std::shared_ptr<CEntity> pEntity);
	bool				DeleteEntityFromScene(CEntity* pEntity);
	bool				DeleteEntityFromScene(UINT entityID);
	WeakEntityPtr	FindEntityFromScene(CEntity* pEntity);
	WeakEntityPtr	FindEntityFromScene(const TSTRING& strEntityName);
	WeakEntityPtr	FindEntityFromScene(UINT entityID);
	WeakEntityPtr	FindInactiveEntityFromScene(CEntity* pEntity);
	WeakEntityPtr	FindInactiveEntityFromScene(const TSTRING& strEntityName);


public:
	void SetSceneMainCamera(std::shared_ptr<CCamera> pCamera);
	

public:
	virtual void ResetScene();


protected:
	void CheckGarbage();


private:
	static bool CompareLayer(CLayer* first, CLayer* second);
	void SortLayer() { m_LayerList.sort(CompareLayer); }


protected:
	bool									m_bActive;
	Types::SceneType					m_SceneType;
	std::list<CLayer*>					m_LayerList;
	std::weak_ptr<CCamera>		m_pMainCameraPtr;


protected:
	typedef std::list<EntityPtr>	StrongEntityPtrList;
	StrongEntityPtrList					m_EntityPtrList;

};
