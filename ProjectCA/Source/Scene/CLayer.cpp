#include "..\..\Include\Scene\CLayer.h"
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

	if (!m_actorList.empty())
		for (auto& it : m_actorList)
			SAFE_DELETE(it)

			m_actorList.clear();
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

void CLayer::Update(float fDeltaTime)
{
	//if (!m_ObjectList.empty())
	//	for (m_it = m_ObjectList.begin(); m_it != m_ObjectList.end(); ++m_it) {
	//		(*m_it)->Update(fDeltaTime);
	//	}
	
	if (!m_actorList.empty())
		for (auto& it : m_actorList)
			it->Update(fDeltaTime);

}

void CLayer::Render(const HDC& hDC)
{
	//if (!m_ObjectList.empty())
	//	for (m_it = m_ObjectList.begin(); m_it != m_ObjectList.end(); ++m_it) {
	//		(*m_it)->Render(hDC);
	//	}

	if (!m_actorList.empty())
		for (auto& it : m_actorList)
			it->Render(hDC);
}

void CLayer::LateUpdate(float fDeltaTime)
{
	/*for (auto& object : m_ObjectList) {
		object->LateUpdate(fDeltaTime);
	}
*/
}

void CLayer::AddActor(CActor * pActor)
{
	m_actorList.insert(pActor);

}

bool CLayer::DeleteActor(ActorID actorID)
{
	CActor* pActor = FindActor(actorID);
	if (FindActor(actorID) == nullptr)
		return false;

	m_actorList.remove(pActor);

	return true;
}

bool CLayer::DeleteActor(CActor * pActor)
{
	if (FindActor(pActor) == nullptr)
		return false;
	
	m_actorList.remove(pActor);

	return true;
}



CActor * CLayer::FindActor(ActorID actorID)
{
	for (auto& it : m_actorList) {
		if (it->GetActorID() == actorID)
			return it;
	}

	return nullptr;
}

CActor * CLayer::FindActor(CActor * pActor)
{
	for (auto& it : m_actorList) {
		if (it == pActor)
			return it;
	}

	return nullptr;
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
