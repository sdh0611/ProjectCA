#pragma once

#include "..\..\..\stdafx.h"
#include "..\CObject.h"


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



private:
	std::shared_ptr<class CActor> m_pPickup;


};