#include "..\..\stdafx.h"
#include "..\..\Include\Scene\CTitleScene.h"
#include "..\..\Include\Core\CInputManager.h"
#include "..\..\Include\Scene\CSceneManager.h"
#include "..\..\Include\Scene\Actor\CActorManager.h"
#include "..\..\Include\Scene\UI\CButton.h"
#include "..\..\Include\Scene\CLayer.h"


CTitleScene::CTitleScene()
	:CScene(Types::ST_TITLE)
{
}

CTitleScene::~CTitleScene()
{
}

bool CTitleScene::Init()
{
	CActorManager* objMgr = CActorManager::GetInstance();

	//Test용 버튼 생성
	auto pButton = objMgr->CreateObject<CButton>(256, 96, 300, 300, TEXT("Button"), this);
	if (pButton == nullptr)
		return false;
	
	auto buttonCallback = []()->void {
		CSceneManager::GetInstance()->SetReadyToChangeScene(true);
	};
	pButton->SetOnClickCallback(buttonCallback);

	m_ObjectPtrList.emplace_back(pButton);

	if (!CreateLayer(TEXT("Button"), 0))
		return false;
	FindLayer(TEXT("Button"))->AddActor(pButton);

	return true;
}

void CTitleScene::Update(double dDeltaTime)
{
	for (const auto& obj : m_ObjectPtrList)
	{
		obj->Update(dDeltaTime);
	}

	CScene::Update(dDeltaTime);
}

void CTitleScene::Render(const HDC & hDC)
{
	CScene::Render(hDC);
}
