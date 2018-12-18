#include "..\..\stdafx.h"
#include "..\..\Include\Scene\CObject.h"
#include "..\..\Include\Scene\Actor\CCamera.h"
#include "..\..\Include\Core\Components\ComponentBase.h"
#include "..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\Include\Core\Components\RenderComponent.h"
#include "..\..\Include\Scene\CCameraManager.h"
#include "..\..\Include\Scene\CLayer.h"


CObject::CObject()
{
}

CObject::~CObject()
{
	m_ComponentTable.clear();
	if (!m_pCamera.expired())
	{
		CCameraManager::GetInstance()->DeleteCamera(m_pCamera.lock());
	}

}
bool CObject::PostInit(const OBJECT_DATA & data, CScene* pScene)
{
	//Object 속성 초기화 및 Transform 컴포넌트 추가
	//Pivot ratio는 외부에서 따로 지정해줄 것.
	m_bActive				= true;
	m_iObjectWidth		= data.m_iObjectWidth;
	m_iObjectHeight		= data.m_iObjectHeight;
	m_strObjectName		= data.m_strObjectName;
	m_ObjectType			= data.m_ObjectType;
	m_ObjectState			= data.m_ObjectState;
	m_ObjectID				= data.m_iObjectID;
	m_strLayerTag			= TEXT("default");
	m_pOwnerScene		= pScene;

	auto pTransform = std::make_shared<TransformComponent>();
	if (!pTransform->PostInit(this, data.m_ObjectPoint))
		return false;

	if (!AddComponent(pTransform, pTransform->GetComponentTag()))
		return false;

	return true;
}

void CObject::Init()
{
	for (const auto& component : m_ComponentTable)
	{
		component.second->Init();
	}

	//if (!m_pCamera.expired())
	//{
	//	m_pCamera.lock()->Init();
	//}
}


void CObject::Update(double dDeltaTime)
{
	for (const auto& component : m_ComponentTable)
	{
		component.second->Update(dDeltaTime);
	}
}

void CObject::LateUpdate()
{
	if (CCameraManager::GetInstance()->GetMainCamera().expired())
		return;

	GetTransform().lock()->AdjustScreenPosition();

	//auto pCollider = GetComponent(TEXT("Collider"));
	//if (!pCollider.expired())
	//{
	//	pCollider.lock()->LateUpdate();
	//}

	auto pRender = GetComponent(TEXT("RenderComponent"));
	if (!pRender.expired())
	{
		pRender.lock()->LateUpdate();
	}

}


std::weak_ptr<ComponentBase> CObject::GetComponent(const TSTRING& strTag)
{
	auto it = m_ComponentTable.find(strTag);

	if (it == m_ComponentTable.end())
		return std::weak_ptr<ComponentBase>();

	return it->second;
}

bool CObject::AddComponent(std::shared_ptr<ComponentBase> pComponent, const TSTRING& strTag)
{
	return m_ComponentTable.insert(std::make_pair(strTag, pComponent)).second;
}

bool CObject::DeleteComponent(const TSTRING & strTag)
{
	ComponentTable::iterator it = m_ComponentTable.find(strTag);

	if (it == m_ComponentTable.end())
		return false;

	m_ComponentTable.erase(strTag);

	return true;
}

CObject * CObject::Clone()
{
	CObject* pClone(this);

	return pClone;
}

void CObject::SetActive(bool bActive)
{
	//puts("SetActive");
	for (const auto& component : m_ComponentTable)
	{
		component.second->SetActive(bActive);
	}

	m_bActive = bActive;
}

void CObject::SetObjectSize(UINT iWidth, UINT iHeight)
{
	m_iObjectWidth = iWidth;
	m_iObjectHeight = iHeight;
}

void CObject::SetObjectWidth(UINT iWidth)
{
	m_iObjectWidth = iWidth;
}

void CObject::SetObjectHeight(UINT iHeight)
{
	m_iObjectHeight = iHeight;
}

void CObject::SetObjectPosition(POSITION position)
{
	GetComponent<TransformComponent>().lock()->SetPosition(position);
}

void CObject::SetObjectPosition(float fx, float fy)
{
	GetComponent<TransformComponent>().lock()->SetPosition(fx, fy);
}

void CObject::SetObjectState(OBJECT_STATE state)
{
	m_ObjectState = state;
}

void CObject::SetObjectName(const TSTRING & strName)
{
	m_strObjectName = strName;
}

void CObject::SetOwnerLayer(CLayer * pLayer)
{
	m_pOwnerLayer = pLayer;
	if (pLayer)
	{
		m_strLayerTag = pLayer->GetLayerTag();
	}
	else
	{
		m_strLayerTag = TEXT("Default");
	}
}

void CObject::SetOwnerScene(CScene * pScene)
{
	m_pOwnerScene = pScene;
}

void CObject::SetOwnerObject(std::shared_ptr<CObject> pOwner)
{
	if (pOwner == nullptr)
	{

	}
	m_pOwnerObject = pOwner;
}

bool CObject::AttachCamera(std::shared_ptr<CCamera> pCamera)
{
	if (!m_pCamera.expired())
		return false;

	m_pCamera = pCamera;

	return true;
}

bool CObject::IsActive()
{
	return m_bActive;
}

bool CObject::IsSubordinate()
{
	return !m_pOwnerObject.expired();
}

UINT CObject::GetObjectWidth() const
{
	return m_iObjectWidth;
}

UINT CObject::GetObjectHeight() const
{
	return m_iObjectHeight;
}

POSITION CObject::GetObjectPosition()
{
	return GetTransform().lock()->GetPosition();
}

OBJECT_ID CObject::GetObjectID() const
{
	return m_ObjectID;
}

OBJECT_TYPE CObject::GetObjectType() const
{
	return m_ObjectType;
}

OBJECT_STATE CObject::GetObjectState() const
{
	return m_ObjectState;
}

CLayer * const CObject::GetOwnerLayer() const
{
	return m_pOwnerLayer;
}

CScene * const CObject::GetOwnerScene() const
{
	return m_pOwnerScene;
}

const TSTRING & CObject::GetObjectName()
{
	return m_strObjectName;
}

std::weak_ptr<TransformComponent> CObject::GetTransform()
{
	return GetComponent<TransformComponent>();
}

std::weak_ptr<CObject> CObject::GetOwnerObject()
{
	return m_pOwnerObject.lock();
}

std::weak_ptr<CCamera> CObject::GetCamera()
{
	return m_pCamera.lock();
}
