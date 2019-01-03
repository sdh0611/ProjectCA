#include "..\..\stdafx.h"
#include "..\..\Include\Core\Sound\CSoundManager.h"
#include "..\..\Include\Core\CCollisionManager.h"
#include "..\..\Include\Core\CInputManager.h"
#include "..\..\Include\Core\Components\Collider.h"
#include "..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\Include\Core\Components\InputComponent.h"
#include "..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\Include\Core\Graphic\CCoinParticle.h"
#include "..\..\Include\Core\Graphic\CBlockParticle.h"
#include "..\..\Include\Scene\CGameScene.h"
#include "..\..\Include\Scene\CLayer.h"
#include "..\..\Include\Scene\CSceneManager.h"
#include "..\..\Include\Scene\Actor\CObjectManager.h"
#include "..\..\Include\Scene\CCameraManager.h"
#include "..\..\Include\Scene\CScoreManager.h"
#include "..\..\Include\Scene\Actor\CEnemy.h"
#include "..\..\Include\Scene\Actor\CKoopa.h"
#include "..\..\Include\Scene\Actor\CGoomba.h"
#include "..\..\Include\Scene\Actor\CRex.h"
#include "..\..\Include\Scene\Actor\CPlayer.h"
#include "..\..\Include\Scene\Actor\CProb.h"
#include "..\..\Include\Scene\Actor\CGround.h"
#include "..\..\Include\Scene\Actor\CSpinBlock.h"
#include "..\..\Include\Scene\Actor\CRandomBlock.h"
#include "..\..\Include\Scene\Actor\CPickupBlock.h"
#include "..\..\Include\Scene\Actor\CCoinBlock.h"
#include "..\..\Include\Scene\Actor\CPipe.h"
#include "..\..\Include\Scene\Actor\CCamera.h"
#include "..\..\Include\Scene\Actor\CBackground.h"
#include "..\..\Include\Scene\Actor\CPickup.h"
#include "..\..\Include\Scene\Actor\CMushroom.h"
#include "..\..\Include\Scene\Actor\CFlower.h"
#include "..\..\Include\Scene\Actor\CEndPickup.h"
#include "..\..\Include\Scene\Actor\CCoin.h"
#include "..\..\Include\Scene\UI\CItemInfo.h"
#include "..\..\Include\Scene\UI\CNumberInterface.h"
#include "..\..\Include\Scene\UI\CFont.h"



CGameScene::CGameScene(Types::SceneType type)
	:CScene(type), m_pObjectManager(nullptr)
{

}

CGameScene::~CGameScene()
{
	m_EntityPtrList.clear();
	puts("Destroy Game");
	CCollisionManager::GetInstance()->Destroy();
}

//Layer우선순위 0번은 추후 UI에 줄 예정임.
bool CGameScene::Init()
{
	if (!CScene::Init())
	{
		return false;
	}

	CSoundManager::GetInstance()->ChangeBGM(TEXT("BGMOverworld"));

	CInputManager::GetInstance()->SetKeyCheckMode(VK_UP, false);
	CInputManager::GetInstance()->SetKeyCheckMode(VK_DOWN, false);

	if (!CCollisionManager::GetInstance()->Init())
		return false;

	m_pScoreManager = CScoreManager::GetInstance();
	m_pScoreManager->Init();

	//Player 생성
	{
		auto pPlayer = m_pObjectManager->CreateActor<CPlayer>(SPRITE_WIDTH, SPRITE_HEIGHT*2.f, 0, 0, Types::OT_PLAYER,
			Types::DIR_RIGHT, TEXT("Player"), this);
		if (pPlayer == nullptr)
			return false;

		auto pCamera = CCameraManager::GetInstance()->CreateCamera(pPlayer, MAX_WIDTH, MAX_HEIGHT);
		if (pCamera.expired())
		{
			return false;
		}
		pCamera.lock()->SetCameraMode(CCamera::CM_SCROLL_HOR);
		//카메라 부착
		pPlayer->AttachCamera(pCamera.lock());
		SetSceneMainCamera(pCamera.lock());

		m_EntityPtrList.emplace_back(pPlayer);
		if (!CreateLayer(TEXT("Player"), 2))
			return false;

		FindLayer(TEXT("Player"))->AddActor(pPlayer);
		m_pPlayer = pPlayer;
	}

	if (!BuildUI())
		return false;

	if (!BuildWorld())
		return false;

	//CSoundManager::GetInstance()->StopChannel(CSoundManager::SoundType::SOUND_BGM);

	//For test
	m_bClear				= false;
	m_iCurScore			= m_pScoreManager->GetScore();
	m_iCoinCount		= m_pScoreManager->GetCoinCount();
	m_iLife				= m_pScoreManager->GetLifeCount();
	m_iRemainTime	= 5;
	m_iTimeScore		= 10000;
	m_dTimeElapsed	= 0.f;

	return true;
}

void CGameScene::Update(double dDeltaTime)
{
	if (!m_bClear)
	{
		CheckGarbage();
		if (m_pPlayer.lock()->IsActive())
		{
			//1. 입력에 따른 동작 수행 후 충돌 검사 및 그에 따른 Actor들과 하위 컴포넌트 동작 Update
			GameUpdate(dDeltaTime);

			//Layer Update -> Rendering을 수행하기 전 expired된 객체가 있는지 검사
			CScene::Update(dDeltaTime);
		}
		else
		{
			if (m_iLife > 0)
			{
				m_pScoreManager->DecreaseLifeCount();
				ResetScene();
			}
			else
			{
				CSceneManager::GetInstance()->SetReadyToChangeScene(true);
				CSceneManager::GetInstance()->CreateNextScene(Types::ST_GAMEOVER);
			}
		}

		if (CInputManager::GetInstance()->IsKeyDown(TEXT("RESET")))
		{
			puts("reset");
			m_iLife = 5;
			m_pScoreManager->Init();
			ResetScene();
		}
	}
	else
	{
		m_dTimeElapsed += dDeltaTime;
		if (m_dTimeElapsed > 2.f && m_dTimeElapsed < 2.1f)
		{
			FindLayer(TEXT("UIClear"))->SetVisible(true);
		}

		if (m_dTimeElapsed > 3.f)
		{
			if (m_iTimeScore > 0)
			{
				if (m_iTimeScore >= 100)
				{
					m_pScoreManager->IncreaseScore(100);
					m_iTimeScore -= 100;
				}
				else
				{
					m_pScoreManager->IncreaseScore(m_iTimeScore);
					m_iTimeScore = 0;
				}
				m_iCurScore = m_pScoreManager->GetScore();
			}

			if(m_dTimeElapsed > 10.f)
			{
				CSceneManager::GetInstance()->SetReadyToChangeScene(true);
				CSceneManager::GetInstance()->CreateNextScene(Types::ST_TITLE);

			}
		}
	}
}

void CGameScene::Render(const HDC& hDC)
{
	//Layer객체가 관리하는 Actor들을 Rendering
	CScene::Render(hDC);

}

