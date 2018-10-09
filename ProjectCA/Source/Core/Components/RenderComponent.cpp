#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Core\Components\RenderComponent.h"
#include	"..\..\..\Include\Core\CResourceManager.h"
#include "..\..\..\Include\Core\Graphic\CSprite.h"
#include "..\..\..\Include\Core\Graphic\CAnim.h"
#include "..\..\..\Include\Scene\Actor\CActor.h"

RenderComponent::RenderComponent()
	:m_bVisible(true)
{
}

RenderComponent::~RenderComponent()
{
		
}

bool RenderComponent::Init(CActor * pOwner, const Types::tstring & strTag)
{
	std::shared_ptr<CActor> pActor = std::shared_ptr<CActor>(pOwner);
	m_pOwner = pActor;

	m_strComponentTag = strTag;
	
	m_ownerState = m_pOwner->GetActorState();
	m_ownerJumpState = m_pOwner->GetActorJumpState();
	m_ownerDirection = m_pOwner->GetActorDirection();

	return true;
}

void RenderComponent::Update(double dDeltaTime)
{
	if (m_ownerState != m_pOwner->GetActorState()
		|| m_ownerDirection != m_pOwner->GetActorDirection()
		|| m_ownerJumpState != m_pOwner->GetActorJumpState()) 
	{
		m_ownerState = m_pOwner->GetActorState();
		m_ownerDirection = m_pOwner->GetActorDirection();
		m_ownerJumpState = m_pOwner->GetActorJumpState();
		ChangeAnimationCilp();
	}

	m_pWeakCurAnim.lock()->Update(dDeltaTime);
	//for (auto& it : m_animationTable) {
	//	it.second->Update(fDeltaTime);
	//}

	//m_dTimeElapsed += fDeltaTime;
}

