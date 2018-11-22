#include "..\..\stdafx.h"
#include "..\..\Include\Scene\CGameScene.h"
#include "..\..\Include\Scene\CLayer.h"
#include "..\..\Include\Core\CCollisionManager.h"
#include "..\..\Include\Scene\Actor\CObjectManager.h"
#include "..\..\Include\Scene\CCameraManager.h"
#include "..\..\Include\Core\Components\Collider.h"
#include "..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\Include\Core\Components\InputComponent.h"
#include "..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\Include\Scene\CObject.h"
#include "..\..\Include\Scene\Actor\CActor.h"
#include "..\..\Include\Scene\Actor\CEnemy.h"
#include "..\..\Include\Scene\Actor\CKoopa.h"
#include "..\..\Include\Scene\Actor\CGoomba.h"
#include "..\..\Include\Scene\Actor\CPlayer.h"
#include "..\..\Include\Scene\Actor\CProb.h"
#include "..\..\Include\Scene\Actor\CGround.h"
#include "..\..\Include\Scene\Actor\CBlock.h"
#include "..\..\Include\Scene\Actor\CCamera.h"
#include "..\..\Include\Scene\Actor\CBackground.h"
#include "..\..\Include\Scene\Actor\CMushroom.h"
#include "..\..\Include\Scene\Actor\CFlower.h"
//#include "..\..\Include\Scene\CWorld.h"



CGameScene::CGameScene(Types::SceneType type)
	:CScene(type), m_pObjectManager(nullptr), m_pPlayer(nullptr)
{
	//기본 레이어 생성.
	//ActorManager에서 Actor를 생성할 경우 기본적으로 default레이어에 들어가게 할 것.
	//Layer order 0은 UI에 줄 것.
	CreateLayer(TEXT("default"), 1);

}

CGameScene::~CGameScene()
{
	//SAFE_DELETE(m_pCurWorld)
	//SAFE_DELETE(m_pNextWorld)
	//m_pObjectManager->Destroy();
	//m_ObjectPtrList.clear();
	puts("Destroy Game");
	CCollisionManager::GetInstance()->Destroy();
}

