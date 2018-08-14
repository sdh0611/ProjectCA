#pragma once
/*
	NOTE:
		Resource들의 경로를 관리하기 위한 Manager Class.
		Singleton Class로 작성
*/

#include "..\..\stdafx.h"
#include "..\Singleton.hpp"


class CResourceManager : public Singleton<CResourceManager> {

	DECLARE_SINGLETON(CResourceManager)

public:



public:
	const Types::tstring& FindPath(const Types::tstring& path);
	bool AddPath(const Types::tstring& pathTag, const Types::tstring& path);
	


private:



private:
	typedef std::unordered_map<Types::tstring, std::list<Types::tstring>>	ResourcePathTable;
	ResourcePathTable				m_pathTable;	


};