#include "..\..\..\Include\Scene\Object\CObject.h"
#include "..\..\..\Include\Core\Components\ComponentBase.h"



CObject::CObject(Types::ObjectType type, Types::Point point, UINT iWidth, 
	UINT iHeight, Types::ObjectState state, Types::Direction dir)
	:m_ObjectType(type), m_ObjectState(state),  m_iObjectWidth(iWidth), 
	m_iObjectHeight(iHeight), m_ObjectDirection(dir), m_fObjectPoint(point)
{


}

CObject::~CObject()
{
	if(!m_ComponentTable.empty())
		for (m_it = m_ComponentTable.begin(); m_it != m_ComponentTable.end(); ++m_it) {
			SAFE_DELETE(m_it->second)
		}

	m_ComponentTable.clear();

}

bool CObject::Init()
{

	for (m_it = m_ComponentTable.begin(); m_it != m_ComponentTable.end(); ++m_it) {
		m_it->second->Init();
	}


	return true;
}

void CObject::Update(float fDeltaTime)
{

	//for (m_it = m_ComponentTable.begin(); m_it != m_ComponentTable.end(); ++m_it) {
	//	//MessageBox(NULL, TEXT("ObjectUpdate1"), TEXT("Info"), MB_ICONINFORMATION);
	//	m_it->second->Update(fDeltaTime);
	//}

	//충돌 여부와 관련없이 실행되어야 하는 컴포넌트들의 업데이트 수행.
	if(m_ComponentTable.find(TEXT("InputComponent")) != m_ComponentTable.end())
		m_ComponentTable[TEXT("InputComponent")]->Update(fDeltaTime);

}

void CObject::LateUpdate(float fDeltaTime)
{
	for (auto& component : m_ComponentTable) {
		if (component.first == TEXT("InputComponent"))
			continue;

		component.second->Update(fDeltaTime);
	}
}

//void CObject::Render()
//{
//	for (m_it = m_ComponentTable.begin(); m_it != m_ComponentTable.end(); ++m_it) {
//		m_it->second->Render();
//	}
//
//
//}

ComponentBase * CObject::GetComponent(const Types::tstring & tag)
{
	m_it = m_ComponentTable.find(tag);
	if( m_it == m_ComponentTable.end())
		return nullptr;

	return m_it->second;
}

bool CObject::AddComponent(const Types::tstring & tag, ComponentBase * component)
{
	if (GetComponent(tag) == nullptr) {
		m_ComponentTable.insert(std::make_pair(tag, component));
		return true;
	}

	return false;
}

bool CObject::DeleteComponent(const Types::tstring & tag){

	ComponentBase* component = GetComponent(tag);
	if (component == nullptr)
		return false;

	SAFE_DELETE(component)
	m_ComponentTable.erase(m_it);

	return true;
}
