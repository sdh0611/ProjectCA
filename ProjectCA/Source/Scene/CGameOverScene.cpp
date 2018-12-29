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
	//CSoundManager::GetInstance()->StopChannel(CSoundManager::SoundType::SOUND_BGM);
	//CSoundManager::GetInstance()->SoundPlay(TEXT("BGMGameOver"));
	CSoundManager::GetInstance()->ChangeBGM(TEXT("BGMGameOver"));

	auto pObjMgr = CObjectManager::GetInstance();

	//GameOver ±ÛÀÚ
	{
		if (!CreateLayer(TEXT("Font"), 1))
			return false;

		auto pFont = pObjMgr->CreateEntity<CFont>(128, SPRITE_HEIGHT / 2, MAX_WIDTH / 2.f, MAX_HEIGHT / 2.f - 100, TEXT("Font"), this);
		if (pFont == nullptr)
			return false;
		pFont->SetFontSize(SPRITE_WIDTH, SPRITE_HEIGHT);
		pFont->SetFontInterval(SPRITE_WIDTH);
		pFont->SetSentence(TEXT("-Game Over-"));
		m_EntityPtrList.emplace_back(pFont);
		FindLayer(TEXT("Font"))->AddActor(pFont);
	}

	//Button
	{
		if (!CreateLayer(TEXT("Button"), 1))
			return false;

		//Continue button
		auto pButton = pObjMgr->CreateEntity<CButton>(SPRITE_WIDTH * 6, SPRITE_HEIGHT, MAX_WIDTH / 2.f, MAX_HEIGHT / 2.f + 100, TEXT("Button"), this);
		if (pButton == nullptr)
			return false;

		auto startButtonCallback = []()->void {
			puts("Continue");
			CSceneManager::GetInstance()->CreateNextScene(Types::ST_GAME);
			CSceneManager::GetInstance()->SetReadyToChangeScene(true);
		};
		pButton->SetImage(TEXT("ContinueButton"));
		pButton->SetOnClickCallback(startButtonCallback);

		m_EntityPtrList.emplace_back(pButton);
		FindLayer(TEXT("Button"))->AddActor(pButton);

		
		//Go Title button
		pButton = pObjMgr->CreateEntity<CButton>(SPRITE_WIDTH * 6, SPRITE_HEIGHT, MAX_WIDTH / 2.f, MAX_HEIGHT / 2.f + 150, TEXT("Button"), this);
		if (pButton == nullptr)
			return false;

		auto exitButtonCallback = []()->void {
			puts("Back title");
			CSceneManager::GetInstance()->CreateNextScene(Types::ST_TITLE);
			CSceneManager::GetInstance()->SetReadyToChangeScene(true);
		};
		pButton->SetImage(TEXT("GoTitleButton"));
		pButton->SetOnClickCallback(exitButtonCallback);

		m_EntityPtrList.emplace_back(pButton);
		FindLayer(TEXT("Button"))->AddActor(pButton);
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
