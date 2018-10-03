#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Core\Components\RenderComponent.h"
#include	"..\..\..\Include\Core\CResourceManager.h"
#include "..\..\..\Include\Core\Graphic\CSprite.h"
#include "..\..\..\Include\Core\Graphic\CAnim.h"
#include "..\..\..\Include\Scene\Actor\CActor.h"


RenderComponent::RenderComponent()
	:m_iFrame(0), m_dTimeElapsed(0.0f), m_bVisible(true)
{
}

RenderComponent::~RenderComponent()
{
		
}

bool RenderComponent::Init(CActor * pOwner, const Types::tstring & strTag)
{
	//m_pWeakSprite = CResourceManager::GetInstance()->GetWeakSprtiePtr(strSpriteName);
	//if (m_pWeakSprite.lock() == nullptr)
	//	return false;

	std::shared_ptr<CActor> pActor = std::shared_ptr<CActor>(pOwner);
	m_pOwner = pActor;

	m_strComponentTag = strTag;
	//m_ownerState = m_pOwner->GetActorState();

	//if (!AddSprite(Types::OS_IDLE, TEXT("PlayerIdle")))
	//	return false;

	//if (!AddSprite(Types::OS_MOVE, TEXT("PlayerMove")))
	//	return false;


	//For Test Code
	//제대로 작동하면 Actor단에서 추가하도록 변경해야함.
	//if (!AddAnim(0.2f, Types::OS_IDLE, TEXT("PlayerIdle"), TEXT("Idle")))
	//	return false;

	//if (!AddAnim(0.2f, Types::OS_MOVE, TEXT("PlayerMove"), TEXT("Move")))
	//	return false;

	return true;
}

void RenderComponent::Update(double fDeltaTime)
{
	//if (m_pWeakSprite.expired())
	//	m_pWeakSprite.reset();

	//if (m_ownerState != m_pOwner->GetActorState())
	//	m_ownerState = m_pOwner->GetActorState();

	for (auto& it : m_animationTable) {
		it.second->Update(fDeltaTime);
	}

	m_dTimeElapsed += fDeltaTime;

}

//DC핸들값은 BackBuffer의 DC를 받아와야 한다.
void RenderComponent::Draw(const HDC & hDC)
{
	if (m_bVisible) {
		//m_animTable[m_ownerState]->Draw(hDC, hMemDC);
		m_pWeakCurAnim.lock()->Draw(hDC);

	}		

}

//bool RenderComponent::AddSprite(Types::ObjectState state, const Types::tstring & strSpriteName)
//{
//	m_pWeakSprite = CResourceManager::GetInstance()->GetWeakSprtiePtr(strSpriteName);
//
//	//해당 Sprite가 없는 경우
//	if(m_pWeakSprite.expired())
//		return false;
//	
//	if (m_spriteTable.find(state) != m_spriteTable.end()) { //테이블 안에 해당 Key값과 연결된 vector가 있는 경우
//		for (auto& it : m_spriteTable[state]) {		//vector에 이미 존재하는 Sprite면 false return
//			if (it.lock() == m_pWeakSprite.lock())
//				return false;
//		}
//		m_spriteTable[state].emplace_back(m_pWeakSprite);		//vector에 없으면 해당 Sprite 추가
//	}
//	else {	//테이블 안에 해당 Key값과 연결된 vector가 없는 경우
//		//해당 Key값과 연결된 Vector를 생성한 후, Sprite 추가
//		m_spriteTable.insert(std::make_pair(state, WeakSpritePtrList()));
//		m_spriteTable[state].emplace_back(m_pWeakSprite);
//	}	
//	
//	return true;
//}

//bool RenderComponent::AddAnim(double dPlayTime, Types::ObjectState state, const Types::tstring & strSpriteName, 
//	const Types::tstring& strAnimTag)
//{
//	std::shared_ptr<CAnim> pAnim = std::make_shared<CAnim>();
//
//	if (!pAnim->Init(m_pOwner, strSpriteName, dPlayTime, strAnimTag))
//		return false;
//
//	if (m_animTable.find(state) != m_animTable.end()) { //테이블 안에 해당 Key값과 연결된 vector가 있는 경우
//		m_animTable[state] = pAnim;
//	}
//	else {	//테이블 안에 해당 Key값과 연결된 vector가 없는 경우
//			//해당 Key값과 연결된 Vector를 생성한 후, Anim 추가
//		m_animTable.insert(std::make_pair(state, pAnim));
//	}
//
//	return true;
//}


bool RenderComponent::AddAnim(double dPlayTime, const Types::tstring & strSpriteName
	, bool bAnimate, const Types::tstring & strAnimTag)
{
	std::shared_ptr<CAnim> pAnim = std::make_shared<CAnim>();

	if (!pAnim->Init(m_pOwner, strSpriteName, dPlayTime, bAnimate, strAnimTag))
		return false;

	if (m_pWeakCurAnim.expired())
		m_pWeakCurAnim = pAnim;

	if (m_animationTable.find(strAnimTag) != m_animationTable.end()) { //테이블 안에 해당 Key값과 연결된 vector가 있는 경우
		m_animationTable[strAnimTag] = pAnim;
	}
	else {	//테이블 안에 해당 Key값과 연결된 vector가 없는 경우
			//해당 Key값과 연결된 Vector를 생성한 후, Anim 추가
		m_animationTable.insert(std::make_pair(strAnimTag, pAnim));
	}

	return true;
}

bool RenderComponent::ChangeAnimation(const Types::tstring & strAnimTag)
{
	auto it = m_animationTable.find(strAnimTag);
	if ( it == m_animationTable.end())
		return false;

	if ((*it).second == nullptr)
		return false;

	if (!m_pWeakCurAnim.expired()) {
		if (m_pWeakCurAnim.lock()->GetAnimTag() == strAnimTag)
			return true;

		m_pWeakCurAnim.lock()->ClearEleapsedTime();
	}
	m_pWeakCurAnim = (*it).second;

	return true;
}
