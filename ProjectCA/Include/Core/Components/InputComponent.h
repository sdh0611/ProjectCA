#pragma once

#include "..\..\..\stdafx.h"
#include "ComponentBase.h"

class CActor;

class InputComponent :public ComponentBase {

public:
	InputComponent(CActor* pOwner);
	virtual ~InputComponent() = default;

	
public:
	virtual bool Init(CActor* pOwner, const Types::tstring& strTag = TEXT("InputComponent")) override;
	virtual void Update(float fDeltaTime) override;


private:
	void KeyProcess();


};