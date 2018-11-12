#pragma once


#include "..\..\..\stdafx.h"
#include "ComponentBase.h"


class CSprite;
class CAnim;

typedef std::vector<std::weak_ptr<CSprite>> WeakSpritePtrList;
typedef std::unordered_map<Types::tstring, std::shared_ptr<CAnim>> AnimationTable;

class RenderComponent : public ComponentBase {

public:
	RenderComponent();
	virtual ~RenderComponent();


public:
	virtual bool	PostInit(CActor* pOwner, 	const Types::tstring& strTag = TEXT("RenderComponent"));
	virtual void	Init();
	virtual void	Update(double dDeltaTime);
	virtual void	LateUpdate(double dDeltaTime) override;
	void			Draw(const HDC& hDC);


public:
	bool AddAnim(double dPlayTime, const Types::tstring& strSpriteName, UINT iWidth, UINT iHeight, 
		bool bLoop, bool bAnimate = true, const Types::tstring& strAnimTag = TEXT("Default"));


public:
	bool IsVisible() const { return m_bVisible; }
	void SetVisible(bool bVisible) { m_bVisible = bVisible; }
	bool IsChangeAnim() const { return m_bChangeAnim; }
	void SetChangeAnim(bool bChange) { m_bChangeAnim = bChange; }
	bool SetAnimationPlaySpeed(double dSpeed);
	void SetAnimation(Types::AnimationMotion motion);


public:
	void ChangeAnimationCilp(Types::AnimationMotion motion);
	bool ChangeAnimation(const Types::tstring& strAnimTag);

	
private:
	void UpdateAnimationMotion();


private:
	bool											m_bVisible;
	bool											m_bChangeAnim;
	std::weak_ptr<CAnim>					m_pWeakCurAnim;
	AnimationTable							m_animationTable;
	Types::AnimationMotion					m_animationState;
	Types::ActorState							m_ownerState;
	Types::VerticalState						m_ownerVerticalState;
	Types::Direction							m_ownerDirection;

};