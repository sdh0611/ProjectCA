#include "..\..\Include\Scene\CLayer.h"
#include "..\..\Include\Scene\Object\CObject.h"



CLayer::CLayer(int order, const Types::tstring & tag):
	m_iOrder(order), m_strLayerTag(tag)
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

bool CLayer::Init()
{
	if (!m_ObjectList.empty())
		for (m_it = m_ObjectList.begin(); m_it != m_ObjectList.end(); ++m_it) {
			if (!(*m_it)->Init())
				return false;
		}


	return true;
}

void CLayer::Update(const double& deltaTime)
{
	if (!m_ObjectList.empty())
		for (m_it = m_ObjectList.begin(); m_it != m_ObjectList.end(); ++m_it) {
			(*m_it)->Update(deltaTime);
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
}

bool CLayer::DeleteObjectFromLayer(CObject * object)
{
	if( !m_ObjectList.empty() )
		for (m_it = m_ObjectList.begin(); m_it != m_ObjectList.end(); ++m_it) {
			if ((*m_it) == object) {
				SAFE_DELETE((*m_it))
				m_ObjectList.erase(m_it);
				return true;
			}
		}

	return false;
}