void CGameScene::SetIsGameClear()
{
	CSoundManager::GetInstance()->ChangeBGM(TEXT("BGMCastleClear"));
	m_iTimeScore	= m_iRemainTime * 50;
	m_bClear			= true;
	m_dTimeElapsed = 0.f;
	
	//FindLayer(TEXT("Background"))->FadeOut();
	//for (const auto& layer : m_LayerList)
	//{
	//	layer->FadeOut();
	//}
}

std::weak_ptr<CPlayer> CGameScene::GetPlayerPtr()
{
	return m_pPlayer;
}


bool CGameScene::BuildUI()
{
	if (!CreateLayer(TEXT("UI"), 1))
		return false;
	
	//글자 생성
	{
		//Mario
		auto pFont = m_pObjectManager->CreateEntity<CInterface>(SPRITE_WIDTH * 5.f, SPRITE_HEIGHT * 2.5, MAX_WIDTH / 4.f, 45.f, TEXT("FontMario"), this);
		if (pFont == nullptr)
			return false;
		AddEntityToScene(pFont);
		if (!pFont->SetImage(TEXT("UIMario")))
			return false;
		FindLayer(TEXT("UI"))->AddActor(pFont);

		//Time
		pFont = m_pObjectManager->CreateEntity<CInterface>(SPRITE_WIDTH * 2.5f, SPRITE_HEIGHT * 2.5f, MAX_WIDTH / 1.7f, 45.f, TEXT("FontTime"), this);
		if (pFont == nullptr)
			return false;
		AddEntityToScene(pFont);
		if (!pFont->SetImage(TEXT("UITime")))
			return false;
		FindLayer(TEXT("UI"))->AddActor(pFont);
	}

	//8bit크기 그림
	{
		//X
		auto pInterface = m_pObjectManager->CreateEntity<CInterface>(SPRITE_WIDTH / 2.f, SPRITE_HEIGHT / 2.f, MAX_WIDTH / 4.4f, 65.f, TEXT("FontX"), this);
		if (pInterface== nullptr)
			return false;
		AddEntityToScene(pInterface);
		if (!pInterface->SetImage(TEXT("UIX")))
			return false;
		FindLayer(TEXT("UI"))->AddActor(pInterface);

		//Coin
		pInterface = m_pObjectManager->CreateEntity<CInterface>(SPRITE_WIDTH / 2.f, SPRITE_HEIGHT / 2.f, MAX_WIDTH / 1.5f, 45.f, TEXT("ImageCoin"), this);
		if (pInterface == nullptr)
			return false;
		AddEntityToScene(pInterface);
		if (!pInterface->SetImage(TEXT("UICoin")))
			return false;
		FindLayer(TEXT("UI"))->AddActor(pInterface);
		
		//X
		pInterface = m_pObjectManager->CreateEntity<CInterface>(SPRITE_WIDTH / 2.f, SPRITE_HEIGHT / 2.f, MAX_WIDTH / 1.465f, 45.f, TEXT("FontX"), this);
		if (pInterface == nullptr)
			return false;
		AddEntityToScene(pInterface);
		if (!pInterface->SetImage(TEXT("UIX")))
			return false;
		FindLayer(TEXT("UI"))->AddActor(pInterface);


	}

	//NumberInterface 생성
	{
		//Life
		auto pNumberInterface = m_pObjectManager->CreateEntity<CNumberInterface>(SPRITE_WIDTH / 2.f, SPRITE_HEIGHT / 2.f, MAX_WIDTH / 3.7f, 65.f, TEXT("NumberLife"), this);
		if (pNumberInterface == nullptr)
			return false;
		pNumberInterface->SetDigit(2);
		pNumberInterface->SetFontSize(SPRITE_WIDTH / 2, SPRITE_HEIGHT / 2);
		pNumberInterface->SetFontInterval(SPRITE_WIDTH / 2);
		pNumberInterface->LinkValuePtr(&m_iLife);
		AddEntityToScene(pNumberInterface);
		FindLayer(TEXT("UI"))->AddActor(pNumberInterface);

		//Time
		pNumberInterface = m_pObjectManager->CreateEntity<CNumberInterface>(SPRITE_WIDTH / 2.f, SPRITE_HEIGHT / 2.f, MAX_WIDTH / 1.65f, 65.f, TEXT("NumberTime"), this);
		if (pNumberInterface == nullptr)
			return false;
		pNumberInterface->SetDigit(3);
		pNumberInterface->SetFontSize(SPRITE_WIDTH / 2, SPRITE_HEIGHT / 2);
		pNumberInterface->SetFontInterval(SPRITE_WIDTH / 2);
		pNumberInterface->SetFontType(CNumberInterface::FontType::FONT_YELLOW);
		pNumberInterface->LinkValuePtr(&m_iRemainTime);
		AddEntityToScene(pNumberInterface);
		FindLayer(TEXT("UI"))->AddActor(pNumberInterface);

		//Coin
		pNumberInterface = m_pObjectManager->CreateEntity<CNumberInterface>(SPRITE_WIDTH / 2.f, SPRITE_HEIGHT / 2.f, MAX_WIDTH / 1.35f, 45.f, TEXT("NumberCoin"), this);
		if (pNumberInterface == nullptr)
			return false;
		pNumberInterface->SetDigit(2);
		pNumberInterface->SetFontSize(SPRITE_WIDTH / 2, SPRITE_HEIGHT / 2);
		pNumberInterface->SetFontInterval(SPRITE_WIDTH / 2);
		pNumberInterface->LinkValuePtr(&m_iCoinCount);
		AddEntityToScene(pNumberInterface);
		FindLayer(TEXT("UI"))->AddActor(pNumberInterface);

		//Score
		pNumberInterface = m_pObjectManager->CreateEntity<CNumberInterface>(SPRITE_WIDTH / 2.f, SPRITE_HEIGHT / 2.f, MAX_WIDTH / 1.35f, 65.f, TEXT("NumberTotalScore"), this);
		if (pNumberInterface == nullptr)
			return false;
		pNumberInterface->SetDigit(7);
		pNumberInterface->SetFontSize(SPRITE_WIDTH / 2, SPRITE_HEIGHT / 2);
		pNumberInterface->SetFontInterval(SPRITE_WIDTH / 2);
		pNumberInterface->LinkValuePtr(&m_iCurScore);
		AddEntityToScene(pNumberInterface);
		FindLayer(TEXT("UI"))->AddActor(pNumberInterface);
	}

	//ItemInfo 생성
	{
		auto pInfo = m_pObjectManager->CreateEntity<CItemInfo>(SPRITE_WIDTH * 2.5, SPRITE_HEIGHT * 2.5, MAX_WIDTH / 2.f, 60.f, TEXT("ItemInfo"), this);
		if (pInfo == nullptr)
			return false;
		AddEntityToScene(pInfo);
		FindLayer(TEXT("UI"))->AddActor(pInfo);
	}

	//Clear시 나오는 UI 등록
	{
		if (!CreateLayer(TEXT("UIClear"), 1))
		{
			return false;
		}
		FindLayer(TEXT("UIClear"))->SetVisible(false);

		// 'MARIO' 출력
		auto pInterface = m_pObjectManager->CreateEntity<CInterface>(SPRITE_WIDTH * 5.f, SPRITE_HEIGHT * 2.5, MAX_WIDTH * 0.5f, MAX_HEIGHT * 0.3f, TEXT("FontMario"), this);
		if (pInterface == nullptr)
			return false;
		AddEntityToScene(pInterface);
		if (!pInterface->SetImage(TEXT("UIMario")))
			return false;
		FindLayer(TEXT("UIClear"))->AddActor(pInterface);

		// 'Course Clear!' 출력
		auto pFont = m_pObjectManager->CreateEntity<CFont>(SPRITE_WIDTH * 2, SPRITE_HEIGHT * 0.5f, MAX_WIDTH * 0.5f, MAX_HEIGHT * 0.35f, TEXT("Font"), this);
		if (pFont == nullptr)
			return false;
		pFont->SetFontSize(SPRITE_WIDTH * 0.7f, SPRITE_HEIGHT * 0.7f);
		pFont->SetFontInterval(SPRITE_WIDTH * 0.72f);
		pFont->SetSentence(TEXT("COURSE CLEAR!"));
		m_EntityPtrList.emplace_back(pFont);
		FindLayer(TEXT("UIClear"))->AddActor(pFont);

		// 시계그림
		pInterface = m_pObjectManager->CreateEntity<CInterface>(SPRITE_WIDTH * 0.6f, SPRITE_HEIGHT * 0.6f, MAX_WIDTH * 0.38f, MAX_HEIGHT * 0.43f, TEXT("Clock"), this);
		if (pInterface == nullptr)
			return false;
		AddEntityToScene(pInterface);
		if (!pInterface->SetImage(TEXT("UIClock")))
			return false;
		FindLayer(TEXT("UIClear"))->AddActor(pInterface);

		// 남은 시간값과 링크, 출력
		auto pNumberInterface = m_pObjectManager->CreateEntity<CNumberInterface>(SPRITE_WIDTH * 0.5f, SPRITE_HEIGHT * 0.5f, MAX_WIDTH * 0.44f, MAX_HEIGHT * 0.425f, TEXT("NumberTime"), this);
		if (pNumberInterface == nullptr)
			return false;
		pNumberInterface->SetDigit(3);
		pNumberInterface->SetFontSize(SPRITE_WIDTH * 0.6f, SPRITE_HEIGHT * 0.6f);
		pNumberInterface->SetFontInterval(SPRITE_WIDTH *0.65f);
		pNumberInterface->LinkValuePtr(&m_iRemainTime);
		AddEntityToScene(pNumberInterface);
		FindLayer(TEXT("UIClear"))->AddActor(pNumberInterface);

		// 'X' 출력
		pInterface = m_pObjectManager->CreateEntity<CInterface>(SPRITE_WIDTH * 0.6f, SPRITE_HEIGHT * 0.6f, MAX_WIDTH * 0.465f, MAX_HEIGHT * 0.425f, TEXT("FontX"), this);
		if (pInterface == nullptr)
			return false;
		AddEntityToScene(pInterface);
		if (!pInterface->SetImage(TEXT("UIX")))
			return false;
		FindLayer(TEXT("UIClear"))->AddActor(pInterface);

		// '50' 출력
		pFont = m_pObjectManager->CreateEntity<CFont>(128, SPRITE_HEIGHT * 0.5f, MAX_WIDTH * 0.5f, MAX_HEIGHT * 0.425f, TEXT("Font"), this);
		if (pFont == nullptr)
			return false;
		pFont->SetFontSize(SPRITE_WIDTH * 0.6f, SPRITE_HEIGHT * 0.6f);
		pFont->SetFontInterval(SPRITE_WIDTH * 0.65f);
		pFont->SetSentence(TEXT("50"));
		AddEntityToScene(pFont);
		FindLayer(TEXT("UIClear"))->AddActor(pFont);

		// '=' 출력
		pFont = m_pObjectManager->CreateEntity<CFont>(128, SPRITE_HEIGHT * 0.5f, MAX_WIDTH * 0.53f, MAX_HEIGHT * 0.425f, TEXT("Font"), this);
		if (pFont == nullptr)
			return false;
		pFont->SetFontSize(SPRITE_WIDTH * 0.6f, SPRITE_HEIGHT * 0.6f);
		pFont->SetFontInterval(SPRITE_WIDTH * 0.65f);
		pFont->SetSentence(TEXT("="));
		AddEntityToScene(pFont);
		FindLayer(TEXT("UIClear"))->AddActor(pFont);

		pNumberInterface = m_pObjectManager->CreateEntity<CNumberInterface>(SPRITE_WIDTH * 0.5f, SPRITE_HEIGHT * 0.5f, MAX_WIDTH * 0.63f, MAX_HEIGHT * 0.425f, TEXT("NumberTime"), this);
		if (pNumberInterface == nullptr)
			return false;
		pNumberInterface->SetDigit(5);
		pNumberInterface->SetFontSize(SPRITE_WIDTH * 0.6f, SPRITE_HEIGHT * 0.6f);
		pNumberInterface->SetFontInterval(SPRITE_WIDTH *0.65f);
		pNumberInterface->LinkValuePtr(&m_iTimeScore);
		AddEntityToScene(pNumberInterface);
		FindLayer(TEXT("UIClear"))->AddActor(pNumberInterface);

	}

	return true;
}

