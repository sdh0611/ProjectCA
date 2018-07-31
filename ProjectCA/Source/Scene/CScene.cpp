#include "..\..\Include\Scene\CScene.h"
#include "..\..\Include\Scene\CLayer.h"


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

}

bool CScene::Init()
{
	if (!m_LayerList.empty())
		for (m_it = m_LayerList.begin(); m_it != m_LayerList.end(); ++m_it) {
			(*m_it)->Init();

		}

	return true;
}

void CScene::Update(float deltaTime)
{
	if (!m_LayerList.empty())
		for (m_it = m_LayerList.begin(); m_it != m_LayerList.end(); ++m_it) {
			(*m_it)->Update(deltaTime);

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

	m_LayerList.emplace_back(new CLayer(order, tag));
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

CLayer * CScene::FindLayer(const Types::tstring & tag)
{
	for (m_it = m_LayerList.begin(); m_it != m_LayerList.end(); ++m_it) {
		if ((*m_it)->GetLayerTag() == tag)
			return (*m_it);
	}

	return nullptr;
}

//LayerOrder가 클수록 먼저 출력됨.
bool CScene::CompareLayer(CLayer * first, CLayer * second)
{
	return (first->GetLayerOrder() > second->GetLayerOrder()); 
}
