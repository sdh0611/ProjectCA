#include "..\..\stdafx.h"
#include "..\..\Include\Scene\CGameScene.h"
#include "..\..\Include\Scene\CLayer.h"
#include "..\..\Include\Core\CCollisionManager.h"
#include "..\..\Include\Scene\CSceneManager.h"
#include "..\..\Include\Scene\Actor\CObjectManager.h"
#include "..\..\Include\Scene\CCameraManager.h"
#include "..\..\Include\Scene\CScoreManager.h"
#include "..\..\Include\Core\CInputManager.h"
#include "..\..\Include\Core\Components\Collider.h"
#include "..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\Include\Core\Components\InputComponent.h"
#include "..\..\Include\Core\Components\TransformComponent.h"
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



CGameScene::CGameScene(Types::SceneType type)
	:CScene(type), m_pObjectManager(nullptr)
{
	//기본 레이어 생성.
	//ActorManager에서 Actor를 생성할 경우 기본적으로 default레이어에 들어가게 할 것.
	//Layer order 0은 UI에 줄 것.
	CreateLayer(TEXT("default"), 1);

}

CGameScene::~CGameScene()
{
	m_ObjectPtrList.clear();
	puts("Destroy Game");
	CCollisionManager::GetInstance()->Destroy();
}

//Layer우선순위 0번은 추후 UI에 줄 예정임.
bool CGameScene::Init()
{
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

		m_ObjectPtrList.emplace_back(pPlayer);
		if (!CreateLayer(TEXT("Player"), 2))
			return false;

		FindLayer(TEXT("Player"))->AddActor(pPlayer);
		m_pPlayer = pPlayer;
	}

	if (!BuildUI())
		return false;

	if (!BuildWorld())
		return false;

	//For test
	m_bClear				= false;
	m_iCurScore			= m_pScoreManager->GetScore();
	m_iCoinCount		= m_pScoreManager->GetCoinCount();
	m_iLife				= m_pScoreManager->GetLifeCount();
	m_iRemainTime	= 5;
	m_dTimeElapsed	= 0.f;

	CScene::Init();

	return true;
}

void CGameScene::Update(double dDeltaTime)
{
	if (!m_bClear)
	{
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
		CSceneManager::GetInstance()->SetReadyToChangeScene(true);
		CSceneManager::GetInstance()->CreateNextScene(Types::ST_GAMEOVER);
	}
}

void CGameScene::Render(const HDC& hDC)
{
	//Layer객체가 관리하는 Actor들을 Rendering
	CScene::Render(hDC);

}

void CGameScene::SetIsGameClear()
{
	m_bClear = true;
}

std::weak_ptr<CPlayer> CGameScene::GetPlayerPtr()
{
	return m_pPlayer;
}


