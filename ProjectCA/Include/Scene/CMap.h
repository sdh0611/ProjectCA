#pragma once

/*
	NOTE:
		Stage Map파일 저장 및 운용을 위한 Class

*/

#include "..\..\stdafx.h"

typedef	std::vector<std::vector<Types::ObjectType>> MapVector;

struct TileInfo {

	

};


class CMap {

public:
	CMap();
	~CMap();


public:



private:



private:
	UINT						m_iMapWidth;
	UINT						m_iMapHeight;
	MapVector				m_mapState;


};