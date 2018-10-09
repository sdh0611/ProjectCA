#include "..\..\..\Include\Core\Window\MainWindow.h"
#include "..\..\..\Include\Scene\CSceneManager.h"
//#include "..\..\..\Include\Scene\Actor\CActorManager.h"
#include "..\..\..\Include\Core\Timer.h"
#include "..\..\..\Include\Core\Debuging\Debug.h"
#include "..\..\..\Include\Core\Window\BackBuffer.h"
#include "..\..\..\Include\Core\CResourceManager.h"


//초기화는 Init() 메소드에서 실시함.
MainWindow::MainWindow()
	:m_pSceneManager(nullptr), m_pTimer(nullptr), m_pBackBuffer(nullptr)
{
	
}

//Heap할당 객체들, DC 정리해줄 것.
MainWindow::~MainWindow()
{
	//Manager Class 정리
	m_pSceneManager->Destroy();
	m_pTimer->Destroy();
	//CActorManager::Destroy();

	ReleaseDC(m_hWnd, m_hDC);
	//DeleteDC(m_hMemDC);

}

//실질적인 MainWindow 초기화 및 생성작업이 이루어짐.
// + Manager Class들의 Loading 수행
bool MainWindow::Init(HINSTANCE hInstance, UINT iWidth, UINT iHeight)
{
	m_hInstance = hInstance;
	m_iWidth = iWidth;
	m_iHeight = iHeight;

	//생성 실패할 경우 return false
	if (!CreateMyWindow(TEXT("MainWindowClass"), TEXT("MainWindow"), TRUE))
		return false;

	m_hDC = GetDC(m_hWnd);
	//m_hMemDC = CreateCompatibleDC(m_hDC);


	//Get Manager Classes Pointer;
	m_pSceneManager = CSceneManager::GetInstance();
	m_pTimer = Timer::GetInstance();
	m_pResourceManager = CResourceManager::GetInstance();

	if (!m_pResourceManager->Init())
		return false;

	//Initializing Manager Classes
	if (!m_pSceneManager->Init(Types::ST_GAME))		//Test용으로 GameScene 생성.
		return false;

	if (!m_pTimer->Init())
		return false;

	if (m_pBackBuffer == nullptr)
		m_pBackBuffer = std::make_unique<BackBuffer>(m_hDC);

	//m_hBackground = (HBITMAP)LoadImage(m_hInstance, TEXT("D:/Projects/VIsualStudio17/ProjectCA/ProjectCA/Resources/Sprite/Background/background_mountain1.bmp"),
	//	IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	return true;
}

//DeltaTime값을 넘겨줌.
void MainWindow::Update()
{
	m_pTimer->Update();
	m_pSceneManager->Update(m_pTimer->GetDeltaTime());
	m_pTimer->CheckFrameCount();
	//m_pSceneManager->Update(1.f/60.f);
	//InvalidateRect(m_hWnd, NULL, FALSE);
}


void MainWindow::Render()
{
	
	if (!m_pBackBuffer->DrawSet(m_hDC))
		return ;

	//HDC hMemDC = CreateCompatibleDC(m_pBackBuffer->GetMemDC());
	//HBITMAP hOldBit = (HBITMAP)SelectObject(hMemDC, m_hBackground);
	//BitBlt(m_pBackBuffer->GetMemDC(), 0, 0, MAX_WIDTH, MAX_HEIGHT, hMemDC, 0, 0, SRCCOPY);
	//SelectObject(hMemDC, hOldBit);
	//
	m_pSceneManager->Render(m_pBackBuffer->GetMemDC());
	m_pTimer->DrawFPS(m_pBackBuffer->GetMemDC());
	BitBlt(m_hDC, 0, 0, MAX_WIDTH, MAX_HEIGHT, m_pBackBuffer->GetMemDC(), 0, 0, SRCCOPY);

	m_pBackBuffer->DrawEnd();

	//DeleteDC(hMemDC);
}


void MainWindow::DeadtimeLogic()
{
	Update();
	Render();
}


bool MainWindow::CreateMyWindow(const Types::tstring& wndClassName,
	const Types::tstring& wndName, BOOL bIsVisible)
{
	m_WndClass.cbClsExtra = 0;
	m_WndClass.cbWndExtra = 0;
	m_WndClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	m_WndClass.hCursor = LoadCursor(m_hInstance, IDC_ARROW);
	m_WndClass.hIcon = LoadIcon(m_hInstance, IDI_APPLICATION);
	m_WndClass.hInstance = m_hInstance;
	m_WndClass.lpfnWndProc = WndProc;
	m_WndClass.lpszClassName = wndClassName.c_str();
	m_WndClass.lpszMenuName = NULL;
	m_WndClass.style = CS_HREDRAW | CS_VREDRAW;

	if (!RegisterClass(&m_WndClass))
		return false;

	m_hWnd = CreateWindowW(wndClassName.c_str(), wndName.c_str(), WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME, CW_USEDEFAULT, CW_USEDEFAULT,
		m_iWidth, m_iHeight, HWND_DESKTOP, NULL, m_hInstance, nullptr);

	//창이 생성되지 않은 경우
	if (m_hWnd == nullptr)
		return false;

	ShowWindow(m_hWnd, bIsVisible);

	//Debug::MessageInfo(TEXT("윈도우 생성!!"));

	return true;

}


bool MainWindow::SetVisible(HWND hWnd, BOOL bIsVisible)
{
	return ( ShowWindow(hWnd, bIsVisible) ? true : false );
}


void MainWindow::SetWndSize(UINT iWidth, UINT iHeight)
{
	m_iWidth = iWidth;
	m_iHeight = iHeight;
}


UINT MainWindow::MessageLoop()
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

	return (UINT)msg.wParam;

	return 0;
}


LRESULT MainWindow::WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	//PAINTSTRUCT ps;
	//HDC hDC;

	switch (iMsg) {
	case WM_CREATE:
		SetTimer(hWnd, 1, 10, NULL);
		return 0;
	case WM_TIMER:
		InvalidateRect(hWnd, nullptr, FALSE);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	//case WM_PAINT:
	//	hDC = BeginPaint(hWnd, &ps);
	//	EndPaint(hWnd, &ps);
	//	ReleaseDC(hWnd, hDC);
	//	return 0;
	}

	return DefWindowProc(hWnd, iMsg, wParam, lParam);
}