#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\Actor\CKoopa.h"
#include "..\..\..\Include\Scene\CGameScene.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\Components\AIComponent.h"
#include "..\..\..\Include\Scene\CCameraManager.h"
#include "..\..\..\Include\Scene\Actor\CCamera.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
//#include "..\..\..\Include\Core\Components\HPComponent.h"
#include "..\..\..\Include\Core\Components\ColliderBox.h"
#include "..\..\..\Include\Core\Components\RenderComponent.h"
#include "..\..\..\Include\Core\Components\AnimationRender.h"


CKoopa::CKoopa()
	:CEnemy()
{
}

CKoopa::~CKoopa()
{
	puts("Koopa Destroy");
}

bool CKoopa::PostInit(const Types::ActorData & data, CGameScene * pScene)
{
	//기본 Actor의 속성 초기화
	CActor::PostInit(data, pScene);

	//PhysicsComponent 초기화
	std::shared_ptr<PhysicsComponent> pPhysics = std::make_shared<PhysicsComponent>();
	if (!pPhysics->PostInit(this, 200.f, 800.f, 1300.f, 700.f))
		return false;

	pPhysics->SetCurSpeed(pPhysics->GetSpeed());

	if (!AddComponent(pPhysics, pPhysics->GetComponentTag()))
		return false;

	//Collider 초기화
	std::shared_ptr<ColliderBox> pCollider = std::make_shared<ColliderBox>();
	if (!pCollider->PostInit(this))
		return false;

	auto onCollisionDelegater = [&](CObject* pOther, Collider::CollisionType type, float fIntersectLength)->void {

		auto pPhysics = GetComponent<PhysicsComponent>().lock();


		switch (pOther->GetObjectType()) {
		case Types::OT_ENEMY:
			if (m_ObjectState != Types::OS_DAMAGED)
			{
				if (pOther->GetObjectState() == Types::OS_DAMAGED)
				{
					if (pOther->GetComponent<PhysicsComponent>().lock()->GetCurSpeed() != 0.f)
					{
						SetObjectState(Types::OS_DEAD);
						GetComponent<ColliderBox>().lock()->SetActive(false);
						pPhysics->SetCurJumpForce(300.f);
						pPhysics->SetGrounded(false);
					}
					else
					{
						if (type == Collider::COLLISION_LEFT)
						{
							SetObjectPosition(GetObjectPosition().x + fIntersectLength, GetObjectPosition().y);
						}
						else if (type == Collider::COLLISION_RIGHT)
						{
							SetObjectPosition(GetObjectPosition().x - fIntersectLength, GetObjectPosition().y);
						}
						FlipActorDirection();
					}

				}
				else//otherActor != Types::OS_DAMAGED
				{
					if (type == Collider::COLLISION_LEFT)
					{
						SetObjectPosition(GetObjectPosition().x + fIntersectLength, GetObjectPosition().y);
					}
					else if (type == Collider::COLLISION_RIGHT)
					{
						SetObjectPosition(GetObjectPosition().x - fIntersectLength, GetObjectPosition().y);
					}
					FlipActorDirection();
				}

			}
			else//Owner == DAMAGED
			{
				if (pOther->GetObjectState() == Types::OS_DAMAGED)
				{
					if (pOther->GetComponent<PhysicsComponent>().lock()->GetCurSpeed() != 0.f)
					{
						SetObjectState(Types::OS_DEAD);
						pPhysics->AddForceY(300.f);
						pPhysics->SetGrounded(false);
						GetComponent<ColliderBox>().lock()->SetActive(false);
					}

				}
			}
			break;
		case Types::OT_PROB:
			switch (type){
			case Collider::COLLISION_BOT:
				pPhysics->SetGrounded(true);
				SetActorVerticalState(Types::VS_IDLE);
				SetObjectPosition(GetObjectPosition().x, GetObjectPosition().y - fIntersectLength);
				break;
			case Collider::COLLISION_TOP:
				SetObjectPosition(GetObjectPosition().x, GetObjectPosition().y + fIntersectLength);
				break;
			case Collider::COLLISION_LEFT:
				FlipActorDirection();
				if (m_ObjectState == Types::OS_DAMAGED)
				{
					pPhysics->SetCurSpeed(pPhysics->GetMaxSpeed());
				}
				break;
			case Collider::COLLISION_RIGHT:
				FlipActorDirection();
				if (m_ObjectState == Types::OS_DAMAGED)
				{
					pPhysics->SetCurSpeed(-1 * pPhysics->GetMaxSpeed());
				}
				break;
			}
			break;
		case Types::OT_PLAYER:
			switch (type){
			case Collider::COLLISION_TOP:
				SetObjectState(Types::OS_DAMAGED);
				GetComponent<ColliderBox>().lock()->SetCurRectHeight(GetComponent<ColliderBox>().lock()->GetHeight()/2.f);
				pPhysics->SetCurSpeed(0.f);
				GetComponent<AnimationRender>().lock()->ChangeAnimationTable(TEXT("KoopaShell"), TEXT("ShellIdle"));
				break;
			case Collider::COLLISION_RIGHT:
				if (GetObjectState() == Types::OS_DAMAGED)
				{
					if (pPhysics->GetCurSpeed() == 0.f)
					{
						pPhysics->AddForceX(-750.f);
						m_Direction = Types::DIR_LEFT;
					}

				}
				//else
				//{
				//	FlipActorDirection();
				//}
				break;
			case Collider::COLLISION_LEFT:
				//puts("LEFT");
				if (GetObjectState() == Types::OS_DAMAGED)
				{
					if (pPhysics->GetCurSpeed() == 0.f)
					{
						pPhysics->AddForceX(750.f);
						m_Direction = Types::DIR_RIGHT;
					}

				}
				//else
				//{
				//	FlipActorDirection();
				//}
				break;
			
			}
			break;
		//case Types::OT_BULLET:
		//	SetActorState(Types::OS_DAMAGED);
		//	GetComponent<ColliderBox>().lock()->SetCurRectHeight(GetComponent<ColliderBox>().lock()->GetHeight() / 2.f);
		//	pPhysics->SetCurSpeed(0.f);
		//	break;

		}

	};

	pCollider->SetDelegate(onCollisionDelegater);
	pCollider->SetSize(m_iObjectWidth*0.45, m_iObjectHeight*0.8);

	if (!AddComponent(pCollider, pCollider->GetComponentTag()))
		return false;


	//RenderComponent 초기화
	std::shared_ptr<AnimationRender> pRender = std::make_shared<AnimationRender>();
	if (!pRender->PostInit(this))
		return false;

	//평소 상태 animation
	if (!pRender->AddAnimation(0.25f, TEXT("KoopaNormal"),TEXT("KoopaGreenWalkRight"), m_iObjectWidth, m_iObjectHeight, true, TEXT("WalkRight")))
		return false;

	if (!pRender->AddAnimation(0.25f, TEXT("KoopaNormal"), TEXT("KoopaGreenWalkLeft"), m_iObjectWidth, m_iObjectHeight, true, TEXT("WalkLeft")))
		return false;

	//Shell 상태 animation
	if (!pRender->AddAnimation(0.2f, TEXT("KoopaShell"), TEXT("KoopaGreenThrownRight"), m_iObjectWidth, m_iObjectHeight, true, TEXT("ThrownRight")))
		return false;

	if (!pRender->AddAnimation(0.2f, TEXT("KoopaShell"), TEXT("KoopaGreenThrownLeft"), m_iObjectWidth, m_iObjectHeight, true, TEXT("ThrownLeft")))
		return false;

	if (!pRender->AddAnimation(0.25f, TEXT("KoopaShell"), TEXT("KoopaGreenShell"), m_iObjectWidth, m_iObjectHeight, true, TEXT("ShellIdle")))
		return false;



	if (!AddComponent(pRender, pRender->GetComponentTag()))
		return false;

	
	return true;
}

