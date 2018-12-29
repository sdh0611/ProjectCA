#pragma once

#include "..\..\stdafx.h"
#include "CEntity.h"

class ComponentBase;
class CParticle;

class CObject : public CEntity
{
public:
	CObject();
	virtual ~CObject();


public:
	virtual bool PostInit(const OBJECT_DATA& data, CScene* pScene);
	virtual void Init();
	virtual void Update(double dDeltaTime);
	virtual void Render(const HDC& hDC) = 0;
	virtual void LateUpdate();


public:
	CObject * Clone();


public:
	void			SetActive(bool bActive);
	void			SetObjectSize(UINT iWidth, UINT iHeight);
	void			SetObjectWidth(UINT iWidth);
	void			SetObjectHeight(UINT iHeight);
	void			SetObjectPosition(POSITION position);
	void			SetObjectPosition(float fx, float fy);
	void			SetObjectState(OBJECT_STATE state); 
	void			SetObjectName(const TSTRING& strName);
	virtual void SetOwnerObject(std::shared_ptr<CObject> pOwner);


public:
	bool												IsActive();
	bool												IsSubordinate();
	UINT												GetObjectWidth() const;
	UINT												GetObjectHeight() const;
	POSITION										GetObjectPosition();
	ENTITY_ID										GetObjectID() const;
	OBJECT_TYPE									GetObjectType() const;
	OBJECT_STATE									GetObjectState() const;
	CLayer* const									GetOwnerLayer() const;
	CScene* const									GetOwnerScene() const;
	const TSTRING&								GetObjectName();
	std::weak_ptr<CObject>						GetOwnerObject();


public:
	void	AddParticle(std::shared_ptr<CParticle> pParticle);
	void	DeleteParticle();


protected:
	virtual void HandlingEvent(EVENT_TYPE type) { }


protected:
	OBJECT_TYPE					m_ObjectType;
	OBJECT_STATE					m_ObjectState;
	std::weak_ptr<CObject>		m_pOwnerObject;
	std::weak_ptr<CParticle>		m_pParticle;


private:
	//대입연산자 막아놓음.
	CObject & operator=(CObject* other) = delete;

};