bool CGameScene::BuildUI()
{
	if (!CreateLayer(TEXT("UI"), 0))
		return false;
	
	//글자 생성
	{
		//Mario
		auto pFont = m_pObjectManager->CreateObject<CInterface>(SPRITE_WIDTH * 5.f, SPRITE_HEIGHT * 2.5, MAX_WIDTH / 4.f, 45.f, Types::OT_UI, TEXT("FontMario"), this);
		if (pFont == nullptr)
			return false;
		m_ObjectPtrList.emplace_back(pFont);
		if (!pFont->SetImage(TEXT("UIMario")))
			return false;
		FindLayer(TEXT("UI"))->AddActor(pFont);

		//Time
		pFont = m_pObjectManager->CreateObject<CInterface>(SPRITE_WIDTH * 2.5f, SPRITE_HEIGHT * 2.5f, MAX_WIDTH / 1.7f, 45.f, Types::OT_UI, TEXT("FontTime"), this);
		if (pFont == nullptr)
			return false;
		m_ObjectPtrList.emplace_back(pFont);
		if (!pFont->SetImage(TEXT("UITime")))
			return false;
		FindLayer(TEXT("UI"))->AddActor(pFont);
	}

	//8bit크기 그림
	{
		//X
		auto pInterface = m_pObjectManager->CreateObject<CInterface>(SPRITE_WIDTH / 2.f, SPRITE_HEIGHT / 2.f, MAX_WIDTH / 4.4f, 65.f, Types::OT_UI, TEXT("FontX"), this);
		if (pInterface== nullptr)
			return false;
		m_ObjectPtrList.emplace_back(pInterface);
		if (!pInterface->SetImage(TEXT("UIX")))
			return false;
		FindLayer(TEXT("UI"))->AddActor(pInterface);

		//Coint
		pInterface = m_pObjectManager->CreateObject<CInterface>(SPRITE_WIDTH / 2.f, SPRITE_HEIGHT / 2.f, MAX_WIDTH / 1.5f, 45.f, Types::OT_UI, TEXT("ImageCoin"), this);
		if (pInterface == nullptr)
			return false;
		m_ObjectPtrList.emplace_back(pInterface);
		if (!pInterface->SetImage(TEXT("UICoin")))
			return false;
		FindLayer(TEXT("UI"))->AddActor(pInterface);
		
		//X
		pInterface = m_pObjectManager->CreateObject<CInterface>(SPRITE_WIDTH / 2.f, SPRITE_HEIGHT / 2.f, MAX_WIDTH / 1.465f, 45.f, Types::OT_UI, TEXT("FontX"), this);
		if (pInterface == nullptr)
			return false;
		m_ObjectPtrList.emplace_back(pInterface);
		if (!pInterface->SetImage(TEXT("UIX")))
			return false;
		FindLayer(TEXT("UI"))->AddActor(pInterface);
	}

	//NumberInterface 생성
	{
		//Life
		auto pNumberInterface = m_pObjectManager->CreateObject<CNumberInterface>(SPRITE_WIDTH / 2.f, SPRITE_HEIGHT / 2.f, MAX_WIDTH / 3.7f, 65.f, Types::OT_UI, TEXT("NumberLife"), this);
		if (pNumberInterface == nullptr)
			return false;
		pNumberInterface->SetDigit(2);
		pNumberInterface->SetFontSize(SPRITE_WIDTH / 2, SPRITE_HEIGHT / 2);
		pNumberInterface->SetFontInterval(SPRITE_WIDTH / 2);
		pNumberInterface->LinkValuePtr(&m_iLife);
		m_ObjectPtrList.emplace_back(pNumberInterface);
		FindLayer(TEXT("UI"))->AddActor(pNumberInterface);

		//Time
		pNumberInterface = m_pObjectManager->CreateObject<CNumberInterface>(SPRITE_WIDTH / 2.f, SPRITE_HEIGHT / 2.f, MAX_WIDTH / 1.65f, 65.f, Types::OT_UI, TEXT("NumberTime"), this);
		if (pNumberInterface == nullptr)
			return false;
		pNumberInterface->SetDigit(3);
		pNumberInterface->SetFontSize(SPRITE_WIDTH / 2, SPRITE_HEIGHT / 2);
		pNumberInterface->SetFontInterval(SPRITE_WIDTH / 2);
		pNumberInterface->SetFontType(CNumberInterface::FontType::FONT_YELLOW);
		pNumberInterface->LinkValuePtr(&m_iRemainTime);
		m_ObjectPtrList.emplace_back(pNumberInterface);
		FindLayer(TEXT("UI"))->AddActor(pNumberInterface);

		//Coin
		pNumberInterface = m_pObjectManager->CreateObject<CNumberInterface>(SPRITE_WIDTH / 2.f, SPRITE_HEIGHT / 2.f, MAX_WIDTH / 1.35f, 45.f, Types::OT_UI, TEXT("NumberCoin"), this);
		if (pNumberInterface == nullptr)
			return false;
		pNumberInterface->SetDigit(2);
		pNumberInterface->SetFontSize(SPRITE_WIDTH / 2, SPRITE_HEIGHT / 2);
		pNumberInterface->SetFontInterval(SPRITE_WIDTH / 2);
		pNumberInterface->LinkValuePtr(&m_iCoinCount);
		m_ObjectPtrList.emplace_back(pNumberInterface);
		FindLayer(TEXT("UI"))->AddActor(pNumberInterface);

		//Score
		pNumberInterface = m_pObjectManager->CreateObject<CNumberInterface>(SPRITE_WIDTH / 2.f, SPRITE_HEIGHT / 2.f, MAX_WIDTH / 1.35f, 65.f, Types::OT_UI, TEXT("NumberScore"), this);
		if (pNumberInterface == nullptr)
			return false;
		pNumberInterface->SetDigit(7);
		pNumberInterface->SetFontSize(SPRITE_WIDTH / 2, SPRITE_HEIGHT / 2);
		pNumberInterface->SetFontInterval(SPRITE_WIDTH / 2);
		pNumberInterface->LinkValuePtr(&m_iCurScore);
		m_ObjectPtrList.emplace_back(pNumberInterface);
		FindLayer(TEXT("UI"))->AddActor(pNumberInterface);
	}

	//ItemInfo 생성
	{
		auto pInfo = m_pObjectManager->CreateObject<CItemInfo>(SPRITE_WIDTH * 2.5, SPRITE_HEIGHT * 2.5, MAX_WIDTH / 2.f, 60.f, Types::OT_UI, TEXT("ItemInfo"), this);
		if (pInfo == nullptr)
			return false;
		m_ObjectPtrList.emplace_back(pInfo);
		FindLayer(TEXT("UI"))->AddActor(pInfo);
	}

	return true;
}

