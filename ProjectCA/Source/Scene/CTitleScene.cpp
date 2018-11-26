#include "..\..\stdafx.h"
#include "..\..\Include\Scene\CTitleScene.h"
#include "..\..\Include\Core\CInputManager.h"
#include "..\..\Include\Scene\CSceneManager.h"
#include "..\..\Include\Scene\Actor\CObjectManager.h"
#include "..\..\Include\Scene\CCameraManager.h"
#include "..\..\Include\Scene\Actor\CCamera.h"
#include "..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\Include\Scene\UI\CButton.h"
#include "..\..\Include\Scene\UI\CFont.h"
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
	CObjectManager* pObjMgr = CObjectManager::GetInstance();

	//Title logo积己
	{
		if (!CreateLayer(TEXT("Title"), 0))
			return false;
	
		auto pTitle = pObjMgr->CreateObject<CBackground>(MAX_WIDTH, MAX_HEIGHT, 0, 0, Types::OT_BACKGROUND, TEXT("Logo"), this);
		if (pTitle == nullptr)
			return false;
		pTitle->SetBackgroundImage(TEXT("Logo"));
		pTitle->SetStatic(true);
		m_ObjectPtrList.emplace_back(pTitle);
		FindLayer(TEXT("Title"))->AddActor(pTitle);
	}

	//Test侩 滚瓢 积己
	{
		if (!CreateLayer(TEXT("Button"), 1))
			return false;

		//Start button
		auto pButton = pObjMgr->CreateObject<CButton>(SPRITE_WIDTH * 12, SPRITE_HEIGHT *1.25, MAX_WIDTH / 2.f, MAX_HEIGHT / 2.f + 120, Types::OT_UI, TEXT("Button"), this);
		if (pButton == nullptr)
			return false;

		auto startButtonCallback = []()->void {
			puts("Start");
			CSceneManager::GetInstance()->SetReadyToChangeScene(true);
		};
		pButton->SetImage(TEXT("StartButton"));
		pButton->SetOnClickCallback(startButtonCallback);

		m_ObjectPtrList.emplace_back(pButton);
		FindLayer(TEXT("Button"))->AddActor(pButton);

		
		//Exit button
		pButton = pObjMgr->CreateObject<CButton>(SPRITE_WIDTH * 10, SPRITE_HEIGHT *1.25, MAX_WIDTH / 2.f, MAX_HEIGHT / 2.f + 180, Types::OT_UI, TEXT("Button"), this);
		if (pButton == nullptr)
			return false;

		auto exitButtonCallback = []()->void {
			puts("Exit");
			PostMessage(HWND_DESKTOP, WM_QUIT, NULL, NULL);
		};
		pButton->SetImage(TEXT("ExitButton"));
		pButton->SetOnClickCallback(exitButtonCallback);

		m_ObjectPtrList.emplace_back(pButton);
		FindLayer(TEXT("Button"))->AddActor(pButton);
	}

	//Font 积己
	{
		if (!CreateLayer(TEXT("Font"), 2))
			return false;

		auto pFont = pObjMgr->CreateObject<CFont>(128, SPRITE_HEIGHT / 2, MAX_WIDTH / 2.f , MAX_HEIGHT / 2.f + 250, Types::OT_UI, TEXT("Font"), this);
		if (pFont == nullptr)
			return false;
		pFont->SetFontSize(SPRITE_WIDTH / 2, SPRITE_HEIGHT / 2);
		pFont->SetFontInterval(SPRITE_WIDTH / 2);
		pFont->SetSentence(TEXT("-Made by SDH-"));
		m_ObjectPtrList.emplace_back(pFont);
		FindLayer(TEXT("Font"))->AddActor(pFont);

		pFont = pObjMgr->CreateObject<CFont>(128, SPRITE_HEIGHT / 2, MAX_WIDTH / 2.f, MAX_HEIGHT / 2.f + 280, Types::OT_UI, TEXT("Font"), this);
		if (pFont == nullptr)
			return false;
		pFont->SetFontSize(SPRITE_WIDTH / 4, SPRITE_HEIGHT / 4);
		pFont->SetFontInterval(SPRITE_WIDTH / 4);
		pFont->SetSentence(TEXT("Since 2018"));
		m_ObjectPtrList.emplace_back(pFont);
		FindLayer(TEXT("Font"))->AddActor(pFont);
	}

	//硅版 积己
	{
		if (!CreateLayer(TEXT("Background"), 10))
			return false;

		auto pBackground = pObjMgr->CreateObject<CBackground>(MAX_WIDTH, MAX_HEIGHT, 0, 0, Types::OT_BACKGROUND, TEXT("Background"), this);
		if (pBackground == nullptr)
			return false;
		pBackground->SetBackgroundImage(TEXT("BackgroundMountain2"));
		pBackground->SetStatic(true);
		m_ObjectPtrList.emplace_back(pBackground);
		FindLayer(TEXT("Background"))->AddActor(pBackground);

	}

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
