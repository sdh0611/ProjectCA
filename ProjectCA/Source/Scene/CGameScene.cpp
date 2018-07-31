#include "..\..\stdafx.h"
#include "..\..\Include\Scene\CGameScene.h"
#include "..\..\Include\Scene\CLayer.h"
#include "..\..\Include\Scene\Object\CPlayer.h"
#include "..\..\Include\Scene\Object\CProbs.h"
#include "..\..\Include\Core\CollisionDetector.h"
#include "..\..\Include\Core\Components\Collider.h"




CGameScene::CGameScene(Types::SceneType type)
	:CScene(type), m_pCollisionDetector(nullptr)
	//:CScene(type)
	//,m_pCurStage(nullptr), m_pNextStage(nullptr)
{
}

CGameScene::~CGameScene()
{
}

//Layer우선순위 0번은 추후 UI에 줄 예정임.
bool CGameScene::Init()
{
	//if(m_pCollisionDetector == nullptr)
	//	m_pCollisionDetector = std::make_unique<CollisionDetector>();

	if (!CreateLayer(TEXT("Player"), 1))
		return false;
	FindLayer(TEXT("Player"))->AddObjectToLayer(new CPlayer(64, 64));

	
	if (!CreateLayer(TEXT("Probs"), 3))
		return false;

	//FindLayer(TEXT("Probs"))->AddObjectToLayer(new CProbs(0.f, 500.f, 800.f, 600.f));
	//FindLayer(TEXT("Probs"))->AddObjectToLayer(new CProbs(100.f, 400.f, 150.f, 450.f));
	FindLayer(TEXT("Probs"))->AddObjectToLayer(new CProbs(200.f, 350.f, 250.f, 400.f));
	//FindLayer(TEXT("Probs"))->AddObjectToLayer(new CProbs(250.f, 300.f, 300.f, 350.f));
	//FindLayer(TEXT("Probs"))->AddObjectToLayer(new CProbs(300.f, 250.f, 350.f, 300.f));


	CScene::Init();


	return true;
}

void CGameScene::Update(float fDeltaTime)
{
	//Scene의 하위 객체들과 컴포넌트들 Update
	CScene::Update(fDeltaTime);

	//Scene내부의 객체들끼리 충돌 검사
	//Bug(06.09) : 여러개의 prob을 추가했더니 마지막 prob만 충돌검사가 수행된다..
	CollisionDetect();

	GameUpdate(fDeltaTime);

}

void CGameScene::Render(const HDC& hDC)
{
	CScene::Render(hDC);
	//InvalidateRect(NULL, NULL, TRUE);
}

//Player와 Enemy
//Player와 Probs
//Enemy와 Probs
//충돌했을 때 충돌한 Object에 대한 정보도 넘겨줘야한다.
void CGameScene::CollisionDetect()
{
	////Player와 Background, UI를 제외한 모든 Layer의 Object들과 충돌 검사.
	for (auto it = FindLayer(TEXT("Player"))->GetObjectList().begin();
		it != FindLayer(TEXT("Player"))->GetObjectList().end(); ++it)
	{		
		for (m_it = m_LayerList.begin(); m_it != m_LayerList.end(); ++m_it) {
			if ( ((*m_it)->GetLayerTag() == TEXT("Background")) || ((*m_it)->GetLayerTag() == TEXT("Player")) )
				continue;

			for ( auto& object : ((*m_it)->GetObjectList()) ) {
				//m_pCollisionDetector->Update( static_cast<Collider*>(value->GetComponent(TEXT("Collider"))), 
				//	static_cast<Collider*>( (*it)->GetComponent(TEXT("Collider")) ) );
				m_pCollisionDetector->Update((*it), object); //it : player

			}
		}
	}

}

//충돌검사 수행 후 처리
void CGameScene::GameUpdate(float fDeltaTime)
{
	if (!m_LayerList.empty()) {
		for (m_it = m_LayerList.begin(); m_it != m_LayerList.end(); ++m_it) {
			

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

