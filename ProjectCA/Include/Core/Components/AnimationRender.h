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
	virtual bool	PostInit(CObject* pOwner, const Types::tstring& strTag = TEXT("RenderComponent")) override;
	virtual void Init() override;
	virtual void Update(double dDeltaTIme) override;
	virtual void Draw(const HDC& hDC) override;
	

public:
	bool AddAnimation(double dPlayTime, const TSTRING& strMapName, const TSTRING& strSpriteName,
		UINT iWidth, UINT iHeight, bool bLoop, const Types::tstring & strAnimTag, bool bInterrupt = true);


public:
	bool SetAnimationPlaySpeed(double dSpeed);
	void SetAnimationMotion(ANIM_MOTION motion);
	void SetCurAnimationTable(const TSTRING& strTableName);
	bool ChangeAnimation(const TSTRING& strAnimTag);
	bool ChangeAnimation(const TSTRING& strAnimTableName, const TSTRING& strAnimTag);
	


public:
	const TSTRING&	GetAnimTag() const;


private:
	void UpdateAnimationMotion();
	void ChangeAnimationClip(ANIM_MOTION motion);

private:
	bool								m_bTableChange;
	ACTOR_STATE					m_OwnerState;
	VER_STATE						m_OwnerVerticalState;
	HOR_STATE						m_OwnerHorizonalState;
	DIRECTION						m_OwnerDirection;
	ANIM_MOTION				m_AnimationState;
	ANIM_MOTION				m_PreAnimationState;
	CActor*							m_pActor;
	std::weak_ptr<CAnim>		m_pCurAnimation;
	AnimationTable				m_AnimationTable;
	TSTRING							m_strCurTableName;

};