#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CEnemy.h"
#include "..\..\..\Include\Core\Components\AIComponent.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\Components\ColliderBox.h"



CEnemy::CEnemy()
	:CActor()
{

}

CEnemy::~CEnemy()
{
}

//bool CEnemy::Init(const Types::ActorData &)
//{
//
//
//
//	return true;
//}

bool CEnemy::PostInit(const Types::ActorData& data, CGameScene* pScene)
{
	return true;
}

bool CEnemy::Init()
{
	//m_actorPoint = m_spawnPoint;
	for (auto& it : m_componentTable)
		it.second->Init();

	m_bActive = true;
	m_actorCurState = Types::AS_IDLE;
	m_direction = Types::DIR_LEFT;
	
	return true;
}

void CEnemy::Update(double fDeltaTime)
{
	CActor::Update(fDeltaTime);

}

void CEnemy::Render(const HDC & hDC)
{
	POSITION position = GetComponent<TransformComponent>().lock()->GetPosition();

	Rectangle(hDC, position.x, position.y, position.x + m_iActorWidth, position.y + m_iActorHeight);
	if (GetComponent<Collider>().lock()->IsCollision()) {
		TextOut(hDC, position.x, position.y, TEXT("TRUE"), sizeof(TEXT("TRUE")));
	}
	else {
		TextOut(hDC, position.x, position.y, TEXT("FALSE"), sizeof(TEXT("FALSE")));
	}

}

void CEnemy::ActorBehavior(double dDeltaTime)
{

}


