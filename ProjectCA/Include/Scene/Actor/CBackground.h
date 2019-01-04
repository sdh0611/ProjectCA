#pragma once

#include "..\..\..\stdafx.h"
#include "..\CEntity.h"

//NOTE: 배경을 그리는 기능들을 구현한 Class.
//			MainCamera의 이동속도에 맞춰 스크롤함.

class CSprite;
class CScene;

class CBackground : public CEntity {

public:
	CBackground();
	virtual ~CBackground();


public:
	virtual bool PostInit(const ENTITY_DATA& data, CScene* pScene);
	virtual void Update(double dDeltaTIme) override;
	virtual void Render(const HDC& hDC) override;


public:
	bool SetBackgroundImage(const TSTRING& strImageName);
	void SetStatic(bool bStatic);
	void SetScrollSpeed(float fSpeed);

public:
	bool IsStatic();


private:
	bool							m_bStatic;						//움직이는 배경인가 멈춰있는 배경인가
	float							m_fScrollSpeed;
	UINT							m_iBackgroundWidth;
	UINT							m_iBackgroundHeight;


};