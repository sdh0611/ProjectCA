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
	m_fDamage			= 0.f;
	m_dTimeElapsed	= 0.f;

	return CActor::PostInit(data ,pScene);
}

void CEnemy::Init()
{
	//m_actorPoint = m_spawnPoint;
	for (auto& it : m_ComponentTable)
		it.second->Init();

	m_bActive			= true;
	m_ActType			= Types::ACT_IDLE;
	m_ObjectType		= Types::OT_ENEMY;
	m_ObjectState		= Types::OS_IDLE;
	m_Direction			= Types::DIR_LEFT;
	m_dTimeElapsed	= 0.f;
}

void CEnemy::Update(double fDeltaTime)
{
	CActor::Update(fDeltaTime);

}

void CEnemy::Render(const HDC & hDC)
{
	POSITION position = GetComponent<TransformComponent>().lock()->GetPosition();

	Rectangle(hDC, position.x, position.y, position.x + m_iObjectWidth, position.y + m_iObjectHeight);
	if (GetComponent<Collider>().lock()->IsCollision()) {
		TextOut(hDC, position.x, position.y, TEXT("TRUE"), sizeof(TEXT("TRUE")));
	}
	else {
		TextOut(hDC, position.x, position.y, TEXT("FALSE"), sizeof(TEXT("FALSE")));
	}

}


