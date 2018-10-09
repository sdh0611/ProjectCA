#pragma once


#include "..\..\..\stdafx.h"
#include "ComponentBase.h"


class CSprite;
class CAnim;

typedef std::vector<std::weak_ptr<CSprite>> WeakSpritePtrList;
typedef std::unordered_map<Types::ObjectState, WeakSpritePtrList> SpriteTable;
typedef std::unordered_map<Types::ObjectState, std::shared_ptr<CAnim>> AnimTable;
typedef std::unordered_map<Types::tstring, std::shared_ptr<CAnim>> AnimationTable;

class RenderComponent : public ComponentBase {

public:
	RenderComponent();
	virtual ~RenderComponent();


public:
	virtual bool Init(CActor* pOwner, 	const Types::tstring& strTag = TEXT("RenderComponent"));
	virtual void Update(double dDeltaTime);
	void Draw(const HDC& hDC);
	//bool AddSprite(Types::ObjectState state, const Types::tstring& strSpriteName);
	//bool AddAnim(double dPlayTime, Types::ObjectState state,
	//	const Types::tstring& strSpriteName, const Types::tstring& strAnimTag);
	bool AddAnim(double dPlayTime, const Types::tstring& strSpriteName,
		bool bAnimate = true, const Types::tstring& strAnimTag = TEXT("Default"));


public:
	bool IsVisible() const { return m_bVisible; }
	void SetVisible(bool bVisible) { m_bVisible = bVisible; }


private:
	void ChangeAnimationCilp();
	bool ChangeAnimation(const Types::tstring& strAnimTag);


private:
	//AnimTable									m_animTable;
	std::weak_ptr<CAnim>					m_pWeakCurAnim;
	AnimationTable							m_animationTable;
	Types::tstring								m_strPreAnimClipName;
	bool											m_bVisible;
	Types::ObjectState							m_ownerState;
	Types::JumpState							m_ownerJumpState;
	Types::Direction							m_ownerDirection;


};