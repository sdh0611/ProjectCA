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
	if (!CEntity::PostInit(data, pScene))
	{
		return false;
	}

	m_ObjectType			= data.m_ObjectType;
	m_ObjectState			= data.m_ObjectState;

	return true;
}

void CObject::Init()
{
	for (const auto& component : m_ComponentTable)
	{
		component.second->Init();
	}
}


void CObject::Update(double dDeltaTime)
{
	CEntity::Update(dDeltaTime);
}

void CObject::LateUpdate()
{
	CEntity::LateUpdate();
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
	m_iEntityWidth		= iWidth;
	m_iEntityHeight	= iHeight;
}

void CObject::SetObjectWidth(UINT iWidth)
{
	m_iEntityWidth = iWidth;
}

void CObject::SetObjectHeight(UINT iHeight)
{
	m_iEntityHeight = iHeight;
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
	m_strEntityName = strName;
}

void CObject::SetOwnerObject(std::shared_ptr<CObject> pOwner)
{
	m_pOwnerObject = pOwner;
}


bool CObject::IsActive()
{
	return m_bActive;
}

bool CObject::IsSubordinate()
{
	return !m_pOwnerObject.expired();
}

bool CObject::IsDead()
{
	if (m_ObjectState == Types::OS_DEAD || m_ObjectState == Types::ACT_DESTROY)
	{
		return true;
	}

	return false;
}

UINT CObject::GetObjectWidth() const
{
	return m_iEntityWidth;
}

UINT CObject::GetObjectHeight() const
{
	return m_iEntityHeight;
}

POSITION CObject::GetObjectPosition()
{
	return GetTransform().lock()->GetPosition();
}

ENTITY_ID CObject::GetObjectID() const
{
	return m_EntityID;
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
	return m_strEntityName;
}

//std::weak_ptr<TransformComponent> CObject::GetTransform()
//{
//	return GetComponent<TransformComponent>();
//}

std::weak_ptr<CObject> CObject::GetOwnerObject()
{
	return m_pOwnerObject.lock();
}

void CObject::AddParticle(std::shared_ptr<CParticle> pParticle)
{
	m_pParticle = pParticle;
}

void CObject::DeleteParticle()
{
	m_pParticle.reset();
}
