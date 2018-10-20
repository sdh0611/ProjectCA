#include "..\..\stdafx.h"
#include "..\..\Include\Scene\CGameScene.h"
#include "..\..\Include\Scene\CLayer.h"
#include "..\..\Include\Core\CCollisionManager.h"
#include "..\..\Include\Core\Components\Collider.h"
#include "..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\Include\Core\Components\InputComponent.h"
#include "..\..\Include\Scene\Actor\CActorManager.h"
#include "..\..\Include\Scene\Actor\CActor.h"
#include "..\..\Include\Scene\Actor\CEnemy.h"
#include "..\..\Include\Scene\Actor\CKoopa.h"
#include "..\..\Include\Scene\Actor\CPlayer.h"
#include "..\..\Include\Scene\Actor\CProb.h"
#include "..\..\Include\Scene\Actor\CBackground.h"
//#include "..\..\Include\Scene\CWorld.h"



CGameScene::CGameScene(Types::SceneType type)
	:CScene(type), m_pActorManager(nullptr), m_pPlayer(nullptr)
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
	CCollisionManager::GetInstance()->Destroy();

	m_strongActorList.clear();
}

//Layer우선순위 0번은 추후 UI에 줄 예정임.
bool CGameScene::Init()
{
	CCollisionManager::GetInstance()->Init();

	m_pActorManager = CActorManager::GetInstance();
	if (!m_pActorManager->Init())
		return false; 

	////if(m_pCurWorld == nullptr)
	////	m_pCurWorld = new CWorld;

	//Player 생성
	m_pPlayer = m_pActorManager->CreateActor<CPlayer>(SPRITE_WIDTH*2.5, SPRITE_HEIGHT*2.5, 0, 0, Types::AT_PLAYER,
		Types::AS_IDLE, Types::VS_IDLE, Types::HS_IDLE, Types::DIR_RIGHT, Types::Point(0.f, 0.f),TEXT("Player"), this);

	m_strongActorList.emplace_back(m_pPlayer);

	if (m_pPlayer == nullptr)
		return false;

	if (!CreateLayer(TEXT("Player"), 2))
		return false;

	FindLayer(TEXT("Player"))->AddActor(m_pPlayer);
	

	////Enemy 생성
	////Windows 좌표계에선 y축이 반대방향이므로 Vector의 값도 반대로 전달해줌.
	//std::shared_ptr<CKoopa> pEnemy = m_pActorManager->CreateActor<CKoopa>(SPRITE_WIDTH*2.5, SPRITE_HEIGHT*2.5, 250.f, 250.f, Types::AT_ENEMY,
	//	Types::AS_IDLE, Types::VS_IDLE, Types::HS_RUN, Types::DIR_LEFT, Types::Point(0.f, 1.0f), TEXT("KoopaGreen"), this);

	//if (pEnemy == nullptr)
	//	return false;

	//m_strongActorList.emplace_back(pEnemy);

	//if (!CreateLayer(TEXT("Enemy"), 3))
	//	return false;

	//FindLayer(TEXT("Enemy"))->AddActor(pEnemy);


	//Prob 생성
	if (!CreateLayer(TEXT("Prob"), 4))
		return false;
	
	std::shared_ptr<CProb> pProb = m_pActorManager->CreateActor<CProb>(900, 200, 400.f, 700.f, Types::AT_PROB,
		Types::AS_IDLE, Types::VS_IDLE, Types::HS_IDLE, Types::DIR_IDLE, Types::Point(0.f, 0.f), TEXT("Prob"), this);
	if (pProb == nullptr)
		return false;
	m_strongActorList.emplace_back(pProb);
	FindLayer(TEXT("Prob"))->AddActor(pProb);

	pProb = m_pActorManager->CreateActor<CProb>(200, 150, 400.f, 350.f, Types::AT_PROB,
		Types::AS_IDLE, Types::VS_IDLE, Types::HS_IDLE, Types::DIR_IDLE, Types::Point(0.f, 0.f), TEXT("Prob"), this);
	if (pProb == nullptr)
		return false;
	m_strongActorList.emplace_back(pProb);
	FindLayer(TEXT("Prob"))->AddActor(pProb);

	//Backgorund 생성
	std::shared_ptr<CBackground> pBack = m_pActorManager->CreateActor<CBackground>(MAX_WIDTH, MAX_HEIGHT, 0.f, 0.f, Types::AT_BACKGROUND,
		Types::AS_IDLE, Types::VS_IDLE, Types::HS_IDLE, Types::DIR_IDLE, Types::Point(0.f, 0.f), TEXT("Background"), this);

	if (pBack == nullptr)
		return false;

	m_strongActorList.emplace_back(pBack);

	if (!CreateLayer(TEXT("Background"), 99))
		return false;

	FindLayer(TEXT("Background"))->AddActor(pBack);


	CScene::Init();


	return true;
}

