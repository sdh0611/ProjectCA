#pragma once

#include "..\..\stdafx.h"
#include "..\Singleton.hpp"


class CScoreManager : public Singleton<CScoreManager> {

	DECLARE_SINGLETON(CScoreManager)
	
public:
	void Init();
	
	
public:
	void IncreaseCoinCount();
	void IncreaseScore(int iScore);
	void IncreaseScore(OBJECT_TYPE);
	void IncreaseLifeCount(int iCount);
	void DecreaseLifeCount();


public:
	int GetScore() const;
	int GetCoinCount() const;
	int GetLifeCount() const;
	const int* GetScorePtr() const;
	const int* GetCointCountPtr() const;
	const int* GetLifeCountPtr() const;


private:
	int		m_iCurScore;
	int		m_iCoinCount;
	int		m_iLifeCount;


};