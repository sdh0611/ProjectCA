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
	virtual bool	PostInit(CEntity* pOwner, const TSTRING& strTag = TEXT("RenderComponent")) override;
	virtual void	Init() override;
	virtual void	Update(double dDeltaTIme) override;
	virtual void	Draw(const HDC& hDC) override;
	void			Draw(const HDC& hDC, POSITION position);


public:
	bool AddAnimation(double dPlayTime, const TSTRING& strMapName, const TSTRING& strSpriteName,
		bool bLoop, const TSTRING & strAnimTag, bool bInterrupt = true);


public:
	void SetPauseAnimation(bool bPause);
	bool SetAnimationPlaySpeed(double dSpeed);
	bool ChangeAnimation(const TSTRING& strAnimTag);
	void ChangeAnimationTable(const TSTRING& strTableName);
	bool ChangeAnimationTable(const TSTRING& strAnimTableName, const TSTRING& strAnimTag);
	

public:
	bool					IsCurAnimationEnd() const;
	const TSTRING&	GetAnimTag() const;


private:
	bool								m_bTableChange;
	CActor*							m_pActor;
	std::weak_ptr<CAnim>		m_pCurAnimation;
	AnimationTable				m_AnimationTable;
	TSTRING							m_strCurTableName;

};