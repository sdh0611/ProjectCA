#include "..\..\stdafx.h"
#include "..\..\Include\Scene\CGameScene.h"
#include "..\..\Include\Scene\CLayer.h"
#include "..\..\Include\Core\CollisionDetector.h"
#include "..\..\Include\Core\Components\Collider.h"
#include "..\..\Include\Scene\Actor\CActorManager.h"
#include "..\..\Include\Scene\CWorld.h"



CGameScene::CGameScene(Types::SceneType type)
	:CScene(type), m_pActorManager(nullptr), m_pCurWorld(nullptr),
	//m_pNextWorld(nullptr), m_pCollisionDetector(nullptr), m_pPlayer(nullptr)
	m_pNextWorld(nullptr), m_pPlayer(nullptr)
{
}

CGameScene::~CGameScene()
{
	SAFE_DELETE(m_pCurWorld)
	SAFE_DELETE(m_pNextWorld)
}

//Layer우선순위 0번은 추후 UI에 줄 예정임.
bool CGameScene::Init()
{
	//if(m_pCollisionDetector == nullptr)
	//	m_pCollisionDetector = std::make_unique<CollisionDetector>();
	m_pActorManager = CActorManager::GetInstance();
	m_pActorManager->Init();	// 

	if(m_pCurWorld == nullptr)
		m_pCurWorld = new CWorld;

	if (!CreateLayer(TEXT("Player"), 1))
		return false;

	StrongActorPtr pActor = m_pActorManager->CreateActor(Types::OT_PLAYER, Types::Point(0, 0),
		Types::DIR_IDLE, 100, 100, m_pCurWorld, this, TEXT("Player"));
	FindLayer()->AddActor(pActor);


	if (!CreateLayer(TEXT("Probs"), 3))
		return false;

	//FindLayer(TEXT("Probs"))->AddObjectToLayer(new CProbs(0.f, 500.f, 800.f, 600.f));
	//FindLayer(TEXT("Probs"))->AddObjectToLayer(new CProbs(100.f, 400.f, 150.f, 450.f));
	//FindLayer(TEXT("Probs"))->AddObjectToLayer(new CProbs(250.f, 300.f, 300.f, 350.f));
	//FindLayer(TEXT("Probs"))->AddObjectToLayer(new CProbs(300.f, 250.f, 350.f, 300.f));

	CScene::Init();


	return true;
}

void CGameScene::Update(float fDeltaTime)
{
	//1. 입력에 따른 동작 Update
	InputUpdate(fDeltaTime);

	//m_pPlayer->GetComponent(TEXT("InputComponent"))->Update(fDeltaTime);

	//2. 입력에 따른 동작 수행 후 충돌 검사 및 그에 따른 Actor들과 하위 컴포넌트 동작 Update
	GameUpdate(fDeltaTime);

	//Layer Update -> Rendering을 수행하기 전 expired된 객체가 있는지 검사하기 위함.
	CScene::Update(fDeltaTime);

}

void CGameScene::Render(const HDC& hDC)
{
	CScene::Render(hDC);

	m_pPlayer->Render(hDC);
	//InvalidateRect(NULL, NULL, TRUE);
}

//Player와 Enemy
//Player와 Probs
//Enemy와 Probs
//충돌했을 때 충돌한 Object에 대한 정보도 넘겨줘야한다.
//void CGameScene::CollisionDetect()
//{
//	////Player와 Background, UI를 제외한 모든 Layer의 Object들과 충돌 검사.
//	for (auto it = FindLayer(TEXT("Player"))->GetObjectList().begin();
//		it != FindLayer(TEXT("Player"))->GetObjectList().end(); ++it)
//	{		
//		for (m_it = m_LayerList.begin(); m_it != m_LayerList.end(); ++m_it) {
//			if ( ((*m_it)->GetLayerTag() == TEXT("Background")) || ((*m_it)->GetLayerTag() == TEXT("Player")) )
//				continue;
//
//			for ( auto& object : ((*m_it)->GetObjectList()) ) {
//				//m_pCollisionDetector->Update( static_cast<Collider*>(value->GetComponent(TEXT("Collider"))), 
//				//	static_cast<Collider*>( (*it)->GetComponent(TEXT("Collider")) ) );
//				m_pCollisionDetector->Update((*it), object); //it : player
//
//			}
//		}
//	}
//
//}

void CGameScene::InputUpdate(float fDeltaTime)
{
	if (KEY_DOWN(VK_ESCAPE)) {
		//ESC를 누를 경우 일시정치 팝업창 출력, 게임 일시정지

	}
	else {
		m_pCurWorld->Update(fDeltaTime);
	}
	


}

//충돌검사 수행 후 처리
void CGameScene::GameUpdate(float fDeltaTime)
{
	if (!m_LayerList.empty()) {
		for (auto& it : m_LayerList){
			it->LateUpdate(fDeltaTime);
		}

	}


}

//해당 Scene을 리셋할 때 호출하는 메소드.

void CGameScene::ResetScene()
{
	//Reset키 검사
	if (KEY_DOWN(VK_BACK)) {

		Init();
	}
}

