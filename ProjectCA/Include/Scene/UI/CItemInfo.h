#pragma once

#include "..\..\..\stdafx.h"
#include "CInterface.h"


class CItemInfo : public CInterface
{
	enum PickupInfo {
		PI_EMPTY = 0, PI_MUSHROOM, PI_FLOWER
	};
public:
	CItemInfo();
	virtual ~CItemInfo();


public:
	virtual bool PostInit(const OBJECT_DATA& data, CScene* pScene);
	virtual void Init();
	virtual void Update(double dDeltaTime);
	virtual void Render(const HDC& hDC);


public:
	void CheckStoredPickup();


private:
	std::weak_ptr<class CPlayer> m_pPlayer;
	typedef std::vector<std::weak_ptr<class CSprite>> InfoImageList;
	InfoImageList	m_InfoImageList;

};