bool CGameScene::BuildWorld()
{
	//Pickup 생성
	{
		if (!CreateLayer(TEXT("Pickup"), 3))
			return false;

		//테스트용 Mushroom 생성
		std::shared_ptr<CPickup> pPickup = m_pObjectManager->CreateActor<CMushroom>(SPRITE_WIDTH, SPRITE_HEIGHT, 300.f, 150.f, Types::OT_PICKUP,
			Types::OS_IDLE, Types::VS_IDLE, Types::HS_RUN, Types::DIR_RIGHT, TEXT("Mushroom"), this);
		if (pPickup == nullptr)
			return false;
		FindLayer(TEXT("Pickup"))->AddActor(pPickup);
		m_ObjectPtrList.emplace_back(pPickup);

		//////테스트용 Flower 생성
		////pPickup = m_pObjectManager->CreateActor<CFlower>(SPRITE_WIDTH, SPRITE_HEIGHT, 300.f, 150.f, Types::OT_PICKUP, Types::DIR_RIGHT, TEXT("Flower"), this);
		////if (pPickup == nullptr)
		////	return false;
		////FindLayer(TEXT("Pickup"))->AddActor(pPickup);
		////m_ObjectPtrList.emplace_back(pPickup);

		//////테스트용 Coin 생성
		////pPickup = m_pObjectManager->CreateActor<CCoin>(SPRITE_WIDTH, SPRITE_HEIGHT, 330.f, 150.f, Types::OT_PICKUP, Types::DIR_RIGHT, TEXT("Coin"), this);
		////if (pPickup == nullptr)
		////	return false;
		////FindLayer(TEXT("Pickup"))->AddActor(pPickup);
		////m_ObjectPtrList.emplace_back(pPickup);

		//////테스트용 Coin 생성
		////pPickup = m_pObjectManager->CreateActor<CCoin>(SPRITE_WIDTH, SPRITE_HEIGHT, 370.f, 150.f, Types::OT_PICKUP, Types::DIR_RIGHT, TEXT("Coin"), this);
		////if (pPickup == nullptr)
		////	return false;
		////FindLayer(TEXT("Pickup"))->AddActor(pPickup);
		////m_ObjectPtrList.emplace_back(pPickup);

		//////테스트용 Coin 생성
		////pPickup = m_pObjectManager->CreateActor<CCoin>(SPRITE_WIDTH, SPRITE_HEIGHT, 410.f, 150.f, Types::OT_PICKUP, Types::DIR_RIGHT, TEXT("Coin"), this);
		////if (pPickup == nullptr)
		////	return false;
		////FindLayer(TEXT("Pickup"))->AddActor(pPickup);
		////m_ObjectPtrList.emplace_back(pPickup);

		//////테스트용 Coin 생성
		////pPickup = m_pObjectManager->CreateActor<CCoin>(SPRITE_WIDTH, SPRITE_HEIGHT, 450.f, 150.f, Types::OT_PICKUP, Types::DIR_RIGHT, TEXT("Coin"), this);
		////if (pPickup == nullptr)
		////	return false;
		////FindLayer(TEXT("Pickup"))->AddActor(pPickup);
		////m_ObjectPtrList.emplace_back(pPickup);

		////테스트용 EndPickup 생성
		//pPickup = m_pObjectManager->CreateActor<CEndPickup>(SPRITE_WIDTH, SPRITE_HEIGHT, 8170.f, 444.f, Types::OT_PICKUP, Types::DIR_RIGHT, TEXT("EndPickup"), this);
		//if (pPickup == nullptr)
		//	return false;
		//FindLayer(TEXT("Pickup"))->AddActor(pPickup);
		//m_ObjectPtrList.emplace_back(pPickup);
	}

	//Enemy 생성
	{
		if (!CreateLayer(TEXT("Enemy"), 4))
			return false;

		std::shared_ptr<CEnemy> pEnemy = m_pObjectManager->CreateActor<CKoopa>(SPRITE_WIDTH, SPRITE_HEIGHT*1.8f, 250.f, 250.f, Types::OT_ENEMY,
			Types::OS_IDLE, Types::VS_IDLE, Types::HS_RUN, Types::DIR_LEFT, TEXT("KoopaGreen"), this);
		if (pEnemy == nullptr)
			return false;
		m_ObjectPtrList.emplace_back(pEnemy);
		FindLayer(TEXT("Enemy"))->AddActor(pEnemy);


		//pEnemy = m_pObjectManager->CreateActor<CKoopa>(SPRITE_WIDTH, SPRITE_HEIGHT*1.8f, 200.f, 250.f, Types::OT_ENEMY,
		//	Types::OS_IDLE, Types::VS_IDLE, Types::HS_RUN, Types::DIR_LEFT, TEXT("KoopaGreen"), this);
		//if (pEnemy == nullptr)
		//	return false;
		//m_ObjectPtrList.emplace_back(pEnemy);
		//FindLayer(TEXT("Enemy"))->AddActor(pEnemy);


		////pEnemy = m_pObjectManager->CreateActor<CKoopa>(SPRITE_WIDTH, SPRITE_HEIGHT*1.8f, 150.f, 250.f, Types::OT_ENEMY,
		////	Types::OS_IDLE, Types::VS_IDLE, Types::HS_RUN, Types::DIR_LEFT, TEXT("KoopaGreen"), this);
		////if (pEnemy == nullptr)
		////	return false;
		////m_ObjectPtrList.emplace_back(pEnemy);
		////FindLayer(TEXT("Enemy"))->AddActor(pEnemy);


		////pEnemy = m_pObjectManager->CreateActor<CKoopa>(SPRITE_WIDTH, SPRITE_HEIGHT*1.8f, 300.f, 250.f, Types::OT_ENEMY,
		////	Types::OS_IDLE, Types::VS_IDLE, Types::HS_RUN, Types::DIR_LEFT, TEXT("KoopaGreen"), this);
		////if (pEnemy == nullptr)
		////	return false;
		////m_ObjectPtrList.emplace_back(pEnemy);
		////FindLayer(TEXT("Enemy"))->AddActor(pEnemy);


		////pEnemy = m_pObjectManager->CreateActor<CGoomba>(SPRITE_WIDTH, SPRITE_HEIGHT, 300.f, 450.f, Types::OT_ENEMY,
		////	Types::OS_IDLE, Types::VS_IDLE, Types::HS_RUN, Types::DIR_LEFT, TEXT("Goomba"), this);
		////if (pEnemy == nullptr)
		////	return false;
		////m_ObjectPtrList.emplace_back(pEnemy);
		////FindLayer(TEXT("Enemy"))->AddActor(pEnemy);


		////pEnemy = m_pObjectManager->CreateActor<CRex>(SPRITE_WIDTH, SPRITE_HEIGHT*1.8f, 500.f, 444.f, Types::OT_ENEMY,
		////	Types::OS_IDLE, Types::VS_IDLE, Types::HS_RUN, Types::DIR_LEFT, TEXT("Rex"), this);
		////if (pEnemy == nullptr)
		////	return false;
		////m_ObjectPtrList.emplace_back(pEnemy);
		////FindLayer(TEXT("Enemy"))->AddActor(pEnemy);


		////pEnemy = m_pObjectManager->CreateActor<CKoopa>(SPRITE_WIDTH, SPRITE_HEIGHT*1.8f, 550.f, 444.f, Types::OT_ENEMY,
		////	Types::OS_IDLE, Types::VS_IDLE, Types::HS_RUN, Types::DIR_LEFT, TEXT("KoopaGreen"), this);
		////if (pEnemy == nullptr)
		////	return false;
		////m_ObjectPtrList.emplace_back(pEnemy);
		////FindLayer(TEXT("Enemy"))->AddActor(pEnemy);


		//pEnemy = m_pObjectManager->CreateActor<CKoopa>(SPRITE_WIDTH, SPRITE_HEIGHT*1.8f, 570.f, 444.f, Types::OT_ENEMY,
		//	Types::OS_IDLE, Types::VS_IDLE, Types::HS_RUN, Types::DIR_LEFT, TEXT("KoopaGreen"), this);
		//if (pEnemy == nullptr)
		//	return false;
		//m_ObjectPtrList.emplace_back(pEnemy);
		//FindLayer(TEXT("Enemy"))->AddActor(pEnemy);
		//
	}

	//Prob 생성
	{
		if (!CreateLayer(TEXT("Prob"), 8))
			return false;

		auto pProb = m_pObjectManager->CreateObject<CPipe>(1, 7, 400.f, 600.f, Types::OT_PROB, TEXT("PIPE"), this);
		if (pProb == nullptr)
			return false;
		FindLayer(TEXT("Prob"))->AddActor(pProb);
		m_ObjectPtrList.emplace_back(pProb);


		pProb = m_pObjectManager->CreateObject<CPipe>(1, 3, 0.f, 600.f, Types::OT_PROB, TEXT("PIPE"), this);
		if (pProb == nullptr)
			return false;
		FindLayer(TEXT("Prob"))->AddActor(pProb);
		m_ObjectPtrList.emplace_back(pProb);


		//pProb = m_pObjectManager->CreateObject<CPipe>(1, 3, 1000.f, 600.f, Types::OT_PROB, TEXT("PIPE"), this);
		//if (pProb == nullptr)
		//	return false;
		//FindLayer(TEXT("Prob"))->AddActor(pProb);
		//m_ObjectPtrList.emplace_back(pProb);


		//pProb = m_pObjectManager->CreateObject<CPipe>(1, 4, 1200.f, 250.f, Types::OT_PROB, TEXT("PIPE"), this);
		//if (pProb == nullptr)
		//	return false;
		//FindLayer(TEXT("Prob"))->AddActor(pProb);
		//m_ObjectPtrList.emplace_back(pProb);


		//pProb = m_pObjectManager->CreateObject<CPipe>(1, 4, 1300.f, 600.f, Types::OT_PROB, TEXT("PIPE"), this);
		//if (pProb == nullptr)
		//	return false;
		//FindLayer(TEXT("Prob"))->AddActor(pProb);
		//m_ObjectPtrList.emplace_back(pProb);


		//pProb = m_pObjectManager->CreateObject<CPipe>(1, 4, 1600.f, 600.f, Types::OT_PROB, TEXT("PIPE"), this);
		//if (pProb == nullptr)
		//	return false;
		//FindLayer(TEXT("Prob"))->AddActor(pProb);
		//m_ObjectPtrList.emplace_back(pProb);
	}

	//Block 생성
	{
		if (!CreateLayer(TEXT("Block"), 9))
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
			m_ObjectPtrList.emplace_back(pPickup);
			if (pPickup == nullptr)
				return false;
			//Pickup set
			pBlock->SetStoredPickup(pPickup);
			m_ObjectPtrList.push_back(pBlock);
			FindLayer(TEXT("Block"))->AddActor(pBlock);


			//pBlock = m_pObjectManager->CreateObject<CPickupBlock>(SPRITE_WIDTH*1.2, SPRITE_HEIGHT*1.2, 460.f, 300.f, Types::OT_BLOCK, TEXT("Block"), this);
			//if (pBlock == nullptr)
			//	return false;
			////Block에 저장시켜놓을 Pickup 생성
			//pPickup = m_pObjectManager->CreateActor<CFlower>(SPRITE_WIDTH, SPRITE_HEIGHT,
			//	pBlock->GetObjectPosition().x, pBlock->GetObjectPosition().y - pBlock->GetObjectHeight() / 2.f, Types::OT_PICKUP, Types::DIR_RIGHT, TEXT("Flower"), this);
			//FindLayer(TEXT("Pickup"))->AddActor(pPickup);
			//m_ObjectPtrList.emplace_back(pPickup);
			//if (pPickup == nullptr)
			//	return false;
			////Pickup set
			//pBlock->SetStoredPickup(pPickup);
			//m_ObjectPtrList.push_back(pBlock);
			//FindLayer(TEXT("Block"))->AddActor(pBlock);


			//pBlock = m_pObjectManager->CreateObject<CPickupBlock>(SPRITE_WIDTH*1.2, SPRITE_HEIGHT*1.2, 520.f, 300.f, Types::OT_BLOCK, TEXT("Block"), this);
			//if (pBlock == nullptr)
			//	return false;
			////Block에 저장시켜놓을 Pickup 생성
			//pPickup = m_pObjectManager->CreateActor<CFlower>(SPRITE_WIDTH, SPRITE_HEIGHT,
			//	pBlock->GetObjectPosition().x, pBlock->GetObjectPosition().y - pBlock->GetObjectHeight() / 2.f, Types::OT_PICKUP, Types::DIR_RIGHT, TEXT("Flower"), this);
			//FindLayer(TEXT("Pickup"))->AddActor(pPickup);
			//m_ObjectPtrList.emplace_back(pPickup);
			//if (pPickup == nullptr)
			//	return false;
			////Pickup set
			//pBlock->SetStoredPickup(pPickup);
			//pBlock->SetHide();
			//m_ObjectPtrList.push_back(pBlock);
			//FindLayer(TEXT("Block"))->AddActor(pBlock);
		}

		//Coin block
		{
			auto pBlock = m_pObjectManager->CreateObject<CCoinBlock>(SPRITE_WIDTH*1.2, SPRITE_HEIGHT*1.2, 620.f, 300.f, Types::OT_BLOCK, TEXT("Block"), this);
			if (pBlock == nullptr)
				return false;
			//pBlock->SetLimitTime(3.f);
			m_ObjectPtrList.push_back(pBlock);
			FindLayer(TEXT("Block"))->AddActor(pBlock);

			//pBlock = m_pObjectManager->CreateObject<CCoinBlock>(SPRITE_WIDTH*1.2, SPRITE_HEIGHT*1.2, 660.f, 300.f, Types::OT_BLOCK, TEXT("Block"), this);
			//if (pBlock == nullptr)
			//	return false;
			//pBlock->SetHide();
			//m_ObjectPtrList.push_back(pBlock);
			//FindLayer(TEXT("Block"))->AddActor(pBlock);
		}

		//Spin block
		{
			auto pBlock = m_pObjectManager->CreateObject<CSpinBlock>(SPRITE_WIDTH*1.2, SPRITE_HEIGHT*1.2, 580.f, 300.f, Types::OT_BLOCK, TEXT("Block"), this);
			if (pBlock == nullptr)
				return false;
			m_ObjectPtrList.push_back(pBlock);
			FindLayer(TEXT("Block"))->AddActor(pBlock);
		}

	}

	//Ground생성
	{
		if (!CreateLayer(TEXT("Ground"), 10))
			return false;

		//auto pGround = m_pObjectManager->CreateObject<CGround>(25, 14, 600.f, 250.f, Types::OT_GROUND, TEXT("Ground"), this);
		//if (pGround == nullptr)
		//	return false;
		//m_ObjectPtrList.push_back(pGround);
		//FindLayer(TEXT("Ground"))->AddActor(pGround);


		auto pGround = m_pObjectManager->CreateObject<CGround>(100, 8, 0.f, 600.f, Types::OT_PROB, TEXT("Ground"), this);
		if (pGround == nullptr)
			return false;
		m_ObjectPtrList.push_back(pGround);
		FindLayer(TEXT("Ground"))->AddActor(pGround);


		//pGround = m_pObjectManager->CreateObject<CGround>(20, 30, -640.f, -100.f, Types::OT_PROB, TEXT("Ground"), this);
		//if (pGround == nullptr)
		//	return false;
		//m_ObjectPtrList.push_back(pGround);
		//FindLayer(TEXT("Ground"))->AddActor(pGround);


		//pGround = m_pObjectManager->CreateObject<CGround>(2, 8, 3250.f, 600.f, Types::OT_PROB, TEXT("Ground"), this);
		//if (pGround == nullptr)
		//	return false;
		//m_ObjectPtrList.push_back(pGround);
		//FindLayer(TEXT("Ground"))->AddActor(pGround);





		//pGround = m_pObjectManager->CreateObject<CGround>(2, 8, 3400.f, 600.f, Types::OT_PROB, TEXT("Ground"), this);
		//if (pGround == nullptr)
		//	return false;
		//m_ObjectPtrList.push_back(pGround);
		//FindLayer(TEXT("Ground"))->AddActor(pGround);


		//pGround = m_pObjectManager->CreateObject<CGround>(20, 8, 3660.f, 600.f, Types::OT_PROB, TEXT("Ground"), this);
		//if (pGround == nullptr)
		//	return false;
		//m_ObjectPtrList.push_back(pGround);
		//FindLayer(TEXT("Ground"))->AddActor(pGround);


		//pGround = m_pObjectManager->CreateObject<CGround>(10, 2, 3980.f, 600.f, Types::OT_PROB, TEXT("Ground"), this);
		//if (pGround == nullptr)
		//	return false;
		//m_ObjectPtrList.push_back(pGround);
		//FindLayer(TEXT("Ground"))->AddActor(pGround);


		//pGround = m_pObjectManager->CreateObject<CGround>(8, 2, 4044.f, 416.f, Types::OT_PROB, TEXT("Ground"), this);
		//if (pGround == nullptr)
		//	return false;
		//m_ObjectPtrList.push_back(pGround);
		//FindLayer(TEXT("Ground"))->AddActor(pGround);


		//pGround = m_pObjectManager->CreateObject<CGround>(6, 2, 4108.f, 452.f, Types::OT_PROB, TEXT("Ground"), this);
		//if (pGround == nullptr)
		//	return false;
		//m_ObjectPtrList.push_back(pGround);
		//FindLayer(TEXT("Ground"))->AddActor(pGround);


		//pGround = m_pObjectManager->CreateObject<CGround>(4, 2, 4172.f, 388.f, Types::OT_PROB, TEXT("Ground"), this);
		//if (pGround == nullptr)
		//	return false;
		//m_ObjectPtrList.push_back(pGround);
		//FindLayer(TEXT("Ground"))->AddActor(pGround);


		//pGround = m_pObjectManager->CreateObject<CGround>(2, 2, 4236.f, 324.f, Types::OT_PROB, TEXT("Ground"), this);
		//if (pGround == nullptr)
		//	return false;
		//m_ObjectPtrList.push_back(pGround);
		//FindLayer(TEXT("Ground"))->AddActor(pGround);


		//pGround = m_pObjectManager->CreateObject<CGround>(15, 8, 4736.f, 444.f, Types::OT_PROB, TEXT("Ground"), this);
		//if (pGround == nullptr)
		//	return false;
		//m_ObjectPtrList.push_back(pGround);
		//FindLayer(TEXT("Ground"))->AddActor(pGround);

		//pGround = m_pObjectManager->CreateObject<CGround>(5, 4, 5300.f, 270.f, Types::OT_PROB, TEXT("Ground"), this);
		//if (pGround == nullptr)
		//	return false;
		//m_ObjectPtrList.push_back(pGround);
		//FindLayer(TEXT("Ground"))->AddActor(pGround);


		//pGround = m_pObjectManager->CreateObject<CGround>(5, 4, 5500.f, 200.f, Types::OT_PROB, TEXT("Ground"), this);
		//if (pGround == nullptr)
		//	return false;
		//m_ObjectPtrList.push_back(pGround);
		//FindLayer(TEXT("Ground"))->AddActor(pGround);


		//pGround = m_pObjectManager->CreateObject<CGround>(5, 4, 5900.f, 220.f, Types::OT_PROB, TEXT("Ground"), this);
		//if (pGround == nullptr)
		//	return false;
		//m_ObjectPtrList.push_back(pGround);
		//FindLayer(TEXT("Ground"))->AddActor(pGround);


		//pGround = m_pObjectManager->CreateObject<CGround>(10, 4, 6100.f, 400.f, Types::OT_PROB, TEXT("Ground"), this);
		//if (pGround == nullptr)
		//	return false;
		//m_ObjectPtrList.push_back(pGround);
		//FindLayer(TEXT("Ground"))->AddActor(pGround);


		//pGround = m_pObjectManager->CreateObject<CGround>(3, 8, 6600.f, 444.f, Types::OT_PROB, TEXT("Ground"), this);
		//if (pGround == nullptr)
		//	return false;
		//m_ObjectPtrList.push_back(pGround);
		//FindLayer(TEXT("Ground"))->AddActor(pGround);


		//pGround = m_pObjectManager->CreateObject<CGround>(30, 8, 6750.f, 444.f, Types::OT_PROB, TEXT("Ground"), this);
		//if (pGround == nullptr)
		//	return false;
		//m_ObjectPtrList.push_back(pGround);
		//FindLayer(TEXT("Ground"))->AddActor(pGround);


		//pGround = m_pObjectManager->CreateObject<CGround>(10, 8, 8010.f, 444.f, Types::OT_PROB, TEXT("Ground"), this);
		//if (pGround == nullptr)
		//	return false;
		//m_ObjectPtrList.push_back(pGround);
		//FindLayer(TEXT("Ground"))->AddActor(pGround);
	}

	//Backgorund 생성
	{
		if (!CreateLayer(TEXT("Background"), 99))
			return false;

		auto pBack = m_pObjectManager->CreateObject<CBackground>(MAX_WIDTH, MAX_HEIGHT, 0.f, 0.f, Types::OT_BACKGROUND, TEXT("Background"), this);
		if (pBack == nullptr)
			return false;
		pBack->SetBackgroundImage(TEXT("BackgroundMountain2"));
		m_ObjectPtrList.emplace_back(pBack);
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
		m_iCoinCount = m_pScoreManager->GetCoinCount();
		m_iLife = m_pScoreManager->GetLifeCount();

		//Actor Update
		for (const auto& actor : m_ObjectPtrList) {
			if (actor->IsActive())
			{
				actor->Update(dDeltaTime);
			}
		}

		//Collsion detect between Actors
		CCollisionManager::GetInstance()->CheckCollision();
		//Update main camera
		CCameraManager::GetInstance()->GetMainCamera().lock()->Update(dDeltaTime);
		//Adjust position on screen 
		for (auto it = m_ObjectPtrList.cbegin(); it != m_ObjectPtrList.cend(); ++it)
		{
			//OBJECT_STATE state = (*it)->GetObjectState();
			//if (state != Types::OS_DEAD && state != Types::OS_DESTROYED)
			//{
				(*it)->LateUpdate();
			//}
		}

	}
	else
	{
		m_pPlayer.lock()->DeadProcess(dDeltaTime);
		//m_pPlayer.lock()->LateUpdate();
	}
	

}

void CGameScene::ResetScene()
{
	m_bClear				= false;
	m_iRemainTime	= 999;
	m_dTimeElapsed	= 0.f;
	CScene::ResetScene();
}



