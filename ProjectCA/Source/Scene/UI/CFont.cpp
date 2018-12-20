#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Scene\UI\CFont.h"
#include "..\..\..\Include\Core\CResourceManager.h"
#include "..\..\..\Include\Core\Graphic\CSprite.h"
#include "..\..\..\Include\Core\Components\ImageRender.h"
#include "..\..\..\Include\Core\Components\TransformComponent.h"


CFont::CFont()
{
}

CFont::~CFont()
{
}

bool CFont::PostInit(const OBJECT_DATA & data, CScene * pScene)
{
	if (!CInterface::PostInit(data, pScene))
		return false;

	auto pResourceMgr = CResourceManager::GetInstance();

	//Lower
	m_FontTable.emplace('a', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontLowerA")).lock());
	m_FontTable.emplace('b', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontLowerB")).lock());
	m_FontTable.emplace('c', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontLowerC")).lock());
	m_FontTable.emplace('d', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontLowerD")).lock());
	m_FontTable.emplace('e', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontLowerE")).lock());
	m_FontTable.emplace('f', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontLowerF")).lock());
	m_FontTable.emplace('g', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontLowerG")).lock());
	m_FontTable.emplace('h', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontLowerH")).lock());
	m_FontTable.emplace('i', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontLowerI")).lock());
	m_FontTable.emplace('j', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontLowerJ")).lock());
	m_FontTable.emplace('k', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontLowerK")).lock());
	m_FontTable.emplace('l', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontLowerL")).lock());
	m_FontTable.emplace('m', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontLowerM")).lock());
	m_FontTable.emplace('n', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontLowerN")).lock());
	m_FontTable.emplace('o', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontLowerO")).lock());
	m_FontTable.emplace('p', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontLowerP")).lock());
	m_FontTable.emplace('q', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontLowerQ")).lock());
	m_FontTable.emplace('r', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontLowerR")).lock());
	m_FontTable.emplace('s', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontLowerS")).lock());
	m_FontTable.emplace('t', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontLowerT")).lock());
	m_FontTable.emplace('u', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontLowerU")).lock());
	m_FontTable.emplace('v', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontLowerV")).lock());
	m_FontTable.emplace('w', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontLowerW")).lock());
	m_FontTable.emplace('x', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontLowerX")).lock());
	m_FontTable.emplace('y', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontLowerY")).lock());
	m_FontTable.emplace('z', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontLowerZ")).lock());


	//Upper
	m_FontTable.emplace('A', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontUpperA")).lock());
	m_FontTable.emplace('B', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontUpperB")).lock());
	m_FontTable.emplace('C', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontUpperC")).lock());
	m_FontTable.emplace('D', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontUpperD")).lock());
	m_FontTable.emplace('E', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontUpperE")).lock());
	m_FontTable.emplace('F', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontUpperF")).lock());
	m_FontTable.emplace('G', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontUpperG")).lock());
	m_FontTable.emplace('H', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontUpperH")).lock());
	m_FontTable.emplace('I', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontUpperI")).lock());
	m_FontTable.emplace('J', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontUpperJ")).lock());
	m_FontTable.emplace('K', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontUpperK")).lock());
	m_FontTable.emplace('L', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontUpperL")).lock());
	m_FontTable.emplace('M', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontUpperM")).lock());
	m_FontTable.emplace('N', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontUpperN")).lock());
	m_FontTable.emplace('O', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontUpperO")).lock());
	m_FontTable.emplace('P', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontUpperP")).lock());
	m_FontTable.emplace('Q', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontUpperQ")).lock());
	m_FontTable.emplace('R', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontUpperR")).lock());
	m_FontTable.emplace('S', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontUpperS")).lock());
	m_FontTable.emplace('T', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontUpperT")).lock());
	m_FontTable.emplace('U', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontUpperU")).lock());
	m_FontTable.emplace('V', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontUpperV")).lock());
	m_FontTable.emplace('W', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontUpperW")).lock());
	m_FontTable.emplace('X', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontUpperX")).lock());
	m_FontTable.emplace('Y', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontUpperY")).lock());
	m_FontTable.emplace('Z', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontUpperZ")).lock());


	//Number
	m_FontTable.emplace('0', pResourceMgr->GetWeakSprtiePtr(TEXT("UINumberWhite0")).lock());
	m_FontTable.emplace('1', pResourceMgr->GetWeakSprtiePtr(TEXT("UINumberWhite1")).lock());
	m_FontTable.emplace('2', pResourceMgr->GetWeakSprtiePtr(TEXT("UINumberWhite2")).lock());
	m_FontTable.emplace('3', pResourceMgr->GetWeakSprtiePtr(TEXT("UINumberWhite3")).lock());
	m_FontTable.emplace('4', pResourceMgr->GetWeakSprtiePtr(TEXT("UINumberWhite4")).lock());
	m_FontTable.emplace('5', pResourceMgr->GetWeakSprtiePtr(TEXT("UINumberWhite5")).lock());
	m_FontTable.emplace('6', pResourceMgr->GetWeakSprtiePtr(TEXT("UINumberWhite6")).lock());
	m_FontTable.emplace('7', pResourceMgr->GetWeakSprtiePtr(TEXT("UINumberWhite7")).lock());
	m_FontTable.emplace('8', pResourceMgr->GetWeakSprtiePtr(TEXT("UINumberWhite8")).lock());
	m_FontTable.emplace('9', pResourceMgr->GetWeakSprtiePtr(TEXT("UINumberWhite9")).lock());


	//etc
	m_FontTable.emplace('?', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontQM")).lock());
	m_FontTable.emplace('!', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontEM")).lock());
	m_FontTable.emplace('#', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontSharp")).lock());
	m_FontTable.emplace('-', pResourceMgr->GetWeakSprtiePtr(TEXT("UIFontLine")).lock());



	m_iFontInterval		= m_FontTable['a'].lock()->GetBitWidth();
	m_iFontWidth		= m_iEntityWidth;
	m_iFontHeight		= m_iEntityHeight;
	m_strSentence		= TEXT("");

	return true;
}

void CFont::Init()
{
	CInterface::Init();
}

void CFont::Update(double dDeltaTime)
{
	CInterface::Update(dDeltaTime);
}

void CFont::Render(const HDC & hDC)
{
	if (m_bActive)
	{
		if (m_strSentence== TEXT(""))
			return;

		auto pRender = GetComponent<ImageRender>().lock();
		auto position = GetTransform().lock()->GetPivot();

		size_t length = m_strSentence.length();
		for (int index = 0; index < length; ++index)
		{
			if (m_strSentence[index] != ' ')
			{
				pRender->Draw(hDC, position, m_FontTable[m_strSentence[index]].lock());
			}

			position.x += m_iFontInterval;

		}
	}

}

void CFont::ResizeFontContainer()
{
	UINT iMax = 0;



	m_iEntityWidth = m_strSentence.length() * m_iFontWidth;
	GetTransform().lock()->AdjustPivot();
}

void CFont::SetFontInterval(UINT iInteval)
{
	m_iFontInterval = iInteval;
}

void CFont::SetFontWidth(UINT iWidth)
{
	m_iFontWidth = iWidth;
	GetComponent<ImageRender>().lock()->SetDrawWidth(iWidth);
}

void CFont::SetFontHeight(UINT iHeight)
{
	m_iFontHeight = iHeight;
	GetComponent<ImageRender>().lock()->SetDrawWidth(iHeight);
}

void CFont::SetFontSize(UINT iWidth, UINT iHeight)
{
	m_iFontWidth	= iWidth;
	m_iFontHeight	= iHeight;
	GetComponent<ImageRender>().lock()->SetDrawSize(iWidth, iHeight);
}

void CFont::SetSentence(const TSTRING & strSentence)
{
	m_strSentence = strSentence;
	ResizeFontContainer();
}
