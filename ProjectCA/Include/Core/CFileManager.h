#pragma once

#include "..\..\stdafx.h"
#include "..\Singleton.hpp"


class CFileManager : public Singleton<CFileManager>{

	DECLARE_SINGLETON(CFileManager)

public:
	bool ReadFile(const wchar_t* path);
	bool WriteFile(const wchar_t* path);


private:

	
};