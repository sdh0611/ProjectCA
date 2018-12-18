#pragma once

#include "..\..\..\stdafx.h"
#include "CRandomBlock.h"

class CPickup;

class CPickupBlock : public CRandomBlock
{
public:
	CPickupBlock();
	virtual ~CPickupBlock();


public:
	virtual bool PostInit(const OBJECT_DATA& objectData, CScene* pScene);
	virtual void Init();
	virtual void Update(double dDeltaTime);
	

public:
	void			SetStoredPickup(std::shared_ptr<CPickup> pPickup);
	virtual void	HandlingEvent(EVENT_TYPE type) override;


private:
	std::weak_ptr<CPickup> m_pPickup;

};