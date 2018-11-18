#pragma once
/*
	NOTE:
		Resource들의 경로를 관리하기 위한 Manager Class.
		Singleton Class로 작성
*/

#include "..\..\stdafx.h"
#include "..\Singleton.hpp"


class CSprite;

typedef std::shared_ptr<CSprite> StrongSpritePtr;
typedef std::weak_ptr<CSprite> WeakSpritePtr;

class CResourceManager : public Singleton<CResourceManager> {

	DECLARE_SINGLETON(CResourceManager)

public:



public:
	//const Types::tstring& FindPath(const Types::tstring& path);
	//bool AddPath(const Types::tstring& pathTag, const Types::tstring& path);
	bool Init();
	

public:
	WeakSpritePtr GetWeakSprtiePtr(const Types::tstring& strName);
	bool AddSpritePtr(const Types::tstring& strName, CSprite* pSprite);
	bool DeleteSpritePtr(const Types::tstring& strName);
	bool AddSpriteUsePath(const Types::tstring& strPath, const Types::tstring& strName);


private:
	bool LoadPlayerSpriteInfo();
	bool LoadEnemySpriteInfo();
	bool LoadGroundTileImageInfo();
	bool LoadBackgroundImageInfo();
	bool LoadUIImageInfo();
	bool LoadPickupImageInfo();
	bool LoadBlockImageInfo();
	bool LoadEffectImageInfo();



private:
	typedef std::unordered_map<Types::tstring, std::list<Types::tstring>>	ResourcePathTable;
	ResourcePathTable				m_pathTable;	

	typedef std::unordered_map<Types::tstring, StrongSpritePtr> StrongSpritePtrTable;
	StrongSpritePtrTable				m_strongSpriteTable;

};