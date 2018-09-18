#pragma once

#include "..\..\..\stdafx.h"
#include "CActor.h"


class CProb : public CActor {

public:
	CProb();
	virtual ~CProb();


public:
	virtual bool Init(const Types::ActorData&) override;
	virtual void Update(float fDeltaTime) override;
	virtual void Render(const HDC& hDC) override;


private:




private:


};