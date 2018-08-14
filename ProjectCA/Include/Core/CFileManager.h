#pragma once

#include "..\..\stdafx.h"
#include "..\Singleton.hpp"

class CFileManager : public Singleton<CFileManager> {

	DECLARE_SINGLETON(CFileManager)

public:
	bool ReadXMLFile(const wchar_t* strFilePath);
	bool ReadCSVFile(const wchar_t* strFilePath);


public:



private:



private:
	std::ostream			m_outStream;
	std::ofstream		m_fStream;

};