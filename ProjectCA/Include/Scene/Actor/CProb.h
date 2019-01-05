#pragma once

#include "..\..\..\stdafx.h"
#include "..\CObject.h"

//NOTE: 게임 내 Ground, Block 등 물체들이 상속받게 될 Class

class CProb : public CObject {

public:
	CProb();
	virtual ~CProb();


public:
	virtual bool PostInit(const OBJECT_DATA&, CScene*) override;

};