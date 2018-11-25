#include "..\..\stdafx.h"
#include "..\..\Include\Scene\CScoreManager.h"


CScoreManager::CScoreManager()
{
}

CScoreManager::~CScoreManager()
{
}

void CScoreManager::Init()
{
	m_iCurScore = 0;
	m_iCoinCount = 96;
	m_iLifeCount = 5;
}


void CScoreManager::IncreaseCoinCount()
{
	if (++m_iCoinCount >= 100)
	{
		m_iCoinCount %= 100;
		++m_iLifeCount;
	}
}

void CScoreManager::IncreaseScore(int iScore)
{
	if (iScore < 0)
		return;

	m_iCurScore += iScore;
}

void CScoreManager::IncreaseScore(OBJECT_TYPE)
{
}

void CScoreManager::IncreaseLifeCount(int iCount)
{
	if (iCount < 0)
		return;
	m_iLifeCount += iCount;
}

void CScoreManager::DecreaseLifeCount()
{
	if(m_iLifeCount > 0)
		--m_iLifeCount;
}

int CScoreManager::GetScore() const
{
	return m_iCurScore;
}

int CScoreManager::GetCoinCount() const
{
	return m_iCoinCount;
}

int CScoreManager::GetLifeCount() const
{
	return m_iLifeCount;
}

const int * CScoreManager::GetScorePtr() const
{
	return &m_iCurScore;
}

const int * CScoreManager::GetCointCountPtr() const
{
	return &m_iCoinCount;
}

const int * CScoreManager::GetLifeCountPtr() const
{
	return &m_iLifeCount;
}
