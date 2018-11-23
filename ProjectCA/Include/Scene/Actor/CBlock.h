#pragma once

#include "..\..\..\stdafx.h"
#include "..\CObject.h"

class CPickup;

class CBlock : public CObject
{
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


private:
	std::shared_ptr<CPickup> m_pPickup;


};