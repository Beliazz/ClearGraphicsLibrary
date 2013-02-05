#include "cgl.h"

//////////////////////////////////////////////////////////////////////////
// cgl window base class
void cgl::util::CGLWindow::PeekMessages()
{
	MSG msg = MSG();
	while(PeekMessage(&msg, get(), 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
void cgl::util::CGLWindow::GetMessages()
{
	MSG msg = MSG();
	while(GetMessage(&msg, get(), 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
//
// static functions
void cgl::util::CGLWindow::PeekMessages(HWND handle)
{
	MSG msg = MSG();
	while(PeekMessage(&msg, handle, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
void cgl::util::CGLWindow::GetMessages(HWND handle)
{
	MSG msg = MSG();
	while(GetMessage(&msg, handle, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

//////////////////////////////////////////////////////////////////////////
// CGLWindowFromConfig
// 
cgl::util::CGLWindowFromConfig::CGLWindowFromConfig( LPCWSTR title , UINT width, UINT height, WNDPROC messageProc, HINSTANCE instance /*= NULL*/, LPCWSTR cursorName /*= IDC_ARROW */ ) :
	CGLWindow("CGLWindowFromConfig"),
	m_width(width),
	m_height(height),
	m_title(title),
	m_cursorName(cursorName),
	m_messageProc(messageProc),
	m_instance(instance)
{

}
HRESULT cgl::util::CGLWindowFromConfig::onRestore( )
{
	// create window class
	WNDCLASSEX wcex;
	ZeroMemory(&wcex, sizeof(wcex));
	wcex.cbSize = sizeof( WNDCLASSEX );
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = m_messageProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = NULL;
	wcex.hIcon =NULL;
	wcex.hCursor = LoadCursor( NULL, m_cursorName);
	wcex.hbrBackground = ( HBRUSH )( COLOR_WINDOW + 1 );
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = m_title;
	wcex.hIconSm = NULL;

	if( !RegisterClassExW( &wcex ) )
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	// set size and position
	RECT rc = {0, 0, m_width, m_height};
	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );

	// create window
	set(CreateWindow(m_title, 
					 m_title, 
					 WS_OVERLAPPEDWINDOW, 
					 CW_USEDEFAULT, 
					 CW_USEDEFAULT,
					 rc.right - rc.left, 
					 rc.bottom - rc.top,
					 NULL,
					 NULL,
					 NULL,
					 NULL));

	if (!get())
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	// show window
	ShowWindow(get(), 1);

	return S_OK;
}
void cgl::util::CGLWindowFromConfig::onReset( )
{
	DestroyWindow(get());
	UnregisterClassW(m_title, NULL);
	set(NULL);
}
cgl::util::PCGLWindow cgl::util::CGLWindowFromConfig::Create( LPCWSTR title , UINT width, UINT height, WNDPROC messageProc, HINSTANCE instance /*= NULL*/, LPCWSTR cursorName /*= IDC_ARROW */ )
{
	return create(new CGLWindowFromConfig(title, width, height, messageProc, instance, cursorName));
}
cgl::util::CGLWindowFromConfig::~CGLWindowFromConfig()
{
	onReset();
}

//////////////////////////////////////////////////////////////////////////
// CGLWindowFromExisting
// 
cgl::util::CGLWindowFromExisting::CGLWindowFromExisting(HWND window)
	: CGLWindow("CGLWindowFromExisting"), m_window(window)
{

}
HRESULT cgl::util::CGLWindowFromExisting::onRestore()
{
	set(m_window);
	return S_OK;
}
void cgl::util::CGLWindowFromExisting::onReset( )
{
	set(NULL);
}
cgl::util::PCGLWindow cgl::util::CGLWindowFromExisting::Create( HWND window )
{
	return create(new CGLWindowFromExisting(window));
}
cgl::util::CGLWindowFromExisting::~CGLWindowFromExisting()
{
	onReset();
}
