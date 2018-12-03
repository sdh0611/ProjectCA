#include "..\..\..\Include\Core\Components\AnimationRender.h"
#include "..\..\..\Include\Core\Components\PhysicsComponent.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"
#include "..\..\..\Include\Core\Components\Collider.h"
#include "..\..\..\Include\Core\CResourceManager.h"
#include "..\..\..\Include\Core\Graphic\CSprite.h"
#include "..\..\..\Include\Scene\Actor\CActor.h"
#include "..\..\..\Include\Core\Graphic\CAnim.h"



AnimationRender::AnimationRender()
	:RenderComponent()
{
}

AnimationRender::~AnimationRender()
{
	m_AnimationTable.clear();
}


bool AnimationRender::PostInit(CObject * pOwner, const TSTRING & strTag)
{
	if (!RenderComponent::PostInit(pOwner, strTag))
		return false;

	m_pActor = static_cast<CActor*>(pOwner);

	return true;
}

void AnimationRender::Init()
{
	RenderComponent::Init();
}

void AnimationRender::Update(double dDeltaTime)
{
	if (m_bActive)
	{
		m_pCurAnimation.lock()->Update(dDeltaTime);
	}

}

void AnimationRender::Draw(const HDC & hDC)
{
	if (m_bActive)
	{

		//POSITION pivot = m_pOwner->GetComponent<TransformComponent>().lock()->GetScreenPivot();
		if (!m_pCurAnimation.expired())
			m_pCurAnimation.lock()->Draw(hDC, m_hRenderDC, m_DrawPivot, m_iDrawWidth, m_iDrawHeight);

		//POSITION position = m_pOwner->GetComponent<TransformComponent>().lock()->GetScreenPosition();
		//std::shared_ptr<Collider> pCollider = STATIC_POINTER_CAST(Collider, m_pOwner->GetComponent(TEXT("Collider")).lock());
		//std::shared_ptr<PhysicsComponent> pPhysics = STATIC_POINTER_CAST(PhysicsComponent, m_pOwner->GetComponent(TEXT("PhysicsComponent")).lock());

		//if (pCollider)
		//{
		//	pCollider->DrawCollider(hDC, position);

		//	if (pCollider->IsCollision())
		//	{
		//		TextOut(hDC, position.x, position.y, TEXT("TRUE"), sizeof(TEXT("TRUE")));
		//	}
		//	else
		//	{
		//		TextOut(hDC, position.x, position.y, TEXT("FALSE"), sizeof(TEXT("FALSE")));
		//	}

		//}

		//if (pPhysics)
		//{
		//	if (pPhysics->IsGrounded())
		//	{
		//		TextOut(hDC, position.x, position.y + 20, TEXT("TRUE"), sizeof(TEXT("TRUE")));
		//	}
		//	else
		//	{
		//		TextOut(hDC, position.x, position.y + 20, TEXT("FALSE"), sizeof(TEXT("FALSE")));
		//	}

		//}



	}
}

bool AnimationRender::AddAnimation(double dPlayTime, const TSTRING& strMapName, const TSTRING& strSpriteName, 
	bool bLoop, const TSTRING & strAnimTag, bool bInterrupt)
{
	auto pAnim = std::make_shared<CAnim>();

	if (!pAnim->Init(strSpriteName, dPlayTime, bLoop, strAnimTag, bInterrupt))
		return false;

	if (m_pCurAnimation.expired())
	{
		m_pCurAnimation = pAnim;
		m_strCurTableName = strMapName;
	}

	auto tableIter = m_AnimationTable.find(strMapName);
	if (tableIter == m_AnimationTable.end())
	{
		AnimationMap map;
		map.insert(std::make_pair(strAnimTag, pAnim));
		m_AnimationTable.insert(std::make_pair(strMapName, map));
	}
	else
	{
		//테이블 안에 해당 Key값과 연결된 vector가 있는 경우
		if (tableIter->second.find(strAnimTag) != tableIter->second.end())
		{
			tableIter->second.at(strAnimTag) = pAnim;
		}
		else
		{
			//테이블 안에 해당 Key값과 연결된 vector가 없는 경우
			//해당 Key값과 연결된 Vector를 생성한 후, Anim 추가
			tableIter->second.insert(std::make_pair(strAnimTag, pAnim));
		}

	}

	return true;

}

void AnimationRender::SetPauseAnimation(bool bPause)
{
	m_pCurAnimation.lock()->SetPauseAnimation(bPause);
}

bool AnimationRender::SetAnimationPlaySpeed(double dSpeed)
{
	return m_pCurAnimation.lock()->SetPlaySpeed(dSpeed);
}

bool AnimationRender::ChangeAnimation(const TSTRING & strAnimTag)
{
	if (!m_pCurAnimation.lock()->IsCanInterrupt())
	{
		if (!m_pCurAnimation.lock()->IsReadyToChange())
		{
			return false;
		}
	}

	auto animIter = m_AnimationTable.at(m_strCurTableName).find(strAnimTag);
	if (animIter == m_AnimationTable.at(m_strCurTableName).end())
	{
		return false;
	}
	//같은 animation인지 판별
	if (!m_pCurAnimation.expired())
	{
		if (m_pCurAnimation.lock()->GetAnimTag() == strAnimTag)
			return true;

		m_pCurAnimation.lock()->ClearEleapsedTime();
	}

	m_pCurAnimation = animIter->second;

	return true;
}

void AnimationRender::ChangeAnimationTable(const TSTRING & strTableName)
{
	auto tableIt = m_AnimationTable.find(strTableName);
	if (tableIt == m_AnimationTable.cend())
	{
		std::cout << strTableName.c_str() << "is not existing key in AnimationTable.";
		return;
	}

	auto animIt = tableIt->second.find(m_pCurAnimation.lock()->GetAnimTag());
	if (animIt == tableIt->second.cend())
	{
		std::cout << m_pCurAnimation.lock()->GetAnimTag().c_str() << "is not existing key in AnimationMap.";
		return;
	}

	m_strCurTableName = strTableName;
	m_pCurAnimation = animIt->second;
}

bool AnimationRender::ChangeAnimationTable(const TSTRING & strAnimTableName, const TSTRING & strAnimTag)
{
	auto animIter = m_AnimationTable.at(strAnimTableName).find(strAnimTag);
	if (animIter == m_AnimationTable.at(strAnimTableName).end())
	{
		return false;
	}
	//같은 animation인지 판별
	if (!m_pCurAnimation.expired())
	{
		if (m_pCurAnimation.lock()->GetAnimTag() == strAnimTag)
			return true;

		m_pCurAnimation.lock()->ClearEleapsedTime();
	}
	m_strCurTableName = strAnimTableName;
	m_pCurAnimation = animIter->second;

	return true;
}

const TSTRING& AnimationRender::GetAnimTag() const
{
	return m_pCurAnimation.lock()->GetAnimTag();
}




