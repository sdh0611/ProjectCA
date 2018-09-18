#pragma once

#include "..\..\..\stdafx.h"
#include "CActor.h"


class CPlayer : public CActor {

public:
	CPlayer();
	virtual ~CPlayer();


public:
	virtual bool Init(const Types::ActorData&) override;
	virtual void Update(float fDeltaTime) override;
	virtual void Render(const HDC& hDC) override;


private:



private:



};