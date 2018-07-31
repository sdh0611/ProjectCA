#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Core\Debuging\Debug.h"

Debug::Debug()
	:m_pMemPool(nullptr), m_iMaxPoolSize(4096) ,m_iMaxLength(128)
{

}

Debug::~Debug(){ }

bool Debug::Init(UINT iPoolSize) 
{
	if (iPoolSize > m_iMaxPoolSize)
		return false;

	m_pMemPool = std::shared_ptr<BYTE>(new BYTE(m_iMaxLength), [](BYTE* ptr) { delete[] ptr; });
	

	return true;

}

bool Debug::Resize(UINT iPoolSize)
{
	if (iPoolSize > m_iMaxPoolSize)
		return false;

	//m_pMemPool.

	return true;
}

void Debug::MessageInfo(const Types::tstring& text)
{
	MessageBox(NULL, text.c_str(), TEXT("Info"), MB_ICONINFORMATION | MB_OK);

}

void Debug::MeesageError(const Types::tstring& text)
{
	MessageBox(NULL, text.c_str(), TEXT("Error"), MB_ICONERROR | MB_OK);


}

void Debug::DrawingText(const Types::tstring & text, const HDC & hDC, int x, int y)
{
	TextOut(hDC, x, y, text.c_str(), text.length());
}
