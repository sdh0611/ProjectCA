#pragma once

#include "..\..\..\stdafx.h"
#include "ComponentBase.h"

class TransformComponent : public ComponentBase {


public:
	TransformComponent();
	virtual ~TransformComponent();


public:
	bool			PostInit(CObject* pOwner, const POSITION& position, const TSTRING& strTag = TEXT("TransformComponent"));
	virtual void	Init() override;
	virtual void	Update(double dDeltaTime) override;
	virtual void	LateUpdate(double dDeltaTime) override;


public:
	void Move(float fx, float fy);
	void SetPosition(const POSITION& position);
	void SetPosition(float fx, float fy);
	void SetPositionX(float fx);
	void SetPositionY(float fy);
	void SetSpawnPosition(const POSITION& position);
	void SetSpawnPosition(float fx, float fy);
	void SetPivotRatio(float fWidthRatio, float fHeightRatio);
	void SetWidthPivotRatio(float fRatio);
	void SetHeightPivotRatio(float fRatio);
	void SetScreenPosition(const POSITION& position);


public:
	float			GetPivotWidthRatio() const;
	float			GetPivotHeightRatio() const;
	POSITION	GetPosition() const;
	POSITION	GetSpawnPosition() const;
	POSITION	GetPivot() const;
	POSITION	GetLastPosition() const;
	POSITION	GetScreenPosition() const;
	POSITION	GetScreenPivot() const;


public:
	//카메라상의 좌표 조정
	void AdjustScreenPosition();


private:
	float			m_fPivotWidthRatio;
	float			m_fPivotHeightRatio;
	POSITION	m_Position;
	POSITION	m_SpawnPosition;
	POSITION	m_LastPosition;
	POSITION	m_Pivot;
	POSITION	m_ScreenPosition;
	POSITION	m_ScreenPivot;

};