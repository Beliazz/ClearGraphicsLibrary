#ifndef D3D11Window_h__
#define D3D11Window_h__

#include "cgl.h"

namespace cgl
{

//////////////////////////////////////////////////////////////////////////
// typedefs
class CGLWindow;
typedef CGL_API std::tr1::shared_ptr<CGLWindow> PCGLWindow;

class CGL_API CGLWindow : public CGLBase<HWND__>
{
private:
	UINT m_width;
	UINT m_height;
	LPCWSTR m_title;
	LPCWSTR m_cursorName;
	WNDPROC m_messageProc;
	HINSTANCE m_instance;

protected:
	void getDependencies(std::vector<PCGLObject>* pDependencies )  { }


public:	
	CGLWindow(LPCWSTR title , 
			  UINT width, 
			  UINT height, 
			  WNDPROC messageProc, 
			  HINSTANCE instance = NULL, 
			  LPCWSTR cursorName = IDC_ARROW );
	~CGLWindow();

	HRESULT onRestore();
	void onReset();
};



}

#endif // D3D11Window_h__