//Layer우선순위 0번은 추후 UI에 줄 예정임.
bool CGameScene::Init()
{
	if (!CCollisionManager::GetInstance()->Init())
		return false;

	////if(m_pCurWorld == nullptr)
	////	m_pCurWorld = new CWorld;

	//Player 생성
	m_pPlayer = m_pObjectManager->CreateActor<CPlayer>(SPRITE_WIDTH*2.5, SPRITE_HEIGHT*2.5, 0, 0, Types::OT_PLAYER,
		Types::DIR_RIGHT, TEXT("Player"), this);

	m_ObjectPtrList.emplace_back(m_pPlayer);

	if (m_pPlayer == nullptr)
		return false;

	auto pCamera = CCameraManager::GetInstance()->CreateCamera(m_pPlayer, MAX_WIDTH, MAX_HEIGHT);
	if (pCamera.expired())
	{
		return false;
	}
	pCamera.lock()->SetCameraMode(CCamera::CM_SCROLL_HOR);
	//카메라 부착
	m_pPlayer->AttachCamera(pCamera.lock());
	SetSceneMainCamera(pCamera.lock());
	
	if (!CreateLayer(TEXT("Player"), 2))
		return false;

	FindLayer(TEXT("Player"))->AddActor(m_pPlayer);
	

	//Enemy 생성
	std::shared_ptr<CEnemy> pEnemy = m_pObjectManager->CreateActor<CKoopa>(SPRITE_WIDTH*2.5, SPRITE_HEIGHT*2.5, 250.f, 250.f, Types::OT_ENEMY,
		Types::OS_IDLE, Types::VS_IDLE, Types::HS_RUN, Types::DIR_LEFT, TEXT("KoopaGreen"), this);
	if (pEnemy == nullptr)
		return false;

	m_ObjectPtrList.emplace_back(pEnemy);
	if (!CreateLayer(TEXT("Enemy"), 4))
		return false;
	FindLayer(TEXT("Enemy"))->AddActor(pEnemy);


	pEnemy = m_pObjectManager->CreateActor<CKoopa>(SPRITE_WIDTH*2.5, SPRITE_HEIGHT*2.5, 200.f, 250.f, Types::OT_ENEMY,
		Types::OS_IDLE, Types::VS_IDLE, Types::HS_RUN, Types::DIR_LEFT, TEXT("KoopaGreen"), this);
	if (pEnemy == nullptr)
		return false;

	m_ObjectPtrList.emplace_back(pEnemy);
	FindLayer(TEXT("Enemy"))->AddActor(pEnemy);


	pEnemy = m_pObjectManager->CreateActor<CKoopa>(SPRITE_WIDTH*2.5, SPRITE_HEIGHT*2.5, 150.f, 250.f, Types::OT_ENEMY,
		Types::OS_IDLE, Types::VS_IDLE, Types::HS_RUN, Types::DIR_LEFT, TEXT("KoopaGreen"), this);
	if (pEnemy == nullptr)
		return false;

	m_ObjectPtrList.emplace_back(pEnemy);
	FindLayer(TEXT("Enemy"))->AddActor(pEnemy);


	pEnemy = m_pObjectManager->CreateActor<CKoopa>(SPRITE_WIDTH*2.5, SPRITE_HEIGHT*2.5, 300.f, 250.f, Types::OT_ENEMY,
		Types::OS_IDLE, Types::VS_IDLE, Types::HS_RUN, Types::DIR_LEFT, TEXT("KoopaGreen"), this);
	if (pEnemy == nullptr)
		return false;

	m_ObjectPtrList.emplace_back(pEnemy);
	FindLayer(TEXT("Enemy"))->AddActor(pEnemy);

	pEnemy = m_pObjectManager->CreateActor<CGoomba>(SPRITE_WIDTH*2.5, SPRITE_HEIGHT*2.5, 350.f, 250.f, Types::OT_ENEMY,
		Types::OS_IDLE, Types::VS_IDLE, Types::HS_RUN, Types::DIR_LEFT, TEXT("Goomba"), this);
	if (pEnemy == nullptr)
		return false;

	m_ObjectPtrList.emplace_back(pEnemy);
	FindLayer(TEXT("Enemy"))->AddActor(pEnemy);


	//////테스트용 Mushroom 생성
	//std::shared_ptr<CActor> pPickup = m_pObjectManager->CreateActor<CMushroom>(SPRITE_WIDTH*2.5, SPRITE_HEIGHT*2.5, 300.f, 150.f, Types::OT_PICKUP,
	//	Types::OS_IDLE, Types::VS_IDLE, Types::HS_RUN, Types::DIR_RIGHT, TEXT("Mushroom"), this);
	//if (pPickup == nullptr)
	//	return false;
	if (!CreateLayer(TEXT("Pickup"), 3))
		return false;
	//FindLayer(TEXT("Pickup"))->AddActor(pPickup);
	//m_ObjectPtrList.emplace_back(pPickup);

	////테스트용 Flower 생성
	// pPickup = m_pObjectManager->CreateActor<CFlower>(SPRITE_WIDTH*2.5, SPRITE_HEIGHT*2.5, 300.f, 150.f, Types::OT_PICKUP,
	//	Types::OS_IDLE, Types::VS_IDLE, Types::HS_RUN, Types::DIR_RIGHT, TEXT("Flower"), this);
	//if (pPickup == nullptr)
	//	return false;
	//FindLayer(TEXT("Pickup"))->AddActor(pPickup);
	//m_ObjectPtrList.emplace_back(pPickup);

	//pPickup = m_pObjectManager->CreateActor<CFlower>(SPRITE_WIDTH*2.5, SPRITE_HEIGHT*2.5, 330.f, 150.f, Types::OT_PICKUP,
	//	Types::OS_IDLE, Types::VS_IDLE, Types::HS_RUN, Types::DIR_RIGHT, TEXT("Flower"), this);
	//if (pPickup == nullptr)
	//	return false;
	//FindLayer(TEXT("Pickup"))->AddActor(pPickup);
	//m_ObjectPtrList.emplace_back(pPickup);

	//pPickup = m_pObjectManager->CreateActor<CFlower>(SPRITE_WIDTH*2.5, SPRITE_HEIGHT*2.5, 360.f, 150.f, Types::OT_PICKUP,
	//	Types::OS_IDLE, Types::VS_IDLE, Types::HS_RUN, Types::DIR_RIGHT, TEXT("Flower"), this);
	//if (pPickup == nullptr)
	//	return false;
	//FindLayer(TEXT("Pickup"))->AddActor(pPickup);
	//m_ObjectPtrList.emplace_back(pPickup);



	//Prob 생성
	if (!CreateLayer(TEXT("Prob"), 10))
		return false;

	std::shared_ptr<CGround> pGround = m_pObjectManager->CreateObject<CGround>(8192, 256, 400.f, 700.f, Types::OT_PROB, TEXT("Prob"), this);
	if (pGround == nullptr)
		return false;
	m_ObjectPtrList.push_back(pGround);
	FindLayer(TEXT("Prob"))->AddActor(pGround);

	pGround = m_pObjectManager->CreateObject<CGround>(256, 160, -400.f, 500.f, Types::OT_PROB, TEXT("Prob"), this);
	if (pGround == nullptr)
		return false;
	m_ObjectPtrList.push_back(pGround);
	FindLayer(TEXT("Prob"))->AddActor(pGround);

	pGround = m_pObjectManager->CreateObject<CGround>(256, 160, 0.f, 350.f, Types::OT_PROB, TEXT("Prob"), this);
	if (pGround == nullptr)
		return false;
	m_ObjectPtrList.push_back(pGround);
	FindLayer(TEXT("Prob"))->AddActor(pGround);

	//pGround = m_pObjectManager->CreateObject<CGround>(256, 160, 400.f, 350.f, Types::OT_PROB, TEXT("Prob"), this);
	//if (pGround== nullptr)
	//	return false;
	//m_ObjectPtrList.push_back(pGround);
	//FindLayer(TEXT("Prob"))->AddActor(pGround);

	pGround = m_pObjectManager->CreateObject<CGround>(256, 160, 800.f, 350.f, Types::OT_PROB, TEXT("Prob"), this);
	if (pGround == nullptr)
		return false;
	m_ObjectPtrList.push_back(pGround);
	FindLayer(TEXT("Prob"))->AddActor(pGround);

	pGround = m_pObjectManager->CreateObject<CGround>(256, 160, 1200.f, 500.f, Types::OT_PROB, TEXT("Prob"), this);
	if (pGround == nullptr)
		return false;
	m_ObjectPtrList.push_back(pGround);
	FindLayer(TEXT("Prob"))->AddActor(pGround);

	//Block 생성
	if (!CreateLayer(TEXT("Block"), 9))
		return false;

	std::shared_ptr<CBlock> pBlock = m_pObjectManager->CreateObject<CBlock>(SPRITE_WIDTH*2.5, SPRITE_HEIGHT*2.5, 400.f, 350.f, Types::OT_PROB, TEXT("Block"), this);
	if (pBlock == nullptr)
		return false;
	m_ObjectPtrList.push_back(pBlock);
	FindLayer(TEXT("Block"))->AddActor(pBlock);


	//Backgorund 생성
	std::shared_ptr<CBackground> pBack = m_pObjectManager->CreateObject<CBackground>(MAX_WIDTH, MAX_HEIGHT, 0.f, 0.f, 
		Types::OT_BACKGROUND, TEXT("Background"), this);

	if (pBack == nullptr)
		return false;
	
	pBack->SetBackgroundImage(TEXT("BackgroundMountain2"));
	m_ObjectPtrList.emplace_back(pBack);

	if (!CreateLayer(TEXT("Background"), 99))
		return false;

	FindLayer(TEXT("Background"))->AddActor(pBack);



	CScene::Init();

	return true;
}

