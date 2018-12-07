#pragma once

#include "..\..\..\stdafx.h"
#include "..\..\Singleton.hpp"

class CAnim;

class CEffectManager : public Singleton<CEffectManager>
{
	DECLARE_SINGLETON(CEffectManager)

public:
	bool Init();
	void Update(double dDeltaTime);


public:



private:
	


};