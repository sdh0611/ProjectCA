#pragma once

#include "..\..\stdafx.h"


class CLayer;
class CScene;
class CCamera;
class ComponentBase;
class TransformComponent;

class CObject 
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
	std::weak_ptr<ComponentBase>	GetComponent(const TSTRING& strTag);
	bool										AddComponent(std::shared_ptr<ComponentBase> pComponent, const TSTRING& strTag);
	bool										DeleteComponent(const TSTRING& strTag);
	template<typename T>
	std::weak_ptr<T>	GetComponent() const
	{
		const std::type_info& type = typeid(T);
		for (auto& it : m_ComponentTable)
		{
			if (typeid(*(it.second)) == type)
				return STATIC_POINTER_CAST(T, it.second);
		}

		return std::weak_ptr<T>();
	}


public:
	CObject * Clone();


public:
	void SetActive(bool bActive);
	void SetObjectSize(UINT iWidth, UINT iHeight);
	void SetObjectWidth(UINT iWidth);
	void SetObjectHeight(UINT iHeight);
	void SetObjectPosition(POSITION position);
	void SetObjectPosition(float fx, float fy);
	void SetObjectState(OBJECT_STATE state); 
	void SetObjectName(const TSTRING& strName);
	void SetOwnerLayer(CLayer* pLayer);
	void SetOwnerScene(CScene* pScene);
	void SetOwnerObject(std::shared_ptr<CObject> pOwner);
	bool AttachCamera(std::shared_ptr<CCamera> pCamera);



public:
	bool												IsActive();
	bool												IsSubordinate();
	UINT												GetObjectWidth() const;
	UINT												GetObjectHeight() const;
	POSITION										GetObjectPosition();
	OBJECT_ID										GetObjectID() const;
	OBJECT_TYPE									GetObjectType() const;
	OBJECT_STATE									GetObjectState() const;
	CLayer* const									GetOwnerLayer() const;
	CScene* const									GetOwnerScene() const;
	const TSTRING&								GetObjectName();
	std::weak_ptr<TransformComponent>	GetTransform();
	std::weak_ptr<CObject>						GetOwnerObject();
	std::weak_ptr<CCamera>					GetCamera();


protected:
	bool								m_bActive;
	UINT								m_iObjectWidth;
	UINT								m_iObjectHeight;
	TSTRING							m_strObjectName;
	TSTRING							m_strLayerTag;
	OBJECT_ID						m_ObjectID;
	OBJECT_TYPE					m_ObjectType;
	OBJECT_STATE					m_ObjectState;
	CScene*							m_pOwnerScene;
	std::weak_ptr<CObject>		m_pOwnerObject;
	std::weak_ptr<CCamera>	m_pCamera;


protected:
	typedef std::unordered_map<Types::tstring, std::shared_ptr<ComponentBase>> ComponentTable;
	ComponentTable	m_ComponentTable;


private:
	CLayer*						m_pOwnerLayer;


private:
	//대입연산자 막아놓음.
	CObject & operator=(CObject* other) = delete;

};