void CGameScene::Update(double fDeltaTime)
{
	//1. 입력에 따른 동작 Update
	// TODO(09.17) : 현재 Input을 받는 것을 Player에 있는 PlayerInputComponent에서 수행하고 있는데,
	//					   이 부분을 GameScene Level에서 수행할 수 있게끔 수정할 필요가 있어보임.
	//InputUpdate(fDeltaTime); -> 일단은 임의로 GameUpdate에 몰빵시킴.

	//2. 입력에 따른 동작 수행 후 충돌 검사 및 그에 따른 Actor들과 하위 컴포넌트 동작 Update
	GameUpdate(fDeltaTime);

	//Layer Update -> Rendering을 수행하기 전 expired된 객체가 있는지 검사하기 위함.
	CScene::Update(fDeltaTime);

}

void CGameScene::Render(const HDC& hDC)
{
	//Layer객체가 관리하는 Actor들을 Rendering
	CScene::Render(hDC);

	//m_pPlayer->Render(hDC);
	//InvalidateRect(NULL, NULL, TRUE);
}

//Player와 Enemy
//Player와 Probs
//Enemy와 Probs
//충돌했을 때 충돌한 Object에 대한 정보도 넘겨줘야한다.
void CGameScene::CollisionDetect()
{
	////Scene내의 모든 Actor들에 대한 충돌검사 시행

	//충돌검사를 했던 Actor끼리 다시 검사하게됨.
	//	-> 고쳐야함.
	for (auto first = m_strongActorList.begin(); first != m_strongActorList.end(); ++first) {
		for (auto second = first; second != m_strongActorList.end(); ++second) {
			if (first == second)		
				continue;
			CCollisionManager::GetInstance()->Update((*first), (*second));
		}
	}

}

void CGameScene::InputUpdate(double fDeltaTime)
{
	if (KEY_DOWN(VK_ESCAPE)) {
		//ESC를 누를 경우 일시정치 팝업창 출력, 게임 일시정지

	}
	else {
		//m_pCurWorld->Update(fDeltaTime);
		m_pPlayer->GetComponent(TEXT("InputComponent"))->Update(fDeltaTime);
	}
	


}

//충돌검사 수행 후 처리
void CGameScene::GameUpdate(double fDeltaTime)
{
	//Collsion detect between Actors
	CollisionDetect();
	
	//Component Update
	for (auto& actor : m_strongActorList) {
		actor->Update(fDeltaTime);
	}

	
	//Collision 후 처리 부분이지만, 테스트용으로 일단 Player의 후처리만 하드코딩.(09.17)
	//		-> 정상 작동
	//if (static_cast<Collider*>(m_pPlayer->GetComponent(TEXT("Collider")))->GetIsCollision()) {
	//	PhysicsComponent* pPhysics = static_cast<PhysicsComponent*>(m_pPlayer->GetComponent(TEXT("PhysicsComponent")));
	//	pPhysics->RestoreActorPoint();
	//}

}

//해당 Scene을 리셋할 때 호출하는 메소드.

void CGameScene::ResetScene()
{
	////Reset키 검사
	//if (KEY_DOWN(VK_BACK)) {

	//	Init();
	//}

	for (auto& actor : m_strongActorList) {
		actor->Init();
	}

}

