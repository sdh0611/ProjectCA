#include "..\..\..\Include\Core\Components\AnimationRender.h"
#include "..\..\..\Include\Core\CResourceManager.h"
#include "..\..\..\Include\Core\Graphic\CSprite.h"
#include "..\..\..\Include\Scene\Actor\CActor.h"

AnimationRender::AnimationRender()
	:RenderComponent(), 
	m_bLoop(false), m_iCurFrame(0), m_iMaxFrame(0),
	m_iDrawWidth(0), m_iDrawHeight(0), m_dPlayTime(0.f), m_dPlaySpeed(0.f),
	m_dPlaySectionLength(0.f), m_dTimeElapsed(0.f), m_AnimationState(Types::AM_IDLE)

{
}

AnimationRender::~AnimationRender()
{
	m_WeakSpriteTable.clear();
}

bool AnimationRender::Init(std::shared_ptr<CActor> pOwner, const Types::tstring & strSpriteName, 
	UINT iWidth, UINT iHeight, double dPlayTime, bool bLoop, const Types::tstring & strAnimTag)
{
	m_pWeakSprite = CResourceManager::GetInstance()->GetWeakSprtiePtr(strSpriteName);
	if (m_pWeakSprite.expired())
		return false;

	m_pOwner					= pOwner;

	m_iMaxFrame				= m_pWeakSprite.lock()->GetBitWidth() / SPRITE_WIDTH;
	m_iDrawWidth				= iWidth;
	m_iDrawHeight				= iHeight;

	m_ColorRef					= RGB(248, 7, 220);

	m_dPlayTime				= dPlayTime;
	m_dPlaySpeed				= 1.f;
	m_dPlaySectionLength	= m_dPlayTime / m_iMaxFrame;

	m_strAnimTag				= strAnimTag;

	m_bLoop						= bLoop;

	return true;
}

void AnimationRender::Update(double dDeltaTIme)
{
	m_dTimeElapsed += dDeltaTIme;
	UpdateAnimationMotion();
	ChangeAnimationCilp(m_animationState);
	m_pWeakCurAnim.lock()->Update(dDeltaTime);


}

void AnimationRender::Draw(const HDC & hDC, const Types::Point & point)
{
}

bool AnimationRender::SetSprite(const Types::tstring & strSpriteName)
{
	return false;
}

bool AnimationRender::SetTotalPlayTime(double dTime)
{
	return false;
}

bool AnimationRender::SetPlaySpeed(double dSpeed)
{
	return false;
}

void AnimationRender::SetDrawingWidth(UINT iWidth)
{
}

void AnimationRender::SetDrawingHeight(UINT iHeight)
{
}

UINT AnimationRender::GetDrawWidth() const
{
	return 0;
}

UINT AnimationRender::GetDrawHeight() const
{
	return 0;
}

void AnimationRender::ClearEleapsedTime()
{
}

const Types::tstring AnimationRender::GetAnimTag() const
{
	return Types::tstring();
}

void AnimationRender::DrawAnimation(const HDC & hDC, const Types::Point & point)
{
}

void AnimationRender::DrawImage(const HDC & hDC, const Types::Point & point)
{
}
