#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Core\Window\MyWindow.h"
#include "..\..\..\Include\Scene\CSceneManager.h"

//이 부분은 나중에 상속받는 부분에서 따로 정의해서써도 무방.
LRESULT MyWindow::WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC;

	switch (iMsg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		ReleaseDC(hWnd, hDC);
		return 0;
	}

	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}


void MyWindow::DeadtimeLogic()
{
}

bool MyWindow::CreateMyWindow(const Types::tstring& wndClassName,
	const Types::tstring& wndName, BOOL bIsVisible)
{
	m_WndClass.cbClsExtra = 0;
	m_WndClass.cbWndExtra = 0;
	m_WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	m_WndClass.hCursor = LoadCursor(m_hInstance, IDC_ARROW);
	m_WndClass.hIcon = LoadIcon(m_hInstance, IDI_APPLICATION);
	m_WndClass.hInstance = m_hInstance;
	m_WndClass.lpfnWndProc = WndProc;
	m_WndClass.lpszClassName = TEXT("MainWindow");
	m_WndClass.lpszMenuName = NULL;
	m_WndClass.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClass(&m_WndClass))
		return false;

	m_hWnd = CreateWindowW(wndClassName.c_str(), wndName.c_str(), WS_OVERLAPPED & ~WS_THICKFRAME, CW_USEDEFAULT, CW_USEDEFAULT,
		m_iWidth, m_iHeight, HWND_DESKTOP, NULL, m_hInstance, nullptr);

	//창이 생성되지 않은 경우
	if (m_hWnd == nullptr)
		return false;

	ShowWindow(m_hWnd, bIsVisible);

	return true;

}

UINT MyWindow::MessageLoop()
{
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, nullptr, NULL, NULL, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			DeadtimeLogic();

		}

	}

	return msg.wParam;

	return 0;
}


MyWindow::~MyWindow()
{
	ReleaseDC(m_hWnd, m_hDC);
}

bool MyWindow::Init(HINSTANCE hInstance, UINT iWidth, UINT iHeight)
{
	m_hInstance = hInstance;
	m_iWidth = iWidth;
	m_iHeight = iHeight;

	m_hDC = GetDC(m_hWnd);

	return true;
}

void MyWindow::Update(const float fDeltaTime)
{


}

void MyWindow::Render(const float fDeltaTime)
{



}


bool MyWindow::SetVisible(HWND hWnd, BOOL bIsVisible)
{
	return (ShowWindow(hWnd, bIsVisible) ? true : false);
}

void MyWindow::SetWndSize(UINT iWidth, UINT iHeight)
{
	m_iWidth = iWidth;
	m_iHeight = iHeight;

}


