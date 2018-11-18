#include "..\..\stdafx.h"
#include "..\..\Include\Scene\CTitleScene.h"
#include "..\..\Include\Core\CInputManager.h"
#include "..\..\Include\Scene\CSceneManager.h"
#include "..\..\Include\Scene\Actor\CObjectManager.h"
#include "..\..\Include\Scene\CCameraManager.h"
#include "..\..\Include\Scene\Actor\CCamera.h"
#include "..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\Include\Scene\UI\CButton.h"
#include "..\..\Include\Scene\Actor\CBackground.h"
#include "..\..\Include\Scene\CLayer.h"


CTitleScene::CTitleScene()
	:CScene(Types::ST_TITLE)
{
}

CTitleScene::~CTitleScene()
{
	puts("Destroy Title");
}

bool CTitleScene::Init()
{
	CObjectManager* objMgr = CObjectManager::GetInstance();

	//Title생성
	auto pTitle = objMgr->CreateObject<CBackground>(MAX_WIDTH, MAX_HEIGHT, 0, 0, TEXT("Logo"), this);
	if (pTitle == nullptr)
		return false;
	pTitle->SetBackgroundImage(TEXT("Logo"));
	pTitle->SetStatic(true);
	m_ObjectPtrList.emplace_back(pTitle);
	//auto pCamera = CCameraManager::GetInstance()->CreateCamera(pTitle, MAX_WIDTH, MAX_HEIGHT);
	//if (pCamera.expired())
	//	return false;
	//pCamera.lock()->SetCameraMode(CCamera::CM_STATIC);
	//pTitle->AttachCamera(pCamera.lock());
	////해당 Scene의 메인카메라 설정
	//SetSceneMainCamera(pCamera.lock());
	
	if (!CreateLayer(TEXT("Title"), 0))
		return false;
	FindLayer(TEXT("Title"))->AddActor(pTitle);


	//Test용 버튼 생성
	auto pButton = objMgr->CreateObject<CButton>(256, 96, MAX_WIDTH/2.f - 128, MAX_HEIGHT/2.f + 50, TEXT("Button"), this);
	if (pButton == nullptr)
		return false;
	
	auto buttonCallback = []()->void {
		CSceneManager::GetInstance()->SetReadyToChangeScene(true);
	};
	pButton->SetOnClickCallback(buttonCallback);

	m_ObjectPtrList.emplace_back(pButton);

	if (!CreateLayer(TEXT("Button"), 1))
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
