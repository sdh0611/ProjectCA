#pragma once

#include "..\..\..\stdafx.h"
#include "CActor.h"


class CCamera;

class CPlayer : public CActor {

	enum PlayerState {
		PS_SMALL, PS_BIG, PS_FLOWER
	};

public:
	CPlayer();
	virtual ~CPlayer();


public:
	//virtual bool Init(const Types::ActorData&) override;
	virtual bool PostInit(const Types::ActorData&, CGameScene*) override;
	virtual void Init() override;
	virtual void Update(double fDeltaTime) override;
	virtual void Render(const HDC& hDC) override;
	virtual void ActorBehavior(double dDeltaTime) override;


public:
	bool AttachCamera(std::shared_ptr<CCamera> pCamera);


public:
	std::weak_ptr<CCamera> GetCamera();


private:
	std::shared_ptr<CCamera>	m_pCamera;


private:
	PlayerState		m_PlayerState;


};