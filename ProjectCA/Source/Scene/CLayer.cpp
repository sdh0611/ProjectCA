#include "..\..\Include\Scene\CLayer.h"
#include "..\..\Include\Scene\Object\CObject.h"



CLayer::CLayer(int order, const Types::tstring & tag):
	m_iOrder(order), m_iObjectNumber(0), m_strLayerTag(tag)
{
}

CLayer::~CLayer()
{
	if(!m_ObjectList.empty())
		for (m_it = m_ObjectList.begin(); m_it != m_ObjectList.end(); ++m_it) {
			SAFE_DELETE((*m_it))
		}

	m_ObjectList.clear();

}

//	TODO(08.06) : 
//		ObjectType별로 Init값을 달리해줘야함
bool CLayer::Init()
{
	if (!m_ObjectList.empty())
		for (m_it = m_ObjectList.begin(); m_it != m_ObjectList.end(); ++m_it) {
			//switch ((*m_it)->GetObjectType()) {
			//case Types::OT_PLAYER:

			//	break;
			//case Types::OT_PROBS:
			//	
			//	break;
			//case Types::OT_ENEMY:

			//	break;
			//}

		}


	return true;
}

void CLayer::Update(float fDeltaTime)
{
	if (!m_ObjectList.empty())
		for (m_it = m_ObjectList.begin(); m_it != m_ObjectList.end(); ++m_it) {
			(*m_it)->Update(fDeltaTime);
		}
}

void CLayer::Render(const HDC& hDC)
{
	if (!m_ObjectList.empty())
		for (m_it = m_ObjectList.begin(); m_it != m_ObjectList.end(); ++m_it) {
			(*m_it)->Render(hDC);
		}
}

void CLayer::LateUpdate(float fDeltaTime)
{
	for (auto& object : m_ObjectList) {
		object->LateUpdate(fDeltaTime);
	}

}

void CLayer::AddObjectToLayer(CObject * object)
{
	m_ObjectList.push_back(object);
	++m_iObjectNumber;
}

bool CLayer::DeleteObjectFromLayer(CObject * object)
{
	if( !m_ObjectList.empty() )
		for (m_it = m_ObjectList.begin(); m_it != m_ObjectList.end(); ++m_it) {
			if ((*m_it) == object) {
				SAFE_DELETE((*m_it))
				m_ObjectList.erase(m_it);
				--m_iObjectNumber;
				return true;
			}
		}

	return false;
}
