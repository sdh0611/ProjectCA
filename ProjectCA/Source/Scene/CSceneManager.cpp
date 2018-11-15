#include "..\..\stdafx.h"
#include "..\..\Include\Scene\CSceneManager.h"
#include "..\..\Include\Scene\CCameraManager.h"
#include "..\..\Include\Scene\Actor\CActorManager.h"
#include "..\..\Include\Scene\CScene.h"
#include "..\..\Include\Scene\CGameScene.h"
#include "..\..\Include\Scene\CTitleScene.h"



CSceneManager::CSceneManager() :m_pScene(nullptr), m_pNextScene(nullptr) 
{ 
}

CSceneManager::~CSceneManager() {
	
	SAFE_DELETE(m_pScene)
	SAFE_DELETE(m_pNextScene);
	CActorManager::GetInstance()->Destroy();
	CCameraManager::GetInstance()->Destroy();

}

bool CSceneManager::Init(Types::SceneType type) 
{

	if (!CreateScene(type))
		return false;

	if (!CActorManager::GetInstance()->Init())
		return false;

	if (!CCameraManager::GetInstance()->Init())
		return false;

	m_bReadyToChageScene = false;

	return true;
}

void CSceneManager::Update(double fDeltaTime)
{
	if (m_bReadyToChageScene)
	{
		ChangeScene(Types::ST_TITLE);
	}
	else
	{
		m_pScene->Update(fDeltaTime);
	}

}

void CSceneManager::Render(const HDC& hDC)
{
	m_pScene->Render(hDC);
}

bool CSceneManager::CreateScene(Types::SceneType type)
{
	SAFE_DELETE(m_pScene);

	switch (type) {
	case Types::ST_TITLE:
		m_pScene = new CTitleScene();
		break;
	case Types::ST_GAME:
		m_pScene = new CGameScene(type);
		break;
	case Types::ST_SELECT:

		break;
	default:
		return false;
	}

	return m_pScene->Init();
}

bool CSceneManager::CreateNextScene(Types::SceneType type)
{
	SAFE_DELETE(m_pNextScene);

	switch (type) {
	case Types::ST_TITLE:
		m_pNextScene = new CTitleScene();
		break;
	case Types::ST_GAME:
		m_pNextScene = new CGameScene(type);
		break;
	case Types::ST_SELECT:

		break;
	default:
		return false;
	}
	
	return m_pNextScene->Init();
}

bool CSceneManager::ChangeScene(Types::SceneType nextSceneType)
{
	if (m_pNextScene == nullptr)
		return false;

	SAFE_DELETE(m_pScene)
	m_pScene = m_pNextScene;
	m_pNextScene = nullptr;

	m_bReadyToChageScene = false;

	return CreateNextScene(nextSceneType);
}

void CSceneManager::SetReadyToChangeScene(bool bReady)
{
	m_bReadyToChageScene = bReady;
}

CScene * CSceneManager::GetCurScene()
{
	return m_pScene;
}
