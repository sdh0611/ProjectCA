#pragma once

#include "..\..\..\stdafx.h"
#include "..\CObject.h"

class CPickup;

class CBlock : public CObject
{
	enum BlockType {
		BT_DEFAULT, BT_HIDE
	};

public:
	CBlock();
	virtual ~CBlock();


public:
	virtual bool PostInit(const OBJECT_DATA& objectData, CScene* pScene);
	virtual void Init();
	virtual void Update(double dDeltaTime);
	virtual void Render(const HDC& hDC);
	virtual void LateUpdate();


public:
	void SetStoredPickup(std::shared_ptr<CPickup> pPickup);
	void SetHide();


public:
	bool IsHiding() const;


private:
	void SetDead();


private:
	bool							m_bHiding;
	BlockType					m_Type;
	std::weak_ptr<CPickup> m_pPickup;


};