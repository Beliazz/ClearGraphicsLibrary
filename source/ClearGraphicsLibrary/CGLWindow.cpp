#include "cgl.h"

//////////////////////////////////////////////////////////////////////////
// CGLWindow
cgl::CGLWindow::CGLWindow( LPCWSTR title , UINT width, UINT height, WNDPROC messageProc, HINSTANCE instance /*= NULL*/, LPCWSTR cursorName /*= IDC_ARROW */ ) :
	CGLBase("CGLWindow"),
	m_width(width),
	m_height(height),
	m_title(title),
	m_cursorName(cursorName),
	m_messageProc(messageProc),
	m_instance(instance)
{

}
HRESULT cgl::CGLWindow::onRestore( )
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
void cgl::CGLWindow::onReset( )
{
	if (get())
	{
		UnregisterClassW(m_title, NULL);
		DestroyWindow(get());
		set(NULL);
	}
}
cgl::CGLWindow::~CGLWindow()
{
	onReset();
}


