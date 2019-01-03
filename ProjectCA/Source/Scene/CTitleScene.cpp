#include "..\..\stdafx.h"
#include "..\..\Include\Scene\CTitleScene.h"
#include "..\..\Include\Core\CInputManager.h"
#include "..\..\Include\Scene\CSceneManager.h"
#include "..\..\Include\Scene\Actor\CObjectManager.h"
#include "..\..\Include\Scene\CCameraManager.h"
#include "..\..\Include\Scene\Actor\CCamera.h"
#include "..\..\Include\Scene\Actor\CGround.h"
#include "..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\Include\Scene\UI\CButton.h"
#include "..\..\Include\Scene\UI\CFont.h"
#include "..\..\Include\Scene\UI\CMenu.h"
#include "..\..\Include\Scene\Actor\CBackground.h"
#include "..\..\Include\Scene\CLayer.h"
#include "..\..\Include\Core\Sound\CSoundManager.h"


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
	if (!CScene::Init())
	{
		return false;
	}

	CSoundManager::GetInstance()->ChangeBGM(TEXT("BGMTitle"));
	
	CInputManager::GetInstance()->SetKeyCheckMode(VK_UP, true);
	CInputManager::GetInstance()->SetKeyCheckMode(VK_DOWN, true);

	auto pObjMgr = CObjectManager::GetInstance();

	auto pCamera = CCameraManager::GetInstance()->CreateCamera(nullptr, MAX_WIDTH, MAX_HEIGHT);
	if (pCamera.expired())
		return false;
	SetSceneMainCamera(pCamera.lock());

	//Title logo持失
	{
		if (!CreateLayer(TEXT("Title"), 1))
			return false;
	
		auto pTitle = pObjMgr->CreateEntity<CInterface>( MAX_WIDTH * 0.7f, MAX_HEIGHT * 0.3f , MAX_WIDTH / 2.f, MAX_HEIGHT / 3.f, TEXT("Logo"), this);
		if (pTitle == nullptr)
			return false;
		pTitle->SetImage(TEXT("Logo2"));
		m_EntityPtrList.emplace_back(pTitle);
		FindLayer(TEXT("Title"))->AddActor(pTitle);
	}

	//Menu 持失
	{
		if (!CreateLayer(TEXT("Menu"), 2))
			return false;

		auto pMenu = pObjMgr->CreateEntity<CMenu>(SPRITE_WIDTH * 9, SPRITE_HEIGHT, MAX_WIDTH * 0.5f, MAX_HEIGHT * 0.5f + 80, TEXT("Button"), this);
		if (pMenu == nullptr)
		{
			return false;
		}

		//Pointer
		auto pInterface = pObjMgr->CreateEntity<CInterface>(SPRITE_WIDTH*2, SPRITE_HEIGHT*2, 0, 0, TEXT("Pointer"), this);
		if (pInterface == nullptr)
		{
			return false;
		}
		pInterface->SetImage(TEXT("Pointer"));
		pMenu->SetFocusPointer(pInterface);
		AddEntityToScene(pMenu);
		FindLayer(TEXT("Menu"))->AddActor(pMenu);

		//Start button
		auto pButton = pObjMgr->CreateEntity<CButton>(SPRITE_WIDTH * 9, SPRITE_HEIGHT, MAX_WIDTH / 2.f, MAX_HEIGHT / 2.f + 120, TEXT("Button"), this);
		if (pButton == nullptr)
			return false;

		auto startButtonCallback = []()->void {
			puts("Start");
			CSoundManager::GetInstance()->SoundPlay(TEXT("SFXButtonActivated"));
			CSceneManager::GetInstance()->CreateNextScene(Types::ST_GAME);
			CSceneManager::GetInstance()->SetReadyToChangeScene(true);
		};
		pButton->SetImage(TEXT("StartButton"));
		pButton->SetOnClickCallback(startButtonCallback);
		pMenu->AddButton(pButton);
		//m_EntityPtrList.emplace_back(pButton);
		//FindLayer(TEXT("Button"))->AddActor(pButton);

		
		//Exit button
		pButton = pObjMgr->CreateEntity<CButton>(SPRITE_WIDTH * 8, SPRITE_HEIGHT, MAX_WIDTH / 2.f, MAX_HEIGHT / 2.f + 180, TEXT("Button"), this);
		if (pButton == nullptr)
			return false;

		auto exitButtonCallback = []()->void {
			puts("Exit");
			CSoundManager::GetInstance()->SoundPlay(TEXT("SFXButtonActivated"));
			PostMessage(HWND_DESKTOP, WM_QUIT, NULL, NULL);
		};
		pButton->SetImage(TEXT("ExitButton"));
		pButton->SetOnClickCallback(exitButtonCallback);
		pMenu->AddButton(pButton);

		//m_EntityPtrList.emplace_back(pButton);
		//FindLayer(TEXT("Button"))->AddActor(pButton);
	}

	//Test遂 Menu Box持失
	{



	}

	//Font 持失
	{
		if (!CreateLayer(TEXT("Font"), 3))
			return false;

		auto pFont = pObjMgr->CreateEntity<CFont>(128, SPRITE_HEIGHT / 2, MAX_WIDTH / 2.f , MAX_HEIGHT / 2.f + 250, TEXT("Font"), this);
		if (pFont == nullptr)
			return false;
		pFont->SetFontSize(SPRITE_WIDTH / 2, SPRITE_HEIGHT / 2);
		pFont->SetFontInterval(SPRITE_WIDTH / 2);
		pFont->SetSentence(TEXT("-Made by SDH-"));
		m_EntityPtrList.emplace_back(pFont);
		FindLayer(TEXT("Font"))->AddActor(pFont);

		pFont = pObjMgr->CreateEntity<CFont>(128, SPRITE_HEIGHT / 2, MAX_WIDTH / 2.f, MAX_HEIGHT / 2.f + 280, TEXT("Font"), this);
		if (pFont == nullptr)
			return false;
		pFont->SetFontSize(SPRITE_WIDTH / 4, SPRITE_HEIGHT / 4);
		pFont->SetFontInterval(SPRITE_WIDTH / 4);
		pFont->SetSentence(TEXT("Since 2018"));
		m_EntityPtrList.emplace_back(pFont);
		FindLayer(TEXT("Font"))->AddActor(pFont);
	}

	//{
	//	if (!CreateLayer(TEXT("Ground"), 3))
	//		return false;

	//	auto pGround = pObjMgr->CreateObject<CGround>(MAX_WIDTH, 256, MAX_WIDTH / 2.f, 700.f, Types::OT_GROUND, TEXT("Ground"), this);
	//	if (pGround == nullptr)
	//		return false;
	//	m_EntityPtrList.push_back(pGround);
	//	FindLayer(TEXT("Ground"))->AddActor(pGround);
	//}

	//壕井 持失
	{
		if (!CreateLayer(TEXT("Background"), 99))
			return false;

		auto pBackground = pObjMgr->CreateObject<CBackground>(MAX_WIDTH, MAX_HEIGHT, 0, 0, Types::OT_BACKGROUND, TEXT("Background"), this);
		if (pBackground == nullptr)
			return false;
		pBackground->SetBackgroundImage(TEXT("BackgroundMountain2"));
		pBackground->SetStatic(true);
		m_EntityPtrList.emplace_back(pBackground);
		FindLayer(TEXT("Background"))->AddActor(pBackground);
	}

	return true;
}

void CTitleScene::Update(double dDeltaTime)
{
	for (const auto& obj : m_EntityPtrList)
	{
		obj->Update(dDeltaTime);
	}

	CScene::Update(dDeltaTime);

}

void CTitleScene::Render(const HDC & hDC)
{
	CScene::Render(hDC);
}
