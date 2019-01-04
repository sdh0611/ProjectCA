#include "..\..\stdafx.h"
#include "..\..\Include\Scene\CGameOverScene.h"
#include "..\..\Include\Core\CInputManager.h"
#include "..\..\Include\Scene\CSceneManager.h"
#include "..\..\Include\Scene\Actor\CObjectManager.h"
#include "..\..\Include\Scene\CCameraManager.h"
#include "..\..\Include\Scene\Actor\CCamera.h"
#include "..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\Include\Scene\UI\CButton.h"
#include "..\..\Include\Scene\UI\CFont.h"
#include "..\..\Include\Scene\UI\CMenu.h"
#include "..\..\Include\Scene\Actor\CBackground.h"
#include "..\..\Include\Scene\CLayer.h"
#include "..\..\Include\Core\Sound\CSoundManager.h"


CGameOverScene::CGameOverScene()
	:CScene(Types::ST_GAMEOVER)
{
}

CGameOverScene::~CGameOverScene()
{
}

bool CGameOverScene::Init()
{
	if (!CScene::Init())
	{
		return false;
	}
	CSoundManager::GetInstance()->ChangeBGM(TEXT("BGMGameOver"));

	CInputManager::GetInstance()->SetKeyCheckMode(VK_UP, true);
	CInputManager::GetInstance()->SetKeyCheckMode(VK_DOWN, true);

	auto pObjMgr = CObjectManager::GetInstance();

	//GameOver 글자
	{
		if (!CreateLayer(TEXT("Font"), 1))
			return false;

		auto pFont = pObjMgr->CreateEntity<CFont>(128, SPRITE_HEIGHT / 2, MAX_WIDTH / 2.f, MAX_HEIGHT / 2.f - 100, TEXT("Font"), this);
		if (pFont == nullptr)
			return false;
		pFont->SetFontSize(SPRITE_WIDTH, SPRITE_HEIGHT);
		pFont->SetFontInterval(SPRITE_WIDTH);
		pFont->SetSentence(TEXT("-GAME OVER-"));
		m_EntityPtrList.emplace_back(pFont);
		FindLayer(TEXT("Font"))->AddActor(pFont);
	}

	//Button
	{
		if (!CreateLayer(TEXT("Menu"), 1))
			return false;

		auto pMenu = pObjMgr->CreateEntity<CMenu>(SPRITE_WIDTH * 6, SPRITE_HEIGHT, MAX_WIDTH * 0.5f, MAX_HEIGHT * 0.5f + 100, TEXT("Menu"), this);
		if (pMenu == nullptr)
		{
			return false;
		}

		//Menu에서 쓸 Pointer
		auto pInterface = pObjMgr->CreateEntity<CInterface>(SPRITE_WIDTH * 2, SPRITE_HEIGHT * 2, 0, 0, TEXT("Pointer"), this);
		if (pInterface == nullptr)
		{
			return false;
		}
		pInterface->SetImage(TEXT("Pointer"));
		pMenu->SetFocusPointer(pInterface);
		AddEntityToScene(pMenu);
		FindLayer(TEXT("Menu"))->AddActor(pMenu);

		//Continue button
		auto pButton = pObjMgr->CreateEntity<CButton>(SPRITE_WIDTH * 6, SPRITE_HEIGHT, MAX_WIDTH / 2.f, MAX_HEIGHT / 2.f + 100, TEXT("Button"), this);
		if (pButton == nullptr)
			return false;

		auto startButtonCallback = []()->void {
			puts("Continue");
			CSoundManager::GetInstance()->SoundPlay(TEXT("SFXButtonActivated"));
			CSceneManager::GetInstance()->CreateNextScene(Types::ST_GAME);
			CSceneManager::GetInstance()->SetReadyToChangeScene(true);
		};
		pButton->SetImage(TEXT("ContinueButton"));
		pButton->SetOnClickCallback(startButtonCallback);
		pMenu->AddButton(pButton);

		//m_EntityPtrList.emplace_back(pButton);
		//FindLayer(TEXT("Button"))->AddActor(pButton);

		
		//Go Title button
		pButton = pObjMgr->CreateEntity<CButton>(SPRITE_WIDTH * 6, SPRITE_HEIGHT, MAX_WIDTH / 2.f, MAX_HEIGHT / 2.f + 150, TEXT("Button"), this);
		if (pButton == nullptr)
			return false;

		auto exitButtonCallback = []()->void {
			puts("Back title");
			CSoundManager::GetInstance()->SoundPlay(TEXT("SFXButtonActivated"));
			CSceneManager::GetInstance()->CreateNextScene(Types::ST_TITLE);
			CSceneManager::GetInstance()->SetReadyToChangeScene(true);
		};
		pButton->SetImage(TEXT("GoTitleButton"));
		pButton->SetOnClickCallback(exitButtonCallback);
		pMenu->AddButton(pButton);

		//m_EntityPtrList.emplace_back(pButton);
		//FindLayer(TEXT("Button"))->AddActor(pButton);
	}

	return true;
}

void CGameOverScene::Update(double dDeltaTime)
{
	for (const auto& obj : m_EntityPtrList)
	{
		obj->Update(dDeltaTime);
	}
	CScene::Update(dDeltaTime);
}

void CGameOverScene::Render(const HDC & hDC)
{
	CScene::Render(hDC);
}
