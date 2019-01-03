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
	virtual bool PostInit(const ENTITY_DATA& data, CScene* pScene) override;
	virtual void Update(double dDeltaTime) override;


public:
	void CheckStoredPickup();


private:
	std::weak_ptr<class CPlayer> m_pPlayer;
	typedef std::vector<std::weak_ptr<class CSprite>> InfoImageList;
	InfoImageList	m_InfoImageList;

};