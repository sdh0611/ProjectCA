#pragma once

#include "..\..\..\stdafx.h"
#include "RenderComponent.h"

class CActor;
class CAnim;

typedef std::unordered_map<TSTRING, std::shared_ptr<CAnim>>	AnimationMap;
typedef std::unordered_map<TSTRING, AnimationMap>				AnimationTable;

class AnimationRender : public RenderComponent {

public:
	AnimationRender();
	virtual ~AnimationRender();


public:
	virtual bool	PostInit(CActor* pOwner, const Types::tstring& strTag = TEXT("RenderComponent")) override;
	virtual void Init() override;
	virtual void Update(double dDeltaTIme) override;
	virtual void Draw(const HDC& hDC) override;
	

public:
	bool AddAnimation(double dPlayTime, const TSTRING& strMapName ,const TSTRING& strSpriteName,
		UINT iWidth, UINT iHeight, bool bLoop, const Types::tstring & strAnimTag);


public:
	bool SetAnimationPlaySpeed(double dSpeed);
	void SetAnimationMotion(Types::AnimationMotion motion);


public:
	const TSTRING	GetAnimTag() const;


private:
	void UpdateAnimationMotion();
	bool ChangeAnimation(const TSTRING& strAnimTag);
	void ChangeAnimationClip(Types::AnimationMotion motion);


private:
	TSTRING							m_strCurTableName;
	std::weak_ptr<CAnim>		m_pCurAnimation;
	AnimationTable				m_AnimationTable;
	Types::ActorState				m_OwnerState;
	Types::VerticalState			m_OwnerVerticalState;
	Types::Direction				m_OwnerDirection;
	Types::AnimationMotion		m_AnimationState;

};