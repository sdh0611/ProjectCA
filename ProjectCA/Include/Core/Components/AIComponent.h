#pragma once

/*
	NOTE:
		Enemy의 행동 정의에 쓰일 AI컴포넌트.
		InputComponent의 인터페이스를 상속받아서 작성함.
*/

#include "..\..\..\stdafx.h"
#include "InputComponent.h"

using Callback = std::function<void(CActor* pActor)>;

class AIComponent : public InputComponent {

public:
	AIComponent();
	virtual ~AIComponent();


public:
	virtual void Init();
	virtual void Update(double fDeltaTime) override;
	virtual void LateUpdate(double dDeltaTime) override;

public:
	Callback SetDelegate(Callback pDelegater);


private:
	void BehaviorUpdate();


private:
	Callback m_pDelegate;

};