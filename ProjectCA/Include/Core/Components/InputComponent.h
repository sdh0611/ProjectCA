#pragma once

#include "..\..\..\stdafx.h"
#include "ComponentBase.h"

class CActor;

class InputComponent :public ComponentBase {

public:
	InputComponent();
	virtual ~InputComponent() = default;

	
public:
	virtual bool Init(std::shared_ptr<CActor> pOwner, const Types::tstring& strTag = TEXT("InputComponent"));
	virtual void Update(float fDeltaTime) = 0;



};