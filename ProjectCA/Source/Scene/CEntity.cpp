#include "..\..\stdafx.h"
#include "..\..\Include\Scene\CEntity.hpp"
#include "..\..\Include\Core\Components\ComponentBase.h"
#include "..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\Include\Core\Components\RenderComponent.h"
#include "..\..\Include\Scene\CLayer.h"
#include "..\..\Include\Scene\CCameraManager.h"


CEntity::CEntity()
{
}

CEntity::~CEntity()
{
}

bool CEntity::PostInit(const ENTITY_DATA & data, CScene * pScene)
{
	m_bActive			= true;
	m_iEntityWidth		= data.m_iEntityWidth;
	m_iEntityHeight	= data.m_iEntityHeight;
	m_EntityID			= data.m_EntityID;
	m_strEntityName	= data.m_strEntityName;

	m_pOwnerScene	= pScene;
	m_pOwnerLayer	= nullptr;

	auto pTransform = std::make_shared<TransformComponent>();
	if (!pTransform->PostInit(this, data.m_EntityPoint))
	{
		return false;
	}

	if (!AddComponent(pTransform, pTransform->GetComponentTag()))
	{
		return false;
	}	

	return true;
}

void CEntity::Init()
{
	SetActive(true);
	for (const auto& component : m_ComponentTable)
	{
		component.second->Init();
	}
}

void CEntity::Update(double dDeltaTime)
{
	for (const auto& component : m_ComponentTable)
	{
		component.second->Update(dDeltaTime);
	}
}

void CEntity::Render(const HDC & hDC)
{
	auto pRender = GetComponent(TEXT("RenderComponent"));
	if (!pRender.expired())
	{
		STATIC_POINTER_CAST(RenderComponent, pRender.lock())->Draw(hDC);
	}
}

void CEntity::LateUpdate()
{
	if (CCameraManager::GetInstance()->GetMainCamera().expired())
		return;

	GetTransform().lock()->AdjustScreenPosition();

	auto pRender = GetComponent(TEXT("RenderComponent"));
	if (!pRender.expired())
	{
		pRender.lock()->LateUpdate();
	}
}

std::weak_ptr<ComponentBase> CEntity::GetComponent(const TSTRING & strTag)
{
	auto it = m_ComponentTable.find(strTag);

	if (it == m_ComponentTable.end())
		return std::weak_ptr<ComponentBase>();

	return it->second;
}

bool CEntity::AddComponent(std::shared_ptr<ComponentBase> pComponent, const TSTRING & strTag)
{
	return m_ComponentTable.insert(std::make_pair(strTag, pComponent)).second;
}

bool CEntity::DeleteComponent(const TSTRING & strTag)
{
	ComponentTable::iterator it = m_ComponentTable.find(strTag);

	if (it == m_ComponentTable.end())
		return false;

	m_ComponentTable.erase(strTag);

	return true;
}

void CEntity::SetActive(bool bActive)
{
	for (const auto& component : m_ComponentTable)
	{
		component.second->SetActive(bActive);
	}

	m_bActive = bActive;
}

void CEntity::SetEntitySize(UINT iWidth, UINT iHeight)
{
	m_iEntityWidth = iWidth;
	m_iEntityHeight = iHeight;
}

void CEntity::SetEntityWidth(UINT iWidth)
{
	m_iEntityWidth = iWidth;
}

void CEntity::SetEntityHeight(UINT iHeight)
{
	m_iEntityHeight = iHeight;
}

void CEntity::SetOwnerLayer(CLayer * pLayer)
{
	m_pOwnerLayer = pLayer;
}

void CEntity::SetOwnerScene(CScene * pScene)
{
	m_pOwnerScene = pScene;
}

void CEntity::SetEntityPosition(POSITION position)
{
	GetComponent<TransformComponent>().lock()->SetPosition(position);
}

void CEntity::SetEntityPosition(float fx, float fy)
{
	GetComponent<TransformComponent>().lock()->SetPosition(fx, fy);
}

void CEntity::SetEntityName(const TSTRING & strName)
{
	m_strEntityName = strName;
}

bool CEntity::AttachCamera(std::shared_ptr<CCamera> pCamera)
{
	if (!m_pCamera.expired())
		return false;

	m_pCamera = pCamera;

	return true;
}

bool CEntity::IsActive()
{
	return m_bActive;
}

UINT CEntity::GetEntityWidth() const
{
	return m_iEntityWidth;
}

UINT CEntity::GetEntityHeight() const
{
	return m_iEntityHeight;
}

ENTITY_ID CEntity::GetEntityID() const
{
	return m_EntityID;
}

CLayer * const CEntity::GetOwnerLayer() const
{
	return m_pOwnerLayer;
}

CScene * const CEntity::GetOwnerScene() const
{
	return m_pOwnerScene;
}

POSITION CEntity::GetEntityPosition() const
{
	return GetComponent<TransformComponent>().lock()->GetPosition();
}

const TSTRING & CEntity::GetEntityName() const
{
	return m_strEntityName;
}

std::weak_ptr<class TransformComponent> CEntity::GetTransform()
{
	return GetComponent<TransformComponent>().lock();
}

std::weak_ptr<CCamera> CEntity::GetCamera()
{
	return m_pCamera.lock();
}