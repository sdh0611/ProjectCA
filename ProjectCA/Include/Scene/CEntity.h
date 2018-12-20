#pragma once


#include "..\..\stdafx.h"

class CScene;
class CLayer;
class CCamera;
class ComponentBase;


class CEntity
{
public:
	CEntity();
	virtual ~CEntity();


public:
	virtual bool PostInit(const ENTITY_DATA& data, CScene* pScene);
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
	void			SetActive(bool bActive);
	void			SetEntitySize(UINT iWidth, UINT iHeight);
	void			SetEntityWidth(UINT iWidth);
	void			SetEntityHeight(UINT iHeight);
	void			SetOwnerLayer(CLayer* pLayer);
	void			SetOwnerScene(CScene* pScene);
	void			SetEntityPosition(POSITION position);
	void			SetEntityPosition(float fx, float fy);
	void			SetEntityName(const TSTRING& strName);
	bool			AttachCamera(std::shared_ptr<CCamera> pCamera);


public:
	bool													IsActive();
	UINT													GetEntityWidth() const;
	UINT													GetEntityHeight() const;
	ENTITY_ID											GetEntityID() const;
	CLayer* const										GetOwnerLayer() const;
	CScene* const										GetOwnerScene() const;
	POSITION											GetEntityPosition() const;
	const TSTRING&									GetEntityName() const;
	std::weak_ptr<class TransformComponent>	GetTransform();
	std::weak_ptr<CCamera>						GetCamera();


protected:
	bool								m_bActive;
	UINT								m_iEntityWidth;
	UINT								m_iEntityHeight;
	ENTITY_ID						m_EntityID;
	TSTRING							m_strEntityName;
	CScene*							m_pOwnerScene;
	CLayer*							m_pOwnerLayer;
	std::weak_ptr<CCamera>	m_pCamera;


protected:
	typedef std::unordered_map<Types::tstring, std::shared_ptr<ComponentBase>> ComponentTable;
	ComponentTable	m_ComponentTable;

};