bool CGameScene::BuildWorld()
{
	//Ground생성
	{
		if (!CreateLayer(TEXT("Ground"), 10))
			return false;

		auto pGround = m_pObjectManager->CreateObject<CGround>(25, 14, 600.f, 250.f, Types::OT_GROUND, TEXT("Ground"), this);
		if (pGround == nullptr)
			return false;
		AddEntityToScene(pGround);
		FindLayer(TEXT("Ground"))->AddActor(pGround);

		pGround = m_pObjectManager->CreateObject<CGround>(20, 30, -768.f, -120.f, Types::OT_PROB, TEXT("Ground"), this);
		if (pGround == nullptr)
			return false;
		AddEntityToScene(pGround);
		FindLayer(TEXT("Ground"))->AddActor(pGround);

		pGround = m_pObjectManager->CreateObject<CGround>(70, 8, -128.f, 600.f, Types::OT_PROB, TEXT("Ground"), this);
		if (pGround == nullptr)
			return false;
		AddEntityToScene(pGround);
		FindLayer(TEXT("Ground"))->AddActor(pGround);

		pGround = m_pObjectManager->CreateObject<CGround>(2, 8, 2290.f, 600.f, Types::OT_PROB, TEXT("Ground"), this);
		if (pGround == nullptr)
			return false;
		AddEntityToScene(pGround);
		FindLayer(TEXT("Ground"))->AddActor(pGround);
			


		pGround = m_pObjectManager->CreateObject<CGround>(2, 8, 2440.f, 600.f, Types::OT_PROB, TEXT("Ground"), this);
		if (pGround == nullptr)
			return false;
		AddEntityToScene(pGround);
		FindLayer(TEXT("Ground"))->AddActor(pGround);


		pGround = m_pObjectManager->CreateObject<CGround>(20, 8, 2700.f, 600.f, Types::OT_PROB, TEXT("Ground"), this);
		if (pGround == nullptr)
			return false;
		AddEntityToScene(pGround);
		FindLayer(TEXT("Ground"))->AddActor(pGround);


		//pGround = m_pObjectManager->CreateObject<CGround>(6, 2, 3148.f, 452.f, Types::OT_PROB, TEXT("Ground"), this);
		//if (pGround == nullptr)
		//	return false;
		//AddEntityToScene(pGround);
		//FindLayer(TEXT("Ground"))->AddActor(pGround);


		//pGround = m_pObjectManager->CreateObject<CGround>(4, 2, 3212.f, 388.f, Types::OT_PROB, TEXT("Ground"), this);
		//if (pGround == nullptr)
		//	return false;
		//AddEntityToScene(pGround);
		//FindLayer(TEXT("Ground"))->AddActor(pGround);


		//pGround = m_pObjectManager->CreateObject<CGround>(2, 2, 3276.f, 324.f, Types::OT_PROB, TEXT("Ground"), this);
		//if (pGround == nullptr)
		//	return false;
		//AddEntityToScene(pGround);
		//FindLayer(TEXT("Ground"))->AddActor(pGround);


		pGround = m_pObjectManager->CreateObject<CGround>(15, 8, 3776.f, 444.f, Types::OT_PROB, TEXT("Ground"), this);
		if (pGround == nullptr)
			return false;
		AddEntityToScene(pGround);
		FindLayer(TEXT("Ground"))->AddActor(pGround);


		pGround = m_pObjectManager->CreateObject<CGround>(8, 12, 6050.f, 300.f, Types::OT_GROUND, TEXT("Ground"), this);
		if (pGround == nullptr)
			return false;
		AddEntityToScene(pGround);
		FindLayer(TEXT("Ground"))->AddActor(pGround);


		pGround = m_pObjectManager->CreateObject<CGround>(15, 8, 5700.f, 444.f, Types::OT_GROUND, TEXT("Ground"), this);
		if (pGround == nullptr)
			return false;
		AddEntityToScene(pGround);
		FindLayer(TEXT("Ground"))->AddActor(pGround);
		

		//pGround = m_pObjectManager->CreateObject<CGround>(10, 8, 7400.f, 500.f, Types::OT_GROUND, TEXT("Ground"), this);
		//if (pGround == nullptr)
		//	return false;
		//AddEntityToScene(pGround);
		//FindLayer(TEXT("Ground"))->AddActor(pGround);

	}


	//Pickup 생성
	{
		if (!CreateLayer(TEXT("Pickup"), 3))
			return false;
		//테스트용 Mushroom 생성
		std::shared_ptr<CPickup> pPickup = m_pObjectManager->CreateActor<CMushroom>(SPRITE_WIDTH, SPRITE_HEIGHT, 300.f, 150.f, Types::OT_PICKUP,
			Types::OS_IDLE, Types::VS_IDLE, Types::HS_RUN, Types::DIR_RIGHT, TEXT("Mushroom"), this);
		if (pPickup == nullptr)
			return false;
		AddEntityToScene(pPickup);
		FindLayer(TEXT("Pickup"))->AddActor(pPickup);

		//테스트용 Flower 생성
		pPickup = m_pObjectManager->CreateActor<CFlower>(SPRITE_WIDTH, SPRITE_HEIGHT, 300.f, 150.f, Types::OT_PICKUP, Types::DIR_RIGHT, TEXT("Flower"), this);
		if (pPickup == nullptr)
			return false;
		AddEntityToScene(pPickup);
		FindLayer(TEXT("Pickup"))->AddActor(pPickup);

		//테스트용 Coin 생성
		pPickup = m_pObjectManager->CreateActor<CCoin>(SPRITE_WIDTH, SPRITE_HEIGHT, 330.f, 150.f, Types::OT_PICKUP, Types::DIR_RIGHT, TEXT("Coin"), this);
		if (pPickup == nullptr)
			return false;
		AddEntityToScene(pPickup);
		FindLayer(TEXT("Pickup"))->AddActor(pPickup);

		//테스트용 Coin 생성
		pPickup = m_pObjectManager->CreateActor<CCoin>(SPRITE_WIDTH, SPRITE_HEIGHT, 370.f, 150.f, Types::OT_PICKUP, Types::DIR_RIGHT, TEXT("Coin"), this);
		if (pPickup == nullptr)
			return false;
		AddEntityToScene(pPickup);
		FindLayer(TEXT("Pickup"))->AddActor(pPickup);

		//테스트용 Coin 생성
		pPickup = m_pObjectManager->CreateActor<CCoin>(SPRITE_WIDTH, SPRITE_HEIGHT, 410.f, 150.f, Types::OT_PICKUP, Types::DIR_RIGHT, TEXT("Coin"), this);
		if (pPickup == nullptr)
			return false;
		AddEntityToScene(pPickup);
		FindLayer(TEXT("Pickup"))->AddActor(pPickup);

		//테스트용 Coin 생성
		pPickup = m_pObjectManager->CreateActor<CCoin>(SPRITE_WIDTH, SPRITE_HEIGHT, 450.f, 150.f, Types::OT_PICKUP, Types::DIR_RIGHT, TEXT("Coin"), this);
		if (pPickup == nullptr)
			return false;
		AddEntityToScene(pPickup);
		FindLayer(TEXT("Pickup"))->AddActor(pPickup);

		//테스트용 Coin 생성
		pPickup = m_pObjectManager->CreateActor<CCoin>(SPRITE_WIDTH, SPRITE_HEIGHT, 0.f, 400.f, Types::OT_PICKUP, Types::DIR_RIGHT, TEXT("Coin"), this);
		if (pPickup == nullptr)
			return false;
		AddEntityToScene(pPickup);
		FindLayer(TEXT("Pickup"))->AddActor(pPickup);

		//
		//테스트용 Coin 생성
		pPickup = m_pObjectManager->CreateActor<CCoin>(SPRITE_WIDTH, SPRITE_HEIGHT, 600.f, 200.f, Types::OT_PICKUP, Types::DIR_RIGHT, TEXT("Coin"), this);
		if (pPickup == nullptr)
			return false;
		AddEntityToScene(pPickup);
		FindLayer(TEXT("Pickup"))->AddActor(pPickup);

		//테스트용 Coin 생성
		pPickup = m_pObjectManager->CreateActor<CCoin>(SPRITE_WIDTH, SPRITE_HEIGHT, 650.f, 200.f, Types::OT_PICKUP, Types::DIR_RIGHT, TEXT("Coin"), this);
		if (pPickup == nullptr)
			return false;
		AddEntityToScene(pPickup);
		FindLayer(TEXT("Pickup"))->AddActor(pPickup);

		//테스트용 Coin 생성
		pPickup = m_pObjectManager->CreateActor<CCoin>(SPRITE_WIDTH, SPRITE_HEIGHT, 700.f, 200.f, Types::OT_PICKUP, Types::DIR_RIGHT, TEXT("Coin"), this);
		if (pPickup == nullptr)
			return false;
		AddEntityToScene(pPickup);
		FindLayer(TEXT("Pickup"))->AddActor(pPickup);

		//테스트용 Coin 생성
		pPickup = m_pObjectManager->CreateActor<CCoin>(SPRITE_WIDTH, SPRITE_HEIGHT, 750.f, 200.f, Types::OT_PICKUP, Types::DIR_RIGHT, TEXT("Coin"), this);
		if (pPickup == nullptr)
			return false;
		AddEntityToScene(pPickup);
		FindLayer(TEXT("Pickup"))->AddActor(pPickup);

		//테스트용 Coin 생성
		pPickup = m_pObjectManager->CreateActor<CCoin>(SPRITE_WIDTH, SPRITE_HEIGHT, 800.f, 200.f, Types::OT_PICKUP, Types::DIR_RIGHT, TEXT("Coin"), this);
		if (pPickup == nullptr)
			return false;
		AddEntityToScene(pPickup);
		FindLayer(TEXT("Pickup"))->AddActor(pPickup);

		//테스트용 Coin 생성
		pPickup = m_pObjectManager->CreateActor<CCoin>(SPRITE_WIDTH, SPRITE_HEIGHT, 850.f, 200.f, Types::OT_PICKUP, Types::DIR_RIGHT, TEXT("Coin"), this);
		if (pPickup == nullptr)
			return false;
		AddEntityToScene(pPickup);
		FindLayer(TEXT("Pickup"))->AddActor(pPickup);
		//

		//테스트용 EndPickup 생성
		pPickup = m_pObjectManager->CreateActor<CEndPickup>(SPRITE_WIDTH, SPRITE_HEIGHT, 8170.f, 500.f, Types::OT_PICKUP, Types::DIR_RIGHT, TEXT("EndPickup"), this);
		if (pPickup == nullptr)
			return false;
		AddEntityToScene(pPickup);
		FindLayer(TEXT("Pickup"))->AddActor(pPickup);
	}

	//Enemy 생성
	{
		if (!CreateLayer(TEXT("Enemy"), 4))
			return false;

		std::shared_ptr<CEnemy> pEnemy = m_pObjectManager->CreateActor<CKoopa>(SPRITE_WIDTH, SPRITE_HEIGHT*1.8f, 250.f, 250.f, Types::OT_ENEMY,
			Types::OS_IDLE, Types::VS_IDLE, Types::HS_RUN, Types::DIR_LEFT, TEXT("KoopaGreen"), this);
		if (pEnemy == nullptr)
			return false;
		AddEntityToScene(pEnemy);
		FindLayer(TEXT("Enemy"))->AddActor(pEnemy);


		pEnemy = m_pObjectManager->CreateActor<CKoopa>(SPRITE_WIDTH, SPRITE_HEIGHT*1.8f, 200.f, 250.f, Types::OT_ENEMY,
			Types::OS_IDLE, Types::VS_IDLE, Types::HS_RUN, Types::DIR_LEFT, TEXT("KoopaGreen"), this);
		if (pEnemy == nullptr)
			return false;
		AddEntityToScene(pEnemy);
		FindLayer(TEXT("Enemy"))->AddActor(pEnemy);


		pEnemy = m_pObjectManager->CreateActor<CKoopa>(SPRITE_WIDTH, SPRITE_HEIGHT*1.8f, 150.f, 250.f, Types::OT_ENEMY,
			Types::OS_IDLE, Types::VS_IDLE, Types::HS_RUN, Types::DIR_LEFT, TEXT("KoopaGreen"), this);
		if (pEnemy == nullptr)
			return false;
		AddEntityToScene(pEnemy);
		FindLayer(TEXT("Enemy"))->AddActor(pEnemy);


		pEnemy = m_pObjectManager->CreateActor<CKoopa>(SPRITE_WIDTH, SPRITE_HEIGHT*1.8f, 300.f, 250.f, Types::OT_ENEMY,
			Types::OS_IDLE, Types::VS_IDLE, Types::HS_RUN, Types::DIR_LEFT, TEXT("KoopaGreen"), this);
		if (pEnemy == nullptr)
			return false;
		AddEntityToScene(pEnemy);
		FindLayer(TEXT("Enemy"))->AddActor(pEnemy);


		pEnemy = m_pObjectManager->CreateActor<CGoomba>(SPRITE_WIDTH, SPRITE_HEIGHT, 300.f, 450.f, Types::OT_ENEMY,
			Types::OS_IDLE, Types::VS_IDLE, Types::HS_RUN, Types::DIR_LEFT, TEXT("Goomba"), this);
		if (pEnemy == nullptr)
			return false;
		AddEntityToScene(pEnemy);
		FindLayer(TEXT("Enemy"))->AddActor(pEnemy);


		pEnemy = m_pObjectManager->CreateActor<CRex>(SPRITE_WIDTH, SPRITE_HEIGHT*1.8f, 500.f, 444.f, Types::OT_ENEMY,
			Types::OS_IDLE, Types::VS_IDLE, Types::HS_RUN, Types::DIR_LEFT, TEXT("Rex"), this);
		if (pEnemy == nullptr)
			return false;
		AddEntityToScene(pEnemy);
		FindLayer(TEXT("Enemy"))->AddActor(pEnemy);


		pEnemy = m_pObjectManager->CreateActor<CKoopa>(SPRITE_WIDTH, SPRITE_HEIGHT*1.8f, 550.f, 444.f, Types::OT_ENEMY,
			Types::OS_IDLE, Types::VS_IDLE, Types::HS_RUN, Types::DIR_LEFT, TEXT("KoopaGreen"), this);
		if (pEnemy == nullptr)
			return false;
		AddEntityToScene(pEnemy);
		FindLayer(TEXT("Enemy"))->AddActor(pEnemy);


		pEnemy = m_pObjectManager->CreateActor<CKoopa>(SPRITE_WIDTH, SPRITE_HEIGHT*1.8f, 570.f, 444.f, Types::OT_ENEMY,
			Types::OS_IDLE, Types::VS_IDLE, Types::HS_RUN, Types::DIR_LEFT, TEXT("KoopaGreen"), this);
		if (pEnemy == nullptr)
			return false;
		AddEntityToScene(pEnemy);
		FindLayer(TEXT("Enemy"))->AddActor(pEnemy);
	

		pEnemy = m_pObjectManager->CreateActor<CGoomba>(SPRITE_WIDTH, SPRITE_HEIGHT, 1000.f, 450.f, Types::OT_ENEMY,
			Types::OS_IDLE, Types::VS_IDLE, Types::HS_RUN, Types::DIR_LEFT, TEXT("Goomba"), this);
		if (pEnemy == nullptr)
			return false;
		AddEntityToScene(pEnemy);
		FindLayer(TEXT("Enemy"))->AddActor(pEnemy);


		pEnemy = m_pObjectManager->CreateActor<CGoomba>(SPRITE_WIDTH, SPRITE_HEIGHT, 1300.f, 450.f, Types::OT_ENEMY,
			Types::OS_IDLE, Types::VS_IDLE, Types::HS_RUN, Types::DIR_LEFT, TEXT("Goomba"), this);
		if (pEnemy == nullptr)
			return false;
		AddEntityToScene(pEnemy);
		FindLayer(TEXT("Enemy"))->AddActor(pEnemy);


		pEnemy = m_pObjectManager->CreateActor<CGoomba>(SPRITE_WIDTH, SPRITE_HEIGHT, 1600.f, 450.f, Types::OT_ENEMY,
			Types::OS_IDLE, Types::VS_IDLE, Types::HS_RUN, Types::DIR_LEFT, TEXT("Goomba"), this);
		if (pEnemy == nullptr)
			return false;
		AddEntityToScene(pEnemy);
		FindLayer(TEXT("Enemy"))->AddActor(pEnemy);


		pEnemy = m_pObjectManager->CreateActor<CKoopa>(SPRITE_WIDTH, SPRITE_HEIGHT*1.8f, 3180.f, 300.f, Types::OT_ENEMY,
			Types::OS_IDLE, Types::VS_IDLE, Types::HS_RUN, Types::DIR_LEFT, TEXT("KoopaGreen"), this);
		if (pEnemy == nullptr)
			return false;
		AddEntityToScene(pEnemy);
		FindLayer(TEXT("Enemy"))->AddActor(pEnemy);

		pEnemy = m_pObjectManager->CreateActor<CRex>(SPRITE_WIDTH, SPRITE_HEIGHT*1.8f, 6250.f, 200.f, Types::OT_ENEMY,
			Types::OS_IDLE, Types::VS_IDLE, Types::HS_RUN, Types::DIR_LEFT, TEXT("Rex"), this);
		if (pEnemy == nullptr)
			return false;
		AddEntityToScene(pEnemy);
		FindLayer(TEXT("Enemy"))->AddActor(pEnemy);

	}

	//Block 생성
	{
		if (!CreateLayer(TEXT("Block"), 8))
			return false;

		//Pickup block
		{
			auto pBlock = m_pObjectManager->CreateObject<CPickupBlock>(SPRITE_WIDTH*1.2, SPRITE_HEIGHT*1.2, 100.f, 300.f, Types::OT_BLOCK, TEXT("Block"), this);
			if (pBlock == nullptr)
				return false;
			//Block에 저장시켜놓을 Pickup 생성
			std::shared_ptr<CPickup> pPickup = m_pObjectManager->CreateActor<CMushroom>(SPRITE_WIDTH, SPRITE_HEIGHT,
				pBlock->GetObjectPosition().x, pBlock->GetObjectPosition().y - pBlock->GetObjectHeight() / 2.f, Types::OT_PICKUP, Types::OS_IDLE, Types::VS_IDLE, Types::HS_RUN,
				Types::DIR_RIGHT, TEXT("Mushroom"), this);
			FindLayer(TEXT("Pickup"))->AddActor(pPickup);
			AddEntityToScene(pPickup);
			if (pPickup == nullptr)
				return false;
			//Pickup set
			pBlock->SetStoredPickup(pPickup);
			AddEntityToScene(pBlock);
			FindLayer(TEXT("Block"))->AddActor(pBlock);


			//pBlock = m_pObjectManager->CreateObject<CPickupBlock>(SPRITE_WIDTH*1.2, SPRITE_HEIGHT*1.2, 460.f, 300.f, Types::OT_BLOCK, TEXT("Block"), this);
			//if (pBlock == nullptr)
			//	return false;
			////Block에 저장시켜놓을 Pickup 생성
			//pPickup = m_pObjectManager->CreateActor<CFlower>(SPRITE_WIDTH, SPRITE_HEIGHT,
			//	pBlock->GetObjectPosition().x, pBlock->GetObjectPosition().y - pBlock->GetObjectHeight() / 2.f, Types::OT_PICKUP, Types::DIR_RIGHT, TEXT("Flower"), this);
			//FindLayer(TEXT("Pickup"))->AddActor(pPickup);
			//AddEntityToScene(pPickup);
			//if (pPickup == nullptr)
			//	return false;
			////Pickup set
			//pBlock->SetStoredPickup(pPickup);
			//AddEntityToScene(pBlock);
			//FindLayer(TEXT("Block"))->AddActor(pBlock);


			pBlock = m_pObjectManager->CreateObject<CPickupBlock>(SPRITE_WIDTH*1.2, SPRITE_HEIGHT*1.2, 150.f, 450.f, Types::OT_BLOCK, TEXT("Block"), this);
			if (pBlock == nullptr)
				return false;
			//Block에 저장시켜놓을 Pickup 생성
			pPickup = m_pObjectManager->CreateActor<CFlower>(SPRITE_WIDTH, SPRITE_HEIGHT,
				pBlock->GetObjectPosition().x, pBlock->GetObjectPosition().y - pBlock->GetObjectHeight() / 2.f, Types::OT_PICKUP, Types::DIR_RIGHT, TEXT("Flower"), this);
			FindLayer(TEXT("Pickup"))->AddActor(pPickup);
			AddEntityToScene(pPickup);
			if (pPickup == nullptr)
				return false;
			//Pickup set
			pBlock->SetStoredPickup(pPickup);
			pBlock->SetHide();
			AddEntityToScene(pBlock);
			FindLayer(TEXT("Block"))->AddActor(pBlock);


			pBlock = m_pObjectManager->CreateObject<CPickupBlock>(SPRITE_WIDTH*1.2, SPRITE_HEIGHT*1.2, 1700.f, 472.f, Types::OT_BLOCK, TEXT("Block"), this);
			if (pBlock == nullptr)
				return false;
			//Block에 저장시켜놓을 Pickup 생성
			pPickup = m_pObjectManager->CreateActor<CFlower>(SPRITE_WIDTH, SPRITE_HEIGHT,
				pBlock->GetObjectPosition().x, pBlock->GetObjectPosition().y - pBlock->GetObjectHeight() / 2.f, Types::OT_PICKUP, Types::DIR_RIGHT, TEXT("Flower"), this);
			FindLayer(TEXT("Pickup"))->AddActor(pPickup);
			AddEntityToScene(pPickup);
			if (pPickup == nullptr)
				return false;
			//Pickup set
			pBlock->SetStoredPickup(pPickup);
			pBlock->SetHide();
			AddEntityToScene(pBlock);
			FindLayer(TEXT("Block"))->AddActor(pBlock);


			pBlock = m_pObjectManager->CreateObject<CPickupBlock>(SPRITE_WIDTH*1.2, SPRITE_HEIGHT*1.2, 2730.f, 600.f, Types::OT_BLOCK, TEXT("Block"), this);
			if (pBlock == nullptr)
				return false;
			//Block에 저장시켜놓을 Pickup 생성
			pPickup = m_pObjectManager->CreateActor<CFlower>(SPRITE_WIDTH, SPRITE_HEIGHT,
				pBlock->GetObjectPosition().x, pBlock->GetObjectPosition().y - pBlock->GetObjectHeight() / 2.f, Types::OT_PICKUP, Types::DIR_RIGHT, TEXT("Flower"), this);
			FindLayer(TEXT("Pickup"))->AddActor(pPickup);
			AddEntityToScene(pPickup);
			if (pPickup == nullptr)
				return false;
			//Pickup set
			pBlock->SetStoredPickup(pPickup);
			AddEntityToScene(pBlock);
			FindLayer(TEXT("Block"))->AddActor(pBlock);
		}

		//Coin block
		{
			auto pBlock = m_pObjectManager->CreateObject<CCoinBlock>(SPRITE_WIDTH*1.2, SPRITE_HEIGHT*1.2, 620.f, 300.f, Types::OT_BLOCK, TEXT("Block"), this);
			if (pBlock == nullptr)
				return false;
			//pBlock->SetLimitTime(3.f);
			AddEntityToScene(pBlock);
			FindLayer(TEXT("Block"))->AddActor(pBlock);

			pBlock = m_pObjectManager->CreateObject<CCoinBlock>(SPRITE_WIDTH*1.2, SPRITE_HEIGHT*1.2, 3320.f, 530.f, Types::OT_BLOCK, TEXT("Block"), this);
			if (pBlock == nullptr)
				return false;
			pBlock->SetHide();
			AddEntityToScene(pBlock);
			FindLayer(TEXT("Block"))->AddActor(pBlock);

			pBlock = m_pObjectManager->CreateObject<CCoinBlock>(SPRITE_WIDTH*1.2, SPRITE_HEIGHT*1.2, 7000.f, 400.f, Types::OT_BLOCK, TEXT("Block"), this);
			if (pBlock == nullptr)
				return false;
			//pBlock->SetLimitTime(3.f);
			AddEntityToScene(pBlock);
			FindLayer(TEXT("Block"))->AddActor(pBlock);
		}

		//Spin block
		{
			auto pBlock = m_pObjectManager->CreateObject<CSpinBlock>(SPRITE_WIDTH*1.2, SPRITE_HEIGHT*1.2, 70.f, 400.f, Types::OT_BLOCK, TEXT("Block"), this);
			if (pBlock == nullptr)
				return false;
			AddEntityToScene(pBlock);
			FindLayer(TEXT("Block"))->AddActor(pBlock);


			pBlock = m_pObjectManager->CreateObject<CSpinBlock>(SPRITE_WIDTH*1.2, SPRITE_HEIGHT*1.2, 500.f, 400.f, Types::OT_BLOCK, TEXT("Block"), this);
			if (pBlock == nullptr)
				return false;
			AddEntityToScene(pBlock);
			FindLayer(TEXT("Block"))->AddActor(pBlock);


			pBlock = m_pObjectManager->CreateObject<CSpinBlock>(SPRITE_WIDTH*1.2, SPRITE_HEIGHT*1.2, 540.f, 400.f, Types::OT_BLOCK, TEXT("Block"), this);
			if (pBlock == nullptr)
				return false;
			AddEntityToScene(pBlock);
			FindLayer(TEXT("Block"))->AddActor(pBlock);


			pBlock = m_pObjectManager->CreateObject<CSpinBlock>(SPRITE_WIDTH*1.2, SPRITE_HEIGHT*1.2, 580.f, 300.f, Types::OT_BLOCK, TEXT("Block"), this);
			if (pBlock == nullptr)
				return false;
			AddEntityToScene(pBlock);
			FindLayer(TEXT("Block"))->AddActor(pBlock);
			

			pBlock = m_pObjectManager->CreateObject<CSpinBlock>(SPRITE_WIDTH*1.2, SPRITE_HEIGHT*1.2, 4320.f, 400.f, Types::OT_BLOCK, TEXT("Block"), this);
			if (pBlock == nullptr)
				return false;
			AddEntityToScene(pBlock);
			FindLayer(TEXT("Block"))->AddActor(pBlock);


			pBlock = m_pObjectManager->CreateObject<CSpinBlock>(SPRITE_WIDTH*1.2, SPRITE_HEIGHT*1.2, 4360.f, 400.f, Types::OT_BLOCK, TEXT("Block"), this);
			if (pBlock == nullptr)
				return false;
			AddEntityToScene(pBlock);
			FindLayer(TEXT("Block"))->AddActor(pBlock);


			pBlock = m_pObjectManager->CreateObject<CSpinBlock>(SPRITE_WIDTH*1.2, SPRITE_HEIGHT*1.2, 4400.f, 400.f, Types::OT_BLOCK, TEXT("Block"), this);
			if (pBlock == nullptr)
				return false;
			AddEntityToScene(pBlock);
			FindLayer(TEXT("Block"))->AddActor(pBlock);


			pBlock = m_pObjectManager->CreateObject<CSpinBlock>(SPRITE_WIDTH*1.2, SPRITE_HEIGHT*1.2, 4620.f, 300.f, Types::OT_BLOCK, TEXT("Block"), this);
			if (pBlock == nullptr)
				return false;
			AddEntityToScene(pBlock);
			FindLayer(TEXT("Block"))->AddActor(pBlock);


			pBlock = m_pObjectManager->CreateObject<CSpinBlock>(SPRITE_WIDTH*1.2, SPRITE_HEIGHT*1.2, 4660.f, 300.f, Types::OT_BLOCK, TEXT("Block"), this);
			if (pBlock == nullptr)
				return false;
			AddEntityToScene(pBlock);
			FindLayer(TEXT("Block"))->AddActor(pBlock);


			pBlock = m_pObjectManager->CreateObject<CSpinBlock>(SPRITE_WIDTH*1.2, SPRITE_HEIGHT*1.2, 4700.f, 300.f, Types::OT_BLOCK, TEXT("Block"), this);
			if (pBlock == nullptr)
				return false;
			AddEntityToScene(pBlock);
			FindLayer(TEXT("Block"))->AddActor(pBlock);


			pBlock = m_pObjectManager->CreateObject<CSpinBlock>(SPRITE_WIDTH*1.2, SPRITE_HEIGHT*1.2, 4740.f, 300.f, Types::OT_BLOCK, TEXT("Block"), this);
			if (pBlock == nullptr)
				return false;
			AddEntityToScene(pBlock);
			FindLayer(TEXT("Block"))->AddActor(pBlock);


			pBlock = m_pObjectManager->CreateObject<CSpinBlock>(SPRITE_WIDTH*1.2, SPRITE_HEIGHT*1.2, 5100.f, 200.f, Types::OT_BLOCK, TEXT("Block"), this);
			if (pBlock == nullptr)
				return false;
			AddEntityToScene(pBlock);
			FindLayer(TEXT("Block"))->AddActor(pBlock);


			pBlock = m_pObjectManager->CreateObject<CSpinBlock>(SPRITE_WIDTH*1.2, SPRITE_HEIGHT*1.2, 5140.f, 200.f, Types::OT_BLOCK, TEXT("Block"), this);
			if (pBlock == nullptr)
				return false;
			AddEntityToScene(pBlock);
			FindLayer(TEXT("Block"))->AddActor(pBlock);
			

			pBlock = m_pObjectManager->CreateObject<CSpinBlock>(SPRITE_WIDTH*1.2, SPRITE_HEIGHT*1.2, 5300.f, 400.f, Types::OT_BLOCK, TEXT("Block"), this);
			if (pBlock == nullptr)
				return false;
			AddEntityToScene(pBlock);
			FindLayer(TEXT("Block"))->AddActor(pBlock);
			
		}

	}

	//Prob 생성
	{
		if (!CreateLayer(TEXT("Prob"), 9))
			return false;

		auto pProb = m_pObjectManager->CreateObject<CPipe>(1, 7, 400.f, 600.f, Types::OT_PROB, TEXT("PIPE"), this);
		if (pProb == nullptr)
			return false;
		AddEntityToScene(pProb);
		FindLayer(TEXT("Prob"))->AddActor(pProb);


		pProb = m_pObjectManager->CreateObject<CPipe>(1, 3, 0.f, 600.f, Types::OT_PROB, TEXT("PIPE"), this);
		if (pProb == nullptr)
			return false;
		AddEntityToScene(pProb);
		FindLayer(TEXT("Prob"))->AddActor(pProb);


		pProb = m_pObjectManager->CreateObject<CPipe>(1, 3, 1000.f, 600.f, Types::OT_PROB, TEXT("PIPE"), this);
		if (pProb == nullptr)
			return false;
		AddEntityToScene(pProb);
		FindLayer(TEXT("Prob"))->AddActor(pProb);


		//pProb = m_pObjectManager->CreateObject<CPipe>(1, 4, 1200.f, 250.f, Types::OT_PROB, TEXT("PIPE"), this);
		//if (pProb == nullptr)
		//	return false;
		//AddEntityToScene(pProb);
		//FindLayer(TEXT("Prob"))->AddActor(pProb);


		pProb = m_pObjectManager->CreateObject<CPipe>(1, 4, 1300.f, 600.f, Types::OT_PROB, TEXT("PIPE"), this);
		if (pProb == nullptr)
			return false;
		AddEntityToScene(pProb);
		FindLayer(TEXT("Prob"))->AddActor(pProb);


		pProb = m_pObjectManager->CreateObject<CPipe>(1, 4, 1600.f, 600.f, Types::OT_PROB, TEXT("PIPE"), this);
		if (pProb == nullptr)
			return false;
		AddEntityToScene(pProb);
		FindLayer(TEXT("Prob"))->AddActor(pProb);


		pProb = m_pObjectManager->CreateObject<CPipe>(1, 2, 3100.f, 600.f, Types::OT_PROB, TEXT("PIPE"), this);
		if (pProb == nullptr)
			return false;
		AddEntityToScene(pProb);
		FindLayer(TEXT("Prob"))->AddActor(pProb);


		pProb = m_pObjectManager->CreateObject<CPipe>(1, 4, 3180.f, 600.f, Types::OT_PROB, TEXT("PIPE"), this);
		if (pProb == nullptr)
			return false;
		AddEntityToScene(pProb);
		FindLayer(TEXT("Prob"))->AddActor(pProb);


		pProb = m_pObjectManager->CreateObject<CPipe>(1, 6, 3260.f, 600.f, Types::OT_PROB, TEXT("PIPE"), this);
		if (pProb == nullptr)
			return false;
		AddEntityToScene(pProb);
		FindLayer(TEXT("Prob"))->AddActor(pProb);


		pProb = m_pObjectManager->CreateObject<CPipe>(1, 6, 7400.f, 700.f, Types::OT_PROB, TEXT("PIPE"), this);
		if (pProb == nullptr)
			return false;
		AddEntityToScene(pProb);
		FindLayer(TEXT("Prob"))->AddActor(pProb);


		pProb = m_pObjectManager->CreateObject<CPipe>(1, 7, 7600.f, 700.f, Types::OT_PROB, TEXT("PIPE"), this);
		if (pProb == nullptr)
			return false;
		AddEntityToScene(pProb);
		FindLayer(TEXT("Prob"))->AddActor(pProb);


		pProb = m_pObjectManager->CreateObject<CPipe>(1, 5, 7800.f, 700.f, Types::OT_PROB, TEXT("PIPE"), this);
		if (pProb == nullptr)
			return false;
		AddEntityToScene(pProb);
		FindLayer(TEXT("Prob"))->AddActor(pProb);


		pProb = m_pObjectManager->CreateObject<CPipe>(1, 5, 8170.f, 700.f, Types::OT_PROB, TEXT("PIPE"), this);
		if (pProb == nullptr)
			return false;
		AddEntityToScene(pProb);
		FindLayer(TEXT("Prob"))->AddActor(pProb);

	}

	//Backgorund 생성
	{
		if (!CreateLayer(TEXT("Background"), 99))
			return false;

		auto pBack = m_pObjectManager->CreateObject<CBackground>(MAX_WIDTH, MAX_HEIGHT, 0.f, 0.f, Types::OT_BACKGROUND, TEXT("Background"), this);
		if (pBack == nullptr)
			return false;
		pBack->SetBackgroundImage(TEXT("BackgroundMountain2"));
		AddEntityToScene(pBack);
		FindLayer(TEXT("Background"))->AddActor(pBack);
	}


	return true;
}

