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
	m_ObjectPtrList.clear();
	CObjectManager::GetInstance()->Clear();
}


void CScene::StartScene()
{
	CCameraManager::GetInstance()->ChangeMainCamera(m_pMainCameraPtr.lock());
}

bool CScene::Init()
{
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
		for (m_it = m_LayerList.begin(); m_it != m_LayerList.end(); ++m_it) {
			(*m_it)->Render(hDC);
			
		}
}

//CLayer 클래스의 friend 메소드.
//CreateLayer메소드를 통해서만 Layer 생성이 가능하게끔 만든다.
bool CScene::CreateLayer(const Types::tstring & tag, int order)
{
	if (FindLayer(tag))
		return false;

	CLayer* pLayer = new CLayer;
	pLayer->Init(tag, order);

	m_LayerList.emplace_back(pLayer);
	SortLayer();

	return true;
}

bool CScene::DeleteLayer(const Types::tstring & tag)
{
	if (!FindLayer(tag))
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

void CScene::AddObjectToScene(CObject * pObject)
{
	m_ObjectPtrList.emplace_back(pObject);
}

void CScene::AddObjectToScene(std::shared_ptr<CObject> pObject)
{
	m_ObjectPtrList.push_back(pObject);
}

WeakObjPtr CScene::FindObjectFromScene(CObject * pObject)
{
	for (const auto& obj : m_ObjectPtrList)
	{
		if (obj.get() == pObject)
		{
			return obj;
		}
	}

	return WeakObjPtr();
}

WeakObjPtr CScene::FindObjectFromScene(const TSTRING & strObjectName)
{
	for (const auto& obj : m_ObjectPtrList)
	{
		if (obj->GetObjectName() == strObjectName)
		{
			return obj;
		}
	}

	return WeakObjPtr();
}

WeakObjPtr CScene::FindInactiveObjectFromScene(CObject * pObject)
{
	for (const auto& obj : m_ObjectPtrList)
	{
		if (obj.get() == pObject)
		{
			return obj;
		}
	}

	return WeakObjPtr();
}

WeakObjPtr CScene::FindInactiveObjectFromScene(const TSTRING & strObjectName)
{
	for (const auto& obj : m_ObjectPtrList)
	{
		if (obj->GetObjectName() == strObjectName)
		{
			return obj;
		}
	}
	

	return WeakObjPtr();
}

void CScene::SetSceneMainCamera(std::shared_ptr<CCamera> pCamera)
{
	m_pMainCameraPtr = pCamera;
}

WeakObjPtr CScene::FindObjectFromScene(UINT iObjectID)
{
	for (const auto& obj : m_ObjectPtrList)
	{
		if (obj->GetObjectID() == iObjectID)
		{
			return obj;
		}

	}

	return WeakObjPtr();
}

bool CScene::DeleteObjectFromScene(CObject * pObject)
{
	for (auto it = m_ObjectPtrList.cbegin(); it != m_ObjectPtrList.cend(); )
	{
		if (it->get() == pObject)
		{
			m_ObjectPtrList.erase(it);
			return true;
		}

	}

	return false;
}

bool CScene::DeleteObjectFromScene(UINT iObjectID)
{
	return false;
}

//해당 Scene을 리셋할 때 호출하는 메소드.
void CScene::ResetScene()
{
	////Reset키 검사
	//if (KEY_DOWN(VK_BACK)) {

	//	Init();
	//}	

	for (const auto& actor : m_ObjectPtrList) {
		actor->Init();
	}

	CCameraManager::GetInstance()->ResetCameraList();
	
}

//LayerOrder가 클수록 먼저 출력됨.
bool CScene::CompareLayer(CLayer * first, CLayer * second)
{
	return (first->GetLayerOrder() > second->GetLayerOrder()); 
}
