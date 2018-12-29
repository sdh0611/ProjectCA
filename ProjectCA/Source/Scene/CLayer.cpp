#include "..\..\Include\Scene\CLayer.h"
#include "..\..\Include\Scene\CEntity.h"
#include "..\..\Include\Scene\Actor\CActor.h"
#include "..\..\Include\Scene\CWorld.h"
//#include "..\..\Include\Scene\Object\CObject.h"


//CLayer::CLayer(int order, const Types::tstring & tag):
//	m_iOrder(order), m_iObjectNumber(0), m_strLayerTag(tag)
//{
//}
CLayer::CLayer() 
{

}

CLayer::~CLayer()
{
	//if(!m_ObjectList.empty())
	//	for (m_it = m_ObjectList.begin(); m_it != m_ObjectList.end(); ++m_it) {
	//		SAFE_DELETE((*m_it))
	//	}

	//m_ObjectList.clear();

	//if (!m_ObjectList.empty())
	//	for (auto& it : m_ObjectList)
	//		SAFE_DELETE(it)

	Destroy();
	m_EntityList.clear();

}

//	TODO(08.06) : 
//		ObjectType별로 Init값을 달리해줘야함
//	->Init 메소드의 역할을 하위 Actor들의 초기화까지 담당하는 것이 아닌 
//		Layer의 속성 그 자체만 초기화하도록 변경함.(08.14)
bool CLayer::Init(const Types::tstring& strTag, UINT iOrder)
{
	//if (!m_ObjectList.empty())
		//for (m_it = m_ObjectList.begin(); m_it != m_ObjectList.end(); ++m_it) {
			//switch ((*m_it)->GetObjectType()) {
			//case Types::OT_PLAYER:

			//	break;
			//case Types::OT_PROBS:
			//	
			//	break;
			//case Types::OT_ENEMY:

			//	break;
			//}

		//}
	m_strLayerTag = strTag;
	m_iOrder = iOrder;
	m_iActorNumber = 0;

	return true;
}

//Update 순서 : Update -> ResolveCollision -> LateUpdate
//Layer Class의 역할을 Rendering으로만 축소함으로 써 Update에서는 expired된 weak_ptr만 체크하기로
void CLayer::Update(double fDeltaTime)
{
	//if (!m_ObjectList.empty())
	//	for (m_it = m_ObjectList.begin(); m_it != m_ObjectList.end(); ++m_it) {
	//		(*m_it)->Update(fDeltaTime);
	//	}
	
	//if (!m_ObjectList.empty()) {
	//	for (auto& it : m_ObjectList)
	//		it->Update(fDeltaTime);

	//	for (auto& it : m_ObjectList)
	//		it->LateUpdate(fDeltaTime);
	//}

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
	//if (!m_ObjectList.empty())
	//	for (m_it = m_ObjectList.begin(); m_it != m_ObjectList.end(); ++m_it) {
	//		(*m_it)->Render(hDC);
	//	}

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

//bool CLayer::DeleteActor(Types::ActorID actorID)
//{
//	std::weak_ptr<CActor> pActor = FindObject(actorID);
//	if (FindObject(actorID).lock() == nullptr)
//		return false;
//
//	for (auto it = m_ObjectList.begin(); it != m_ObjectList.end(); ) {
//		if ((*it).lock() == pActor.lock()) {
//			m_ObjectList.erase(it);
//			break;
//		}
//		else
//			++it;
//	}
//
//	return true;
//}

bool CLayer::DeleteActor(std::shared_ptr<CEntity>& pEntity)
{
	if (FindEntity(pEntity).lock() == nullptr)
		return false;

	for (auto it = m_EntityList.begin(); it != m_EntityList.end(); ) {
		if (it->lock() == pEntity)
		{
			it->lock()->SetOwnerLayer(nullptr);
			m_EntityList.erase(it);
			break;
		}
		else
		{
			++it;
		}
	}


	//auto func = [&](std::shared_ptr<CActor> strongPtr)->bool {
	//	if (ptr == strongPtr)
	//		return true;
	//	return false;
	//};
	//
	//m_ObjectList.remove_if(func);

	return true;
}


//
//std::weak_ptr<CObject> CLayer::FindObject(Types::ActorID actorID)
//{
//	for (const auto& it : m_ObjectList) {
//		if (it.lock()->GetActorID() == actorID)
//			return it;
//	}
//
//	return std::weak_ptr<CActor>();	//return nullptr
//}

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

//void CLayer::AddObjectToLayer(CObject * object)
//{
//	m_ObjectList.push_back(object);
//	++m_iObjectNumber;
//}
//
//bool CLayer::DeleteObjectFromLayer(CObject * object)
//{
//	if( !m_ObjectList.empty() )
//		for (m_it = m_ObjectList.begin(); m_it != m_ObjectList.end(); ++m_it) {
//			if ((*m_it) == object) {
//				SAFE_DELETE((*m_it))
//				m_ObjectList.erase(m_it);
//				--m_iObjectNumber;
//				return true;
//			}
//		}
//
//	return false;
//}