void CGameScene::Update(double fDeltaTime)
{

	if (m_pPlayer->IsActive())
	{
		//1. 입력에 따른 동작 수행 후 충돌 검사 및 그에 따른 Actor들과 하위 컴포넌트 동작 Update
		GameUpdate(fDeltaTime);

		//Layer Update -> Rendering을 수행하기 전 expired된 객체가 있는지 검사하기 위함.
		CScene::Update(fDeltaTime);
	}
	if (KEY_ONCE_PRESS(VK_ESCAPE))
	{
		puts("reset");
		ResetScene();
	}
}

void CGameScene::Render(const HDC& hDC)
{
	//Layer객체가 관리하는 Actor들을 Rendering
	CScene::Render(hDC);

}

void CGameScene::CollisionDetect()
{
	//////Scene내의 모든 Actor들에 대한 충돌검사 시행
	//for (auto first = m_ObjectPtrList.begin(); first != m_ObjectPtrList.end(); ++first) {
	//	if ((*first)->IsActive())
	//	{
	//		for (auto second = first; second != m_ObjectPtrList.end(); ++second) {
	//			if (first == second)
	//			{
	//				continue;
	//			}
	//			if ((*second)->IsActive())
	//			{
	//				CCollisionManager::GetInstance()->CheckCollision((*first), (*second));
	//			}
	//		}
	//	}
	//}

	

}

void CGameScene::InputUpdate(double fDeltaTime)
{



}

void CGameScene::GameUpdate(double dDeltaTime)
{	
	if (!m_pPlayer->IsDead())
	{

		//Actor Update
		for (const auto& actor : m_ObjectPtrList) {
			if (actor->IsActive())
			{
				actor->Update(dDeltaTime);
			}
		}

		//Collsion detect between Actors
		//CollisionDetect();
		CCollisionManager::GetInstance()->CheckCollision();
		CCameraManager::GetInstance()->GetMainCamera().lock()->Update(dDeltaTime);
	}
	else
	{
		m_pPlayer->DeadProcess(dDeltaTime);
	}
	
	//Adjust Position on Screen 
	for (auto it = m_ObjectPtrList.cbegin(); it != m_ObjectPtrList.cend(); ++it)
	{
		//if (actor->GetActorType() != Types::AT_BACKGROUND)
		{
			if ((*it)->IsActive())
			{
				(*it)->LateUpdate();
				//++it;
				//object->GetComponent<TransformComponent>().lock()->AdjustScreenPosition();
			}
			//else
			//{
			//	it = m_ObjectPtrList.erase(it);
			//}
		}
	}

	//if (KEY_ONCE_PRESS(VK_ESCAPE))
	//{
	//	puts("reset");
	//	ResetScene();
	//}

}



