#pragma once


#include "..\..\stdafx.h"
#include "CScene.h"

class CObject;

class CLayer {
	//CScene의 CreateLayer 메소드를 통해서만 생성할 수 있게 함.
	friend bool CScene::CreateLayer(const Types::tstring& tag, int order);
	
private:
	//상기 friend지정 메소드를 제외한 외부에서의 생성을 막기 위해 private으로 선언
	CLayer(int order, const Types::tstring& tag);


public:
	~CLayer();


public:
	bool Init();
	void Update(const double& deltaTime);
	void Render(const HDC& hDC);
	void LateUpdate(float fDeltaTime);


public:
	void AddObjectToLayer(CObject* object);
	bool DeleteObjectFromLayer(CObject * object);

public:
	inline const std::list<CObject*>& GetObjectList() const { return m_ObjectList; }
	inline const Types::tstring& GetLayerTag() const { return m_strLayerTag; }
	inline void SetLayerTag(const Types::tstring& tag) { m_strLayerTag = tag; }
	inline int GetLayerOrder() { return m_iOrder; }
	inline void SetLayerOrder(int order) { if (order >= 0) m_iOrder = order; }

private:



private:
	int											m_iOrder;
	Types::tstring							m_strLayerTag;
	std::list<CObject*>					m_ObjectList;
	std::list<CObject*>::iterator			m_it;




};