void CKoopa::Init()
{
	CEnemy::Init();
	GetComponent<AnimationRender>().lock()->ChangeAnimationTable(TEXT("KoopaNormal"), TEXT("WalkLeft"));
}

void CKoopa::Update(double dDeltaTime)
{
	CEnemy::Update(dDeltaTime);

}

void CKoopa::Render(const HDC & hDC)
{
	auto pRender = GetComponent<AnimationRender>().lock();
	if (pRender->IsActive())
		pRender->Draw(hDC);

}

void CKoopa::ChangeAnimationClip()
{


}

void CKoopa::ActorBehavior(double dDeltaTime)
{
	auto pPhysics = GetComponent<PhysicsComponent>().lock();
	auto pTransform = GetTransform().lock();
	auto pRender = GetComponent<AnimationRender>().lock();
	if(m_ObjectState != Types::OS_DEAD)
	{
		if (m_ObjectState != Types::OS_DAMAGED)
		{
			float fWalkSpeed = pPhysics->GetSpeed();

			if (m_Direction == Types::DIR_LEFT)
			{
				pPhysics->SetCurSpeed(-1 * fWalkSpeed);
				pRender->ChangeAnimation(TEXT("WalkLeft"));
			}
			else if (m_Direction == Types::DIR_RIGHT)
			{
				pPhysics->SetCurSpeed(fWalkSpeed);
				pRender->ChangeAnimation(TEXT("WalkRight"));
			}

		}
		else
		{
			if (pPhysics->GetCurSpeed() != 0.f)
			{
				if (m_Direction == Types::DIR_LEFT)
				{
					pRender->ChangeAnimation(TEXT("ThrownLeft"));
				}
				else if (m_Direction == Types::DIR_RIGHT)
				{
					pRender->ChangeAnimation(TEXT("ThrownRight"));
				}
			}
		}

	}
	else
	{
		auto pCamera = CCameraManager::GetInstance()->GetMainCamera().lock();
		POSITION position = pTransform->GetScreenPosition();
		if (position.y > pCamera->GetCameraHeight() + m_iObjectHeight)
		{
			m_bActive = false;
			return;
		}
		if (position.x < 0.f - m_iObjectWidth || position.x > pCamera->GetCameraWidth() + m_iObjectWidth)
		{
			m_bActive = false;
			return;
		}

	}

	GetTransform().lock()->Move(pPhysics->GetCurSpeed() * dDeltaTime, pPhysics->GetCurJumpForce() * dDeltaTime);

}