void CGameScene::GameUpdate(double dDeltaTime)
{	
	if (!m_pPlayer.lock()->IsDead())
	{
		//시간, 점수, 코인개수, Life값 Update
		m_dTimeElapsed += dDeltaTime;
		if (m_dTimeElapsed > 1.f)
		{
			if (--m_iRemainTime > 0)
			{
				m_dTimeElapsed = 0.f;
			}
			else //TimeOut
			{
				m_pPlayer.lock()->SetPlayerDead();
			}
		}
		m_iCurScore = m_pScoreManager->GetScore();
		m_iCoinCount	= m_pScoreManager->GetCoinCount();
		m_iLife			= m_pScoreManager->GetLifeCount();

		//Actor Update
		for (const auto& entity : m_EntityPtrList) {
			if (entity->IsActive())
			{
				entity->Update(dDeltaTime);
			}
		}

		//Collsion detect between Actors
		CCollisionManager::GetInstance()->CheckCollision();
		//Update main camera
		CCameraManager::GetInstance()->GetMainCamera().lock()->Update(dDeltaTime);
		//Adjust position on screen 
		for (auto it = m_EntityPtrList.cbegin(); it != m_EntityPtrList.cend(); ++it)
		{
			(*it)->LateUpdate();
		}

	}
	else
	{
		m_pPlayer.lock()->DeadProcess(dDeltaTime);
	}
	
}

void CGameScene::ResetScene()
{
	CSoundManager::GetInstance()->ChangeBGM(TEXT("BGMOverworld"));
	m_bClear				= false;
	m_iRemainTime	= 999;
	m_dTimeElapsed	= 0.f;
	CScene::ResetScene();
}