//DC핸들값은 BackBuffer의 DC를 받아와야 한다.
void RenderComponent::Draw(const HDC & hDC)
{
	if (m_bVisible) 
	{
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

void RenderComponent::ChangeAnimationCilp()
{

	//switch (m_ownerState) {
	//case Types::OS_IDLE:
	//	if (m_ownerDirection == Types::DIR_LEFT)
	//		if(m_pOwner->GetActorJumpState() == Types::JS_JUMP)
	//			ChangeAnimation(TEXT("JumpLeft"));
	//		else if(m_pOwner->GetActorJumpState() == Types::JS_FALL)
	//			ChangeAnimation(TEXT("FalldownLeft"));
	//		else
	//			ChangeAnimation(TEXT("IdleLeft"));
	//	else if (m_ownerDirection == Types::DIR_RIGHT)
	//		if (m_pOwner->GetActorJumpState() == Types::JS_JUMP)
	//			ChangeAnimation(TEXT("JumpRight"));
	//		else if (m_pOwner->GetActorJumpState() == Types::JS_FALL)
	//			ChangeAnimation(TEXT("FalldownRight"));
	//		else
	//			ChangeAnimation(TEXT("IdleRight"));
	//	break;
	//case Types::OS_WALK:
	//	if (m_ownerDirection == Types::DIR_LEFT)
	//		if (m_pOwner->GetActorJumpState() == Types::JS_JUMP)
	//			ChangeAnimation(TEXT("JumpLeft"));
	//		else if (m_pOwner->GetActorJumpState() == Types::JS_FALL)
	//			ChangeAnimation(TEXT("FalldownLeft"));
	//		else
	//			ChangeAnimation(TEXT("WalkLeft"));
	//	else if (m_ownerDirection == Types::DIR_RIGHT)
	//		if (m_pOwner->GetActorJumpState() == Types::JS_JUMP) {
	//			ChangeAnimation(TEXT("JumpRight"));
	//			puts("JUMPLEFT");
	//		}
	//		else if (m_pOwner->GetActorJumpState() == Types::JS_FALL)
	//			ChangeAnimation(TEXT("FalldownRight"));
	//		else
	//			ChangeAnimation(TEXT("WalkRight"));
	//	break;
	//case Types::OS_RUN:
	//	if (m_ownerDirection == Types::DIR_LEFT)
	//		if (m_pOwner->GetActorJumpState() != Types::JS_IDLE)
	//			ChangeAnimation(TEXT("RunJumpLeft"));
	//		else
	//			ChangeAnimation(TEXT("RunLeft"));
	//	else if (m_ownerDirection == Types::DIR_RIGHT)
	//		if (m_pOwner->GetActorJumpState() != Types::JS_IDLE)
	//			ChangeAnimation(TEXT("RunJumpRight"));
	//		else
	//			ChangeAnimation(TEXT("RunRight"));
	//	break;
	//case Types::OS_LOOKUP:
	//	if (m_ownerDirection == Types::DIR_LEFT)
	//		ChangeAnimation(TEXT("LookupLeft"));
	//	else if (m_ownerDirection == Types::DIR_RIGHT)
	//		ChangeAnimation(TEXT("LookupRight"));
	//	break;
	//case Types::OS_SITDOWN:
	//	if (m_ownerDirection == Types::DIR_LEFT)
	//		ChangeAnimation(TEXT("SitdownLeft"));
	//	else if (m_ownerDirection == Types::DIR_RIGHT)
	//		ChangeAnimation(TEXT("SitdownRight"));
	//	break;
	//case Types::OS_ATTACK:
	//	if (m_ownerDirection == Types::DIR_LEFT)
	//		ChangeAnimation(TEXT("AttackLeft"));
	//	else if (m_ownerDirection == Types::DIR_RIGHT)
	//		ChangeAnimation(TEXT("AttackRight"));
	//	break;
	//case Types::OS_DAMAGED:
	//	if (m_ownerDirection == Types::DIR_LEFT)
	//		ChangeAnimation(TEXT("DamagedLeft"));
	//	else if (m_ownerDirection == Types::DIR_RIGHT)
	//		ChangeAnimation(TEXT("DamagedRight"));
	//	break;

	//}

	//if (m_ownerState == Types::OS_RUN)
	//	puts("RUN");

	if (m_ownerJumpState == Types::JS_JUMP) {
		switch (m_ownerState) {
		case Types::OS_IDLE:
		case Types::OS_WALK:
			if (m_ownerDirection == Types::DIR_LEFT)
			{
				if (m_pOwner->GetActorPreState() == Types::OS_RUN)
				{
					ChangeAnimation(TEXT("RunJumpLeft"));
					break;
				}
				ChangeAnimation(TEXT("JumpLeft"));
			}
			else if (m_ownerDirection == Types::DIR_RIGHT)
			{
				if (m_pOwner->GetActorPreState() == Types::OS_RUN)
				{
					ChangeAnimation(TEXT("RunJumpRight"));
					break;
				}
				ChangeAnimation(TEXT("JumpRight"));
			}
			break;
		case Types::OS_RUN:
			if (m_ownerDirection == Types::DIR_LEFT)
			{
				if (m_pOwner->GetActorPreState() == Types::OS_WALK)
				{
					ChangeAnimation(TEXT("JumpLeft"));
					break;
				}
				ChangeAnimation(TEXT("RunJumpLeft"));
			}
			else if (m_ownerDirection == Types::DIR_RIGHT)
			{
				if (m_pOwner->GetActorPreState() == Types::OS_WALK)
				{
					ChangeAnimation(TEXT("JumpRight"));
					break;
				}
				ChangeAnimation(TEXT("RunJumpRight"));
			}
			break;
		case Types::OS_ATTACK:
			if (m_ownerDirection == Types::DIR_LEFT)
				ChangeAnimation(TEXT("AttackLeft"));
			else if (m_ownerDirection == Types::DIR_RIGHT)
				ChangeAnimation(TEXT("AttackRight"));
			break;
		case Types::OS_DAMAGED:
			if (m_ownerDirection == Types::DIR_LEFT)
				ChangeAnimation(TEXT("DamagedLeft"));
			else if (m_ownerDirection == Types::DIR_RIGHT)
				ChangeAnimation(TEXT("DamagedRight"));
			break;
		}

	}
	else if (m_ownerJumpState == Types::JS_FALL) {
		switch (m_ownerState) {
		case Types::OS_IDLE:
		case Types::OS_WALK:
			if (m_ownerDirection == Types::DIR_LEFT)
			{
				if (m_pOwner->GetActorPreState() == Types::OS_RUN)
				{
					ChangeAnimation(TEXT("RunJumpLeft"));
					break;
				}
				ChangeAnimation(TEXT("FalldownLeft"));
			}
			else if (m_ownerDirection == Types::DIR_RIGHT) {
				if (m_pOwner->GetActorPreState() == Types::OS_RUN)
				{	
					ChangeAnimation(TEXT("RunJumpRight"));
					break;
				}
				ChangeAnimation(TEXT("FalldownRight"));
			}
			break;
		case Types::OS_RUN:
			if (m_ownerDirection == Types::DIR_LEFT) {
				if (m_pOwner->GetActorPreState() == Types::OS_WALK)
				{
					ChangeAnimation(TEXT("FalldownLeft"));
					break;
				}
				ChangeAnimation(TEXT("RunJumpLeft"));
			}
			else if (m_ownerDirection == Types::DIR_RIGHT) {
				if (m_pOwner->GetActorPreState() == Types::OS_WALK)
				{
					ChangeAnimation(TEXT("FalldownRight"));
					break;
				}
				ChangeAnimation(TEXT("RunJumpRight"));
			}
			break;
		case Types::OS_ATTACK:
			if (m_ownerDirection == Types::DIR_LEFT)
				ChangeAnimation(TEXT("AttackLeft"));
			else if (m_ownerDirection == Types::DIR_RIGHT)
				ChangeAnimation(TEXT("AttackRight"));
			break;
		case Types::OS_DAMAGED:
			if (m_ownerDirection == Types::DIR_LEFT)
				ChangeAnimation(TEXT("DamagedLeft"));
			else if (m_ownerDirection == Types::DIR_RIGHT)
				ChangeAnimation(TEXT("DamagedRight"));
			break;
		}
	}
	else {
		switch (m_ownerState) {
		case Types::OS_IDLE:
			if (m_ownerDirection == Types::DIR_LEFT)
				ChangeAnimation(TEXT("IdleLeft"));
			else if(m_ownerDirection == Types::DIR_RIGHT)
				ChangeAnimation(TEXT("IdleRight"));
			break;
		case Types::OS_WALK:
			if (m_ownerDirection == Types::DIR_LEFT)
				ChangeAnimation(TEXT("WalkLeft"));
			else if (m_ownerDirection == Types::DIR_RIGHT)
				ChangeAnimation(TEXT("WalkRight"));
			break;
		case Types::OS_RUN:
			if (m_ownerDirection == Types::DIR_LEFT)
				ChangeAnimation(TEXT("RunLeft"));
			else if (m_ownerDirection == Types::DIR_RIGHT)
				ChangeAnimation(TEXT("RunRight"));
			break;
		case Types::OS_LOOKUP:
			if (m_ownerDirection == Types::DIR_LEFT)
				ChangeAnimation(TEXT("LookupLeft"));
			else if (m_ownerDirection == Types::DIR_RIGHT)
				ChangeAnimation(TEXT("LookupRight"));
			break;
		case Types::OS_SITDOWN:
			if (m_ownerDirection == Types::DIR_LEFT)
				ChangeAnimation(TEXT("SitdownLeft"));
			else if (m_ownerDirection == Types::DIR_RIGHT)
				ChangeAnimation(TEXT("SitdownRight"));
			break;
		case Types::OS_ATTACK:
			if (m_ownerDirection == Types::DIR_LEFT)
				ChangeAnimation(TEXT("AttackLeft"));
			else if (m_ownerDirection == Types::DIR_RIGHT)
				ChangeAnimation(TEXT("AttackRight"));
			break;
		case Types::OS_DAMAGED:
			if (m_ownerDirection == Types::DIR_LEFT)
				ChangeAnimation(TEXT("DamagedLeft"));
			else if (m_ownerDirection == Types::DIR_RIGHT)
				ChangeAnimation(TEXT("DamagedRight"));
			break;

		}

	}



}
