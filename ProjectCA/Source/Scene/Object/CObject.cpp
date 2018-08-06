#include "..\..\..\Include\Scene\Object\CObject.h"
#include "..\..\..\Include\Core\Components\ComponentBase.h"
#include "..\..\..\Include\Core\Components\InputComponent.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Core\Components\Collider.h"


CObject::CObject(Types::ObjectType type)
	:m_ObjectType(type), m_pScene(nullptr)
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

/*
	TODO(08.06):
		Object와 Scene간의 상호작용을 위한 Scene포인터의 초기화 추가
		Scene, Layer, Object간의 초기화 방법에 대해 다시 생각해보자.
*/

bool CObject::Init(Types::Point point, UINT iWidth,
	UINT iHeight, Types::ObjectState state, Types::Direction dir)
{
	bool ret;

	m_ObjectState = state;
	m_ObjectDirection = dir;
	m_fObjectPoint = point;
	m_iObjectWidth = iWidth;
	m_iObjectHeight = iHeight;
	
	for(auto& it : m_ComponentTable)
		ret = InitComponents(it.second);

	return ret;
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

//오브젝트 초기화시 각 컴포넌트별로 기본 값으로 초기화함.
bool CObject::InitComponents(ComponentBase* pComponent)
{
	Types::tstring tag = pComponent->GetComponentTag();
	bool ret = true;
	
	if (!tag.compare(TEXT("PhysicsComponent"))) {
		PhysicsComponent* physics = static_cast<PhysicsComponent*>(pComponent);
		ret = physics->Init(400.f, 1300.f, -350.f);
	}
	else if (!tag.compare(TEXT("Collider"))) {
		Collider* collider = static_cast<Collider*>(pComponent);
		ret = collider->Init(m_fObjectPoint);
	}
	
	return ret;
}
