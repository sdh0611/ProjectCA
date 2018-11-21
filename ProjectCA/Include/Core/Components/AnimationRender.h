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
	bool ChangeAnimation(const TSTRING& strAnimTag);
	void ChangeAnimationTable(const TSTRING& strTableName);
	bool ChangeAnimationTable(const TSTRING& strAnimTableName, const TSTRING& strAnimTag);
	

public:
	const TSTRING&	GetAnimTag() const;


private:
	bool								m_bTableChange;
	CActor*							m_pActor;
	std::weak_ptr<CAnim>		m_pCurAnimation;
	AnimationTable				m_AnimationTable;
	TSTRING							m_strCurTableName;

};