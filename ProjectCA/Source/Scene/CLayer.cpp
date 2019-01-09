#include "..\..\Include\Scene\CLayer.h"
#include "..\..\Include\Scene\CEntity.hpp"
#include "..\..\Include\Scene\Actor\CActor.h"
#include "..\..\Include\Scene\CWorld.h"
#include "..\..\Include\Core\Components\RenderComponent.h"


CLayer::CLayer() 
{

}

CLayer::~CLayer()
{
	Destroy();
	m_EntityList.clear();

}

bool CLayer::Init(const Types::tstring& strTag, UINT iOrder)
{
	m_bVisible			= true;
	m_iOrder				= iOrder;
	m_strLayerTag		= strTag;

	return true;
}

//Layer Class의 역할을 Rendering으로만 축소함으로 써 Update에서는 expired된 weak_ptr만 체크
void CLayer::Update(double fDeltaTime)
{
	if (!m_EntityList.empty())
		for (auto it = m_EntityList.begin(); it != m_EntityList.end(); ++it) {
			if ((*it).expired()) {
				m_EntityList.erase(it);
				break;
			}
		}
}

void CLayer::Render(const HDC& hDC)
{
	if (m_bVisible)
	{
		if (!m_EntityList.empty())
		{
			for (const auto& entity : m_EntityList)
			{
				if (entity.lock()->IsActive())
				{
					entity.lock()->Render(hDC);
				}
			}
		}
	}
}

void CLayer::Destroy()
{
	for(auto it = m_EntityList.begin(); it != m_EntityList.end(); ++it)
	{
		if (!it->expired())
		{
			it->lock()->SetOwnerLayer(nullptr);
		}
			
	}
}

void CLayer::AddActor(std::shared_ptr<CEntity> pEntity)
{
	if (pEntity->GetOwnerLayer())
	{
		pEntity->GetOwnerLayer()->DeleteActor(pEntity);
	}
	pEntity->SetOwnerLayer(this);
	m_EntityList.emplace_back(pEntity);

}

bool CLayer::DeleteActor(ENTITY_ID id)
{
	for (auto it = m_EntityList.cbegin(); it != m_EntityList.cend(); ++it)
	{
		if (it->lock()->GetEntityID() == id)
		{
			m_EntityList.erase(it);
			return true;
		}
	}

	return false;
}

bool CLayer::DeleteActor(std::shared_ptr<CEntity>& pEntity)
{
	for (auto it = m_EntityList.begin(); it != m_EntityList.end(); ++it) {
		if (it->lock() == pEntity)
		{
			it->lock()->SetOwnerLayer(nullptr);
			m_EntityList.erase(it);
			return true;
		}
	}

	return false;
}

void CLayer::FadeOut()
{
	for (const auto& entity : m_EntityList)
	{
		STATIC_POINTER_CAST(RenderComponent, entity.lock()->GetComponent(TEXT("RenderComponent")).lock())->SetRenderMode(RenderComponent::RenderMode::RENDER_FADE_OUT);
	}
 
}

void CLayer::FadeIn()
{
	for (const auto& entity : m_EntityList)
	{
		entity.lock()->GetComponent<RenderComponent>().lock()->SetRenderMode(RenderComponent::RenderMode::RENDER_FADE_IN);
	}
}


std::weak_ptr<CEntity> CLayer::FindEntity(ENTITY_ID id)
{
	for (auto it = m_EntityList.cbegin(); it != m_EntityList.cend(); ++it)
	{
		if (it->lock()->GetEntityID() == id)
		{
			return it->lock();
		}
	}
	
	return std::weak_ptr<CEntity>();
}

std::weak_ptr<CEntity> CLayer::FindEntity(const std::shared_ptr<CEntity>& pEntity)
{
	for (const auto& it : m_EntityList) {
		if (it.lock() == pEntity)
			return it;
	}

	return std::weak_ptr<CActor>();	//return nullptr
}

const std::list<std::weak_ptr<CEntity>>& CLayer::GetEntityList() const
{
	return m_EntityList;
}
