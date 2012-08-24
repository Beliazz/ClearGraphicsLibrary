#include "cgl.h"

//////////////////////////////////////////////////////////////////////////
// CGLWindowFromConfig
// 
cgl::CGLWindowFromConfig::CGLWindowFromConfig( LPCWSTR title , UINT width, UINT height, WNDPROC messageProc, HINSTANCE instance /*= NULL*/, LPCWSTR cursorName /*= IDC_ARROW */ ) :
	CGLWindow("CGLWindowFromConfig"),
	m_width(width),
	m_height(height),
	m_title(title),
	m_cursorName(cursorName),
	m_messageProc(messageProc),
	m_instance(instance)
{

}
HRESULT cgl::CGLWindowFromConfig::onRestore( )
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
void cgl::CGLWindowFromConfig::onReset( )
{
	if (get())
	{
		UnregisterClassW(m_title, NULL);
		DestroyWindow(get());
		set(NULL);
	}
}
cgl::PCGLWindow cgl::CGLWindowFromConfig::Create( LPCWSTR title , UINT width, UINT height, WNDPROC messageProc, HINSTANCE instance /*= NULL*/, LPCWSTR cursorName /*= IDC_ARROW */ )
{
	return PCGLWindow(new CGLWindowFromConfig(title, width, height, messageProc, instance, cursorName));
}
cgl::CGLWindowFromConfig::~CGLWindowFromConfig()
{
	onReset();
}

//////////////////////////////////////////////////////////////////////////
// CGLWindowFromExisting
// 
cgl::CGLWindowFromExisting::CGLWindowFromExisting(HWND window)
	: CGLWindow("CGLWindowFromExisting"), m_window(window)
{

}
HRESULT cgl::CGLWindowFromExisting::onRestore()
{
	set(m_window);
	return S_OK;
}
void cgl::CGLWindowFromExisting::onReset( )
{
	set(NULL);
}
cgl::PCGLWindow cgl::CGLWindowFromExisting::Create( HWND window )
{
	return PCGLWindow(new CGLWindowFromExisting(window));
}
cgl::CGLWindowFromExisting::~CGLWindowFromExisting()
{
	onReset();
}
