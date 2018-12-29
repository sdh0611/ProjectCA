#include "..\..\Include\Scene\CScene.h"
#include "..\..\Include\Scene\CLayer.h"
#include "..\..\Include\Scene\CObject.h"
#include "..\..\Include\Scene\CCameraManager.h"
#include "..\..\Include\Scene\Actor\CObjectManager.h"

CScene::CScene(Types::SceneType type):
	m_SceneType(type)
{

}

CScene::~CScene()
{
	if(!m_LayerList.empty())
		for (m_it = m_LayerList.begin(); m_it != m_LayerList.end(); ++m_it) {
			SAFE_DELETE((*m_it))
		}

	m_LayerList.clear();
	//CCameraManager::GetInstance()->Clear();
	m_EntityPtrList.clear();
	CObjectManager::GetInstance()->Clear();
}


void CScene::StartScene()
{
	CCameraManager::GetInstance()->ChangeMainCamera(m_pMainCameraPtr.lock());
}

bool CScene::Init()
{
	//Default 레이어 생성.
	//모든 Entity들은 생성시 기본적으로 DefaultLayer로 들어가게됨.
	if (!CreateLayer(TEXT("Default"), 0))
	{
		return false;
	}

	return true;
}

void CScene::Update(double dDeltaTime)
{
	if (!m_LayerList.empty())
		for (m_it = m_LayerList.begin(); m_it != m_LayerList.end(); ++m_it) {
			(*m_it)->Update(dDeltaTime);

		}

}

void CScene::Render(const HDC& hDC)
{
	if (!m_LayerList.empty())
		for (m_it = m_LayerList.begin(); m_it != m_LayerList.end(); ++m_it)
		{
			(*m_it)->Render(hDC);
		}

}

//CLayer 클래스의 friend 메소드.
//CreateLayer메소드를 통해서만 Layer 생성이 가능하게끔 만든다.
bool CScene::CreateLayer(const TSTRING & strTag, int order)
{
	if (FindLayer(strTag))
		return false;

	CLayer* pLayer = new CLayer;
	pLayer->Init(strTag, order);

	m_LayerList.emplace_back(pLayer);
	SortLayer();

	return true;
}

bool CScene::DeleteLayer(const TSTRING & strTag)
{
	if (!FindLayer(strTag))
		return false;

	SAFE_DELETE((*m_it));
	m_LayerList.erase(m_it);

	return true;
}

CLayer * CScene::FindLayer(const TSTRING & strTag)
{
	for (const auto& layer : m_LayerList) {
		if (!layer->GetLayerTag().compare(strTag))
			return layer;
	}

	return nullptr;
}

void CScene::AddEntityToScene(CEntity * pEntity)
{
	m_EntityPtrList.emplace_back(pEntity);
}

void CScene::AddEntityToScene(std::shared_ptr<CEntity> pEntity)
{
	FindLayer(TEXT("Default"))->AddActor(pEntity);
	m_EntityPtrList.push_back(pEntity);
}

WeakEntityPtr CScene::FindEntityFromScene(CEntity * pEntity)
{
	for (const auto& obj : m_EntityPtrList)
	{
		if (obj.get() == pEntity)
		{
			return obj;
		}
	}

	return WeakEntityPtr();
}

WeakEntityPtr CScene::FindEntityFromScene(const TSTRING & strEntityName)
{
	for (const auto& obj : m_EntityPtrList)
	{
		if (obj->GetEntityName() == strEntityName)
		{
			return obj;
		}
	}

	return WeakEntityPtr();
}

WeakEntityPtr CScene::FindInactiveEntityFromScene(CEntity * pEntity)
{
	for (const auto& obj : m_EntityPtrList)
	{
		if (obj.get() == pEntity)
		{
			return obj;
		}
	}

	return WeakEntityPtr();
}

WeakEntityPtr CScene::FindInactiveEntityFromScene(const TSTRING & strEntityName)
{
	for (const auto& obj : m_EntityPtrList)
	{
		if (obj->GetEntityName() == strEntityName)
		{
			return obj;
		}
	}
	

	return WeakEntityPtr();
}

void CScene::SetSceneMainCamera(std::shared_ptr<CCamera> pCamera)
{
	m_pMainCameraPtr = pCamera;
}

WeakEntityPtr CScene::FindEntityFromScene(UINT entityID)
{
	for (const auto& obj : m_EntityPtrList)
	{
		if (obj->GetEntityID() == entityID)
		{
			return obj;
		}

	}

	return WeakEntityPtr();
}

bool CScene::DeleteEntityFromScene(CEntity * pEntity)
{
	for (auto it = m_EntityPtrList.cbegin(); it != m_EntityPtrList.cend(); ++it)
	{
		if (it->get() == pEntity)
		{
			m_EntityPtrList.erase(it);
			return true;
		}
	}

	return false;
}

bool CScene::DeleteEntityFromScene(UINT entityID)
{
	for (auto it = m_EntityPtrList.cbegin(); it != m_EntityPtrList.cend(); ++it)
	{
		if ((*it)->GetEntityID() == entityID)
		{
			m_EntityPtrList.erase(it);
			return true;
		}
	}

	return false;
}

//해당 Scene을 리셋할 때 호출하는 메소드.
void CScene::ResetScene()
{
	////Reset키 검사
	//if (KEY_DOWN(VK_BACK)) {

	//	Init();
	//}	

	for (const auto& actor : m_EntityPtrList) {
		actor->Init();
	}

	CCameraManager::GetInstance()->ResetCameraList();
	
}

//LayerOrder가 클수록 먼저 출력됨.
bool CScene::CompareLayer(CLayer * first, CLayer * second)
{
	return (first->GetLayerOrder() > second->GetLayerOrder()); 
}

void CScene::CheckGarbage()
{
	for (auto it = m_EntityPtrList.cbegin(); it != m_EntityPtrList.cend(); )
	{
		if ((*it) == nullptr)
		{
			it = m_EntityPtrList.erase(it);
		}
		else
		{
			++it;
		}
	}

}
