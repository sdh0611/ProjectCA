#pragma once

#include "..\..\..\stdafx.h"
#include "CProb.h"

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
	//void SetStoredPickup(std::shared_ptr<CPickup> pPickup);
	void SetHide();


public:
	bool IsHiding() const;


protected:
	//void SetDead();
	//virtual void HandlingEvent(EVENT_TYPE type) override;


protected:
	bool							m_bHiding;
	BlockType					m_Type;
	//std::weak_ptr<CPickup> m_pPickup;


};