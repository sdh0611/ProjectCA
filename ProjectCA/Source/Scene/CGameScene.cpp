#include "..\..\stdafx.h"
#include "..\..\Include\Scene\CGameScene.h"
#include "..\..\Include\Scene\CLayer.h"
#include "..\..\Include\Core\CollisionDetector.h"
#include "..\..\Include\Core\Components\Collider.h"
#include "..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\Include\Core\Components\InputComponent.h"
#include "..\..\Include\Scene\Actor\CActorManager.h"
#include "..\..\Include\Scene\Actor\CActor.h"
#include "..\..\Include\Scene\Actor\CEnemy.h"
#include "..\..\Include\Scene\Actor\CPlayer.h"
//#include "..\..\Include\Scene\CWorld.h"



CGameScene::CGameScene(Types::SceneType type)
	:CScene(type), m_pActorManager(nullptr), m_pPlayer(nullptr),
	m_pCollisionDetector(nullptr)
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
	m_strongActorList.clear();
}

//Layer우선순위 0번은 추후 UI에 줄 예정임.
bool CGameScene::Init()
{
	if(m_pCollisionDetector == nullptr)
		m_pCollisionDetector = std::make_unique<CollisionDetector>();

	m_pActorManager = CActorManager::GetInstance();
	if (!m_pActorManager->Init())
		return false; 

	//if(m_pCurWorld == nullptr)
	//	m_pCurWorld = new CWorld;

	if (!CreateLayer(TEXT("Player"), 2))
		return false;

	//World의 포인터가 들어가는 자리는 임시로 nullptr로 대체함.(09.13)
	//m_pPlayer = m_pActorManager->CreateActor(Types::OT_PLAYER, Types::Point(0, 0),
	//	Types::DIR_IDLE, 100, 100, (this), TEXT("Player"));

	m_pPlayer = m_pActorManager->CreateActor<CPlayer>(100, 100, 0, 0, Types::OT_PLAYER, Types::OS_IDLE, 
		Types::DIR_IDLE, TEXT("Player"), this);

	m_strongActorList.emplace_back(m_pPlayer);

	if (m_pPlayer == nullptr)
		return false;

	FindLayer(TEXT("Player"))->AddActor(m_pPlayer);
	
	std::shared_ptr<CEnemy> pEnemy = m_pActorManager->CreateActor<CEnemy>(50, 50, 250, 250, Types::OT_ENEMY,
		Types::OS_IDLE, Types::DIR_DOWN, TEXT("Enemy"), this);

	if (pEnemy == nullptr)
		return false;

	m_strongActorList.emplace_back(pEnemy);

	if (!CreateLayer(TEXT("Enemy"), 3))
		return false;

	FindLayer(TEXT("Enemy"))->AddActor(pEnemy);

	//StrongActorPtr pProb = m_pActorManager->CreateActor(Types::OT_PROBS, Types::Point(300, 300),
	//	Types::DIR_IDLE, 200, 200, (this), TEXT("Probs"));

	//StrongActorPtr pProb;

	//m_strongActorList.emplace_back(pProb);

	//FindLayer(TEXT("Probs"))->AddActor(pProb);

	//FindLayer(TEXT("Probs"))->AddObjectToLayer(new CProbs(100.f, 400.f, 150.f, 450.f));
	//FindLayer(TEXT("Probs"))->AddObjectToLayer(new CProbs(250.f, 300.f, 300.f, 350.f));
	//FindLayer(TEXT("Probs"))->AddObjectToLayer(new CProbs(300.f, 250.f, 350.f, 300.f));

	CScene::Init();


	return true;
}

void CGameScene::Update(float fDeltaTime)
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
	
	for (auto& first : m_strongActorList) {
		for (auto& second : m_strongActorList) {
			if (first == second)		
				continue;
			m_pCollisionDetector->Update(first, second);
		}
	}

}

void CGameScene::InputUpdate(float fDeltaTime)
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
void CGameScene::GameUpdate(float fDeltaTime)
{
	//Component Update
	for (auto& actor : m_strongActorList) {
		actor->Update(fDeltaTime);
	}

	//Collsion detect between Actors
	CollisionDetect();

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
	//Reset키 검사
	if (KEY_DOWN(VK_BACK)) {

		Init();
	}
}

