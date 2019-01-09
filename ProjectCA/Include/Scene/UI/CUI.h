#pragma once

#include "..\..\..\stdafx.h"
#include "..\CEntity.hpp"


class CUI : public CEntity
{
public:
	CUI();
	virtual ~CUI();


public:
	virtual bool PostInit(const ENTITY_DATA& data, CScene* pScene) override;
	virtual void Render(const HDC& hDC) override;
	virtual void LateUpdate() override;



};