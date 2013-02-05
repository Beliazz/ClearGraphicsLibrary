#ifndef D3D11Window_h__
#define D3D11Window_h__

#include "cgl.h"

namespace cgl
{
	namespace util
	{
		//////////////////////////////////////////////////////////////////////////
		// typedefs
		class CGLWindow;
		typedef CGL_API std::tr1::shared_ptr<CGLWindow> PCGLWindow;

		class CGL_API CGLWindow : public cgl::core::CGLBase<HWND__>
		{
		public:
			CGLWindow(std::string className) : CGLBase(className) {}

			void PeekMessages();
			void GetMessages();

			static void PeekMessages(HWND handle);
			static void GetMessages(HWND handle);
		};

		class CGL_API CGLWindowFromConfig : public CGLWindow
		{
		private:
			UINT m_width;
			UINT m_height;
			LPCWSTR m_title;
			LPCWSTR m_cursorName;
			WNDPROC m_messageProc;
			HINSTANCE m_instance;

		protected:
			void getDependencies(std::vector<cgl::core::PCGLObject>* pDependencies )  { }
			HRESULT onRestore();
			void onReset();

			CGLWindowFromConfig(LPCWSTR title , 
				UINT width, 
				UINT height, 
				WNDPROC messageProc, 
				HINSTANCE instance = NULL, 
				LPCWSTR cursorName = IDC_ARROW );

		public:	
			~CGLWindowFromConfig();

			static PCGLWindow Create(LPCWSTR title , 
				UINT width, 
				UINT height, 
				WNDPROC messageProc, 
				HINSTANCE instance = NULL, 
				LPCWSTR cursorName = IDC_ARROW );
		};

		class CGL_API CGLWindowFromExisting : public CGLWindow
		{
		private:
			HWND m_window;

		protected:
			void getDependencies(std::vector<cgl::core::PCGLObject>* pDependencies )  { }
			HRESULT onRestore();
			void onReset();

			CGLWindowFromExisting(HWND window);

		public:
			~CGLWindowFromExisting();
			static PCGLWindow Create(HWND window);
		};
	}
}

#endif // D3D11Window_h__
