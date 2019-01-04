#pragma once

#include "..\..\..\stdafx.h"
#include "CProb.h"

//NOTE: RandomBox들의 추상 클래스.

class CPickup;

class CRandomBlock : public CProb
{
	enum BlockType {
		BT_DEFAULT, BT_HIDE
	};

public:
	CRandomBlock();
	virtual ~CRandomBlock();


public:
	virtual bool PostInit(const OBJECT_DATA& objectData, CScene* pScene);
	virtual void Init();
	virtual void Update(double dDeltaTime) = 0;
	virtual void Render(const HDC& hDC);
	virtual void LateUpdate();


public:
	void SetHide();


public:
	bool IsHiding() const;


protected:
	bool							m_bHiding;
	BlockType					m_